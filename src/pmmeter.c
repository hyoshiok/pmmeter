/*
 * Copyright (c) 2013, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Intel Corporation nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
/*
 * The application can generate LMA (Local Memory Access) and RMA
 * (Remote Memory Access) with latency information on NUMA system.
 *
 * Please note the latencies reported by mgen are not the official data
 * from Intel/IBM. It is just a tool to test numatop.
 */

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sched.h>
#include <pthread.h>
#include <sys/mman.h> 
#include <sys/shm.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <libgen.h>
#include <inttypes.h>
#include <string.h>
#include <signal.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>
#include <numa.h>
#include <libpmem.h>
#include "../common/include/util.h"
#include "../common/include/os/os_util.h"
#include "./include/util.h"
#include "./definitions.h"
#include <locale.h>

/*-----------------------------------------------------------------------
 * INSTRUCTIONS:
 *
 *	1) STREAM requires different amounts of memory to run on different
 *           systems, depending on both the system cache size(s) and the
 *           granularity of the system timer.
 *     You should adjust the value of 'LAST_LEVEL_CACHE_SIZE' (below or
 *           passed at compile time) to allow the code to calculate how
 *           big each MPI processes' array should be. Array sizes should
 *           meet both of the following criteria:
 *       (a) Each array must be at least 4 times the size of the
 *           available last level cache memory.
 *       (b) The size should be large enough so that the 'timing calibration'
 *           output by the program is at least 20 clock-ticks.
 *           Example: most versions of Windows have a 10 millisecond timer
 *               granularity.  20 "ticks" at 10 ms/tic is 200 milliseconds.
 *               If the processor and memory are capable of 10 GB/s, this is 2 GB in 200 msec.
 *               This means the each array must be at least 1 GB.
 */
#ifndef LAST_LEVEL_CACHE_SIZE
#   define LAST_LEVEL_CACHE_SIZE	1000000
#endif

#define BW_OPS_COUNT                    1000L*1000L

/*  Users are allowed to modify the "OFFSET" variable, which *may* change the
 *         relative alignment of the arrays (though compilers may change the
 *         effective offset by making the arrays non-contiguous on some systems).
 *      Use of non-zero values for OFFSET can be especially helpful if the
 *         array size is set to a value close to a large power of 2.
 *      OFFSET can also be set on the compile line without changing the source
 *         code using, for example, "-DOFFSET=56".
 */
#ifndef OFFSET
#   define OFFSET	0
#endif

double s_nsofclk;
uint64_t s_clkofsec;
double s_latest_avglat = 0.0;
struct timeval s_tvbase, e_tvbase;

static int s_rand_arr[RAND_ARRAY_SIZE*32L*1024L]; // size of static int should be less than 2GB
//static int r_array_size[2] = {131072, 262144}; //, 1048576, 2097152, 4194304, 8388608, 16777216};
//static int r_array_size[10] = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288}; //, 1048576, 2097152, 4194304, 8388608, 16777216};
//static int r_array_size[10] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512}; //, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144}; //, 524288, 1048576, 2097152, 4194304, 8388608, 16777216};
#define BASE 8388608L
#define LEN 4194304L
//static int r_array_size[9] = {BASE, BASE+LEN, BASE+LEN*2, BASE+LEN*3, BASE+LEN*4, BASE+LEN*5, BASE+LEN*6, BASE+LEN*7, BASE+LEN*8};
//#define LEN 16
//static int r_array_size[25] = {2048, 2048+LEN, 2048+LEN*2, 2048+LEN*3, 2048+LEN*4, 2048+LEN*5, 2048+LEN*6, 2048+LEN*7, 2048+LEN*8, 2048+LEN*9, 2048+LEN*10, 2048+LEN*11, 2048+LEN*12, 2048+LEN*13, 2048+LEN*14, 2048+LEN*15, 2048+LEN*16, 2048+LEN*17, 2048+LEN*18, 2048+LEN*19, 2048+LEN*20, 2048+LEN*21, 2048+LEN*21, 2048+LEN*22, 2048+LEN*23, 4096}; // max size is 131072
//static int r_array_size[24] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 30, 32}; //, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128}; //, 2048, 2048+64, 2048+128, 2048+128+64, 2048+256, 2048+512, 2048+1024, 2048+1024+512}; // max size is 131072
static int r_array_size[1] = {131072};//, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128}; //, 2048, 2048+64, 2048+128, 2048+128+64, 2048+256, 2048+512, 2048+1024, 2048+1024+512}; // max size is 131072

#define NUM_THREAD 96 // max number of threads //single thread only
static int num_thread = 1;

static int rand_array_size;
static int s_ncpus;
static void *s_buf = NULL;
static void *d_buf = NULL;
static void *s_nvmbuf = NULL;
static void *d_nvmbuf = NULL;
static void *s_bufth[NUM_THREAD];
static void *d_bufth[NUM_THREAD];
static void *s_nvmbufth[NUM_THREAD];
static void *d_nvmbufth[NUM_THREAD];
static FILE *fp[NUM_THREAD];
static int bw_func[NUM_THREAD];
static double bw_number[NUM_THREAD];
static double bw_nvmnumber[NUM_THREAD];
static void *s_lbuf = NULL;
static void *d_lbuf = NULL;
static unsigned int s_randseed;

static int write_bw(void *, void *, size_t);
//static void buf_move(void *, void *, int);
static void *malloc_(size_t, int, int);
static void *buf_create(int, int);
static void *buf_persistent_create(int, int, size_t, char *, int);
static void buf_release(void *, size_t);
static void * buf_create_read(int, int, int, int, int);
static void * buf_persistent_create_read(void *arg);
static int nvm_release(int);
static void * dependent_read_bw_th (void *arg);
static int dependent_read(void *, int, int, int);
static int dependent_write(void *, int, int, int);
static int persistent_read(void *, int, int, int);

STREAM_TYPE	*a, *b, *c;

static double mysecond();
static void checkSTREAMresults(int array_size);
static int checktick();

int array_size;
int socket, core;
int performing_persistent;
benchmark_results b_results;
aggregate_results node_results;
aggregate_results a_results;
communicator world_comm, node_comm, root_comm;
static int optvalue[10];

long st;

static void
print_usage(const char *exec_name)
{
	char buffer[PATH_MAX];
	long i;

	strncpy(buffer, exec_name, PATH_MAX);
	buffer[PATH_MAX - 1] = 0;

	printf("Usage: %s [option(s): acdfhilnotsCN]\n", basename(buffer));
	printf("Options:\n"
	    "    -a: access pattern, 0:seq & random, 1:seq, 2:random\n"
	    "    -c: cache flush pattern, 0:clwb, 1:clflush, 2:clflushopt\n"
	    "    -d: pm device name, (default /mnt/pmem) \n"
	    "    -f: fence pattern, 0:mfence, 1:sfence, 2:lfence\n"
	    "    -h: print helps\n"
	    "    -i: i386 instruction, 0:movq, 1:vmov, 2:movb, 3:ntmov\n"
	    "    -l: 0:load & store, 1:load, 2:store\n"
	    "    -n: number of threads (default 1) \n"
	    "    -o: output file name (default stdio) \n"
            "    -t: the duration seconds for measuring. (default 1) \n" 
	    "    -x: 0:Bandwidth and Latency, 1:Bandwidth only, 2:Latency only\n"
	    "    -C: the cpu where creates a thread to access memory. (default 0) \n"
	    "    -N: the node where the memory is allocated on. (default 0) \n");
	printf("\nFor example:\n"
	    "    1. Generate LMA for 10s (memory allocated on node1, thread runs on cpu1):\n"
	    "       %s -N 1 -C 1 -t 10\n"
	    "    2. Generate RMA for 10s (memory allocated on node0, thread runs on cpu10):\n"
	    "       %s -N 0 -C 10 -t 10\n",
	    basename(buffer), basename(buffer));
}

static void
sigint_handler(int sig)
{
	switch (sig) {
	case SIGINT:
		(void) signal(SIGINT, sigint_handler);
		break;

	case SIGHUP:
		(void) signal(SIGINT, sigint_handler);
		break;
	
	case SIGQUIT:
		(void) signal(SIGINT, sigint_handler);
		break;

	case SIGPIPE:
		(void) signal(SIGINT, sigint_handler);
		break;

	case SIGTERM:
		(void) signal(SIGINT, sigint_handler);
		break;
	}

	printf("-------------------------\n");
	printf("%24s\n", "*** Terminated! ***");
	if (s_latest_avglat > 0.0) {
		printf("%9s  %13.1f\n\n", "Average", s_latest_avglat);
	} else {
		printf("%9s  %13.1f\n\n", "Average", 0.0);
	}

	if (s_buf != NULL) {
	  buf_release(s_buf, BUF_SIZE);
	}

	exit (0);
}

int
main(int argc, char *argv[])
{
	int node_alloc = 0, cpu_consumer = 0;
	int node, cpu;
	int meas_sec = MEAS_TIME_DEFAULT;
	int ret = -1;
	int c;
        int nvm = 1; // default nvm mode
	int is_pmem;
	size_t mapped_len;
	char path[MAX_FILE_NAME_LENGTH];
	char path2[MAX_FILE_NAME_LENGTH];
	long i, k;
	size_t buf_size;
	time_t tt;
	struct tm ti;
	char dt[40];
	pthread_t pt[NUM_THREAD];
	double bw_number_sum;
	double bw_nvmnumber_sum;

	s_randseed = 0;
	optind = 1;
	opterr = 0;

	while ((c = getopt(argc, argv, "C:N:a:c:d:f:hi:l:n:o:t:x:")) != EOF) {
		switch (c) {
		case 'C':
			cpu_consumer = atoi(optarg);
			break;

		case 'N':
			node_alloc = atoi(optarg);
			break;

		case 'h':
			print_usage(argv[0]);
			ret = 0;
			goto L_EXIT0;

		case 'a':
			optvalue[0] = atoi(optarg);
			break;
			
		case 'c':
			optvalue[1] = atoi(optarg);
			break;

		case 'd':
		        printf("not yet implemented. %c \n", c);
			print_usage(argv[0]);
			ret = 0;
			goto L_EXIT0;

		case 'f':
			optvalue[2] = atoi(optarg);
			break;

		case 'i':
			optvalue[3] = atoi(optarg);
			break;

		case 'l':
			optvalue[4] = atoi(optarg);
			break;

                case 'n':
		        num_thread = atoi(optarg);
			if (num_thread > NUM_THREAD){
			  printf("Number of pthread should be less than %d but -n %d\n", 
				 NUM_THREAD, num_thread);
			  print_usage(argv[0]);
			  goto L_EXIT0;
			};
                        break;
			
		case 'o':
		        printf("not yet implemented. %c \n", c);
			print_usage(argv[0]);
			ret = 0;
			goto L_EXIT0;

		case 't':
			meas_sec = atoi(optarg);
			break;

		case 'x':
			optvalue[5] = atoi(optarg);
			break;

		case ':':
			printf("Missed argument for option %c.\n",
			    optopt);
			print_usage(argv[0]);
			goto L_EXIT0;

		case '?':
			printf("Unrecognized option %c.\n", optopt);
			print_usage(argv[0]);
			goto L_EXIT0;
		}
	}

	time(&tt);
	localtime_r(&tt,&ti);
	strftime(dt,sizeof(dt),"%Y-%m-%d %H:%M:%S",&ti);
	printf("%s\n",dt);

	printf("command: ");
	for (i = 0; i < argc; i++){
	  printf("%s%s", argv[i], (i < argc-1) ? " " : "" );
	};
	printf("\n");


	s_ncpus = sysconf(_SC_NPROCESSORS_CONF);

	if (node_alloc == -1) {
		printf("Missed argument for option '-N'.\n");
		print_usage(argv[0]);
		goto L_EXIT0;
	}

	if (cpu_consumer == -1) {
		printf("Missed argument for option '-C'.\n");
		print_usage(argv[0]);
		goto L_EXIT0;
	}

	if ((signal(SIGINT, sigint_handler) == SIG_ERR) ||
	    (signal(SIGHUP, sigint_handler) == SIG_ERR) ||
	    (signal(SIGQUIT, sigint_handler) == SIG_ERR) ||
	    (signal(SIGTERM, sigint_handler) == SIG_ERR) ||
	    (signal(SIGPIPE, sigint_handler) == SIG_ERR)) {
		goto L_EXIT0;	
	}

	setlocale(LC_NUMERIC,"ja_JP.utf8");
	gettimeofday(&s_tvbase, 0);
	os_calibrate(&s_nsofclk, &s_clkofsec);

	buf_size = BUF_SIZE;
	s_buf = (void *)malloc_(buf_size, node_alloc, 0);
	d_buf = (void *)malloc_(buf_size+0x100, node_alloc, 0) + 0x100;
	//	printf("s_buf %'lx, d_buf %'lx\n", s_buf, d_buf);
	if (s_buf == NULL || d_buf == NULL) {
	  printf("Failed to create buffer.\n");
	  goto L_EXIT1;
	}
	
	s_lbuf = (void *)malloc(buf_size);
	d_lbuf = (void *)malloc(buf_size+0x100) + 0x100;
	setlocale(LC_NUMERIC,"ja_JP.utf8");
	printf("malloc size %'llu(%'llx), address %llx \n", buf_size,buf_size, s_lbuf);
	printf("malloc size %'llu(%'llx), address %llx \n", buf_size,buf_size, d_lbuf);
	//	printf("s_lbuf %'lx, d_lbuf %'lx\n", s_lbuf, d_lbuf);

	if (s_lbuf == NULL || d_lbuf == NULL) {
	  printf("Failed to create buffer.\n");
	  goto L_EXIT1;
	}

	cpu = sched_getcpu();
	node = numa_node_of_cpu(cpu);
	printf("processor_bind, cpu %d, node %d\n", cpu, node);

	if (optvalue[5] == 2) { // Latency only skip bandwidth measurements
	  printf("latency only, optvalue[5]:%d\n", optvalue[5]);
	  goto L_LATENCY;
	}

// threading

	rand_array_size = r_array_size[0]; // for initialization

	printf("Number of Thread %02d\n", num_thread);

	for (i = 0; i < num_thread; i++){
	  if (pthread_create(&pt[i], NULL, buf_persistent_create_read, (void *)i) != 0) {
	    printf("failed to create thread \n");
	    goto L_EXIT1;
	  }
        }

	//printf("buf_persistent_create_read ...\n");
	//buf_persistent_create();
	//printf("buf_persistent_create_read done\n");

	for (i = 0; i < num_thread; i++){
	  if (pthread_join(pt[i], NULL) != 0) {
	    printf("failed to join thread \n");
	    goto L_EXIT1;
	  }
        }

	time(&tt);
	localtime_r(&tt,&ti);
	strftime(dt,sizeof(dt),"%Y-%m-%d %H:%M:%S",&ti);
	printf("%s\n",dt);

	printf("BW_RS_BASIC_OPS_TASK_COUNT %d \n", get_no_of_rs_tasks());

	//      12345678901234567890123456789012345678901234567890123456789012345678901234567890
	printf("                                pattern;   dram (mb/s);    nvm (mb/s)  \n");

	for (k = 0; k < get_no_of_rs_tasks() ; k++){  // BW_RS_BASIC_OPS_TASK_COUNT == 81
	  //printf("[%d]tasks_str  thread start...\n", k);
	  for (i = 0; i < num_thread; i++){
	    bw_func[i] = k;
	    if (pthread_create(&pt[i], NULL, dependent_read_bw_th, (void *)i) != 0) {
	      printf("failed to create thread \n");
	      goto L_EXIT1;
	    }
	  }

	  bw_number_sum = 0;
	  bw_nvmnumber_sum = 0;

	  for (i = 0; i < num_thread; i++){
	    if (pthread_join(pt[i], NULL) != 0) {
	      printf("failed to join thread \n");
	      goto L_EXIT1;
	    }
	    bw_number_sum += bw_number[i];
	    bw_nvmnumber_sum += bw_nvmnumber[i];
	  }
	  printf("[%02d]%35s; ", k, get_funcname_str(k));
	  printf("%'13.1f; %'13.1f\n", bw_number_sum, bw_nvmnumber_sum);
	}

	  for (i = 0; i < num_thread; i++){
	    nvm_release(i);
	    }
	
	printf("pthread exited\n");

L_LATENCY:

/*
// threading
*/

/* Latency */

	if (optvalue[5] == 1) { // skip latency 
	  goto L_EXIT1;
	};

	for (i = 0; i < sizeof(r_array_size)/sizeof(r_array_size[0]); i++) {
	  rand_array_size = r_array_size[i];
	  printf("RAND_ARRAY_SIZE %d \n", rand_array_size);

	  time(&tt);
	  localtime_r(&tt,&ti);
	  strftime(dt,sizeof(dt),"%Y-%m-%d %H:%M:%S",&ti);
	  printf("%s\n",dt);

	  if ((s_buf = buf_create(node_alloc, rand_array_size)) == NULL) {
	    printf("Failed to create buffer.\n");
	    goto L_EXIT0;
	  }

	  if (dependent_read(s_buf, cpu_consumer, node_alloc, meas_sec) != 0) {
		printf("Failed to dependent read.\n");
		goto L_EXIT0;
	  }

	  //************
	  //goto L_EXIT0;

	  if (nvm == 1) {
	        printf("nvm mode.\n");
		time(&tt);
		localtime_r(&tt,&ti);
		strftime(dt,sizeof(dt),"%Y-%m-%d %H:%M:%S",&ti);
		printf("%s\n",dt);

		if ((s_buf = buf_persistent_create(node_alloc, is_pmem, mapped_len, path, rand_array_size)) == NULL) {
		        printf("Failed to create persistent buffer.\n");
			goto L_EXIT0;
		}

		cpu = sched_getcpu();
		node = numa_node_of_cpu(cpu);
		printf("processor_bind, cpu %d, node %d\n", cpu, node);

		printf("nvm read latency. \n");
		if (dependent_read(s_buf, cpu_consumer, node_alloc, meas_sec) != 0) {
		        printf("Failed to dependent read.\n");
		       goto L_EXIT0;
		}

		pmem_unmap(s_buf, mapped_len);
		// Delete the file used to store the persistent data on the persistent memory
		remove(path);

		time(&tt);
		localtime_r(&tt,&ti);
		strftime(dt,sizeof(dt),"%Y-%m-%d %H:%M:%S",&ti);
		printf("%s\n",dt);

		if (persistent_read(s_buf, cpu_consumer, node_alloc, meas_sec) !=0) {
         		printf("Failed to persistent read.\n");
			goto L_EXIT0;
		}
	  }

	ret = 0;

L_EXIT0:
	if (s_buf != NULL) {
	  buf_release(s_buf, BUF_SIZE);
	}
	if (d_buf != NULL) {
	  buf_release(d_buf, BUF_SIZE);
	}
	}
L_EXIT1:
	if (s_buf != NULL) {
	  buf_release(s_buf, BUF_SIZE);
	}
	if (d_buf != NULL) {
	  buf_release(d_buf, BUF_SIZE);
	}
	if (s_lbuf != NULL) {
	  buf_release(s_lbuf, BUF_SIZE);
	}
	if (d_lbuf != NULL) {
	  buf_release(d_lbuf, BUF_SIZE);
	}
	return (ret);
}

static void *
malloc_ (size_t buf_size, int node_alloc, int buf_class)
{
        void *buf; 

	buf = (void *)numa_alloc_local(buf_size);
	setlocale(LC_NUMERIC,"ja_JP.utf8");
	printf("numa malloc size %'llu(%'llx), address %llx \n", buf_size,buf_size, buf);

	return (buf);
}

static int
write_bw(void * s_buf, void * d_buf, size_t buf_size)
{
  //	char * tmp;
        long * tmp;
	long * s_tmp;
	long * d_tmp;
	long i, j;
	int repeat_c = 10;
	long long bw, diff;

	setlocale(LC_NUMERIC,"ja_JP.utf8");
	gettimeofday(&s_tvbase, 0);
	for (i = 0; i < repeat_c; i++) {
	  tmp = s_buf;
	  for (j = 0; j < buf_size/sizeof(long); j++) {
	    *tmp++ = 0xCAFEBABE;
	  };
	  //  printf("*");
        };
	gettimeofday(&e_tvbase, 0);
	//printf("\n time %ld, %ld \n ", e_tvbase.tv_sec, e_tvbase.tv_usec);
	//printf("time %ld, %ld \n ", s_tvbase.tv_sec, s_tvbase.tv_usec);

	diff = (e_tvbase.tv_sec - s_tvbase.tv_sec)*1000*1000;
	//	printf("time diff %ld, %ld \n ", diff, (e_tvbase.tv_usec - s_tvbase.tv_usec)); 
	//	printf("buf_size, %ld, buf_size/sizeof(long), %ld \n", buf_size, buf_size/sizeof(long));
	diff = diff + e_tvbase.tv_usec - s_tvbase.tv_usec;
	bw = buf_size * repeat_c/ diff * 1000;
	printf("store address %lx\n", s_buf);
	printf("Bandwidth store %'d KB/sec, time %'d[us]\n", bw, diff);

	gettimeofday(&s_tvbase, 0);
	for (i = 0; i < repeat_c; i++) {
	  tmp = s_buf;
	  s_tmp = s_buf;
	  for (j = 0; j < buf_size/sizeof(long); j++) {
	    st = *tmp++;
	  };
        };
	gettimeofday(&e_tvbase, 0);
	//printf("\n time %ld, %ld \n ", e_tvbase.tv_sec, e_tvbase.tv_usec);
	//printf("time %ld, %ld \n ", s_tvbase.tv_sec, s_tvbase.tv_usec);
	printf("load address %lx, %lx \n", &st, st);
	diff = (e_tvbase.tv_sec - s_tvbase.tv_sec)*1000*1000;
	//	printf("time diff %ld, %ld \n ", diff, (e_tvbase.tv_usec - s_tvbase.tv_usec)); 
	//	printf("buf_size, %ld, buf_size/sizeof(long), %ld \n", buf_size, buf_size/sizeof(long));
	diff = diff + e_tvbase.tv_usec - s_tvbase.tv_usec;
	bw = buf_size * repeat_c/ diff * 1000;
	printf("Bandwidth load %'d KB/sec, time %'d[us]\n", bw, diff);
	
	printf("buf_move from %llx to %llx \n", s_buf, d_buf);
	gettimeofday(&s_tvbase, 0);
	for (i = 0; i < repeat_c; i++) {
	  s_tmp = s_buf;
	  d_tmp = d_buf;
	  buf_move(s_tmp, d_tmp, buf_size/sizeof(long));
	};
	gettimeofday(&e_tvbase, 0);
	//printf("time %ld, %ld \n ", e_tvbase.tv_sec, e_tvbase.tv_usec);
	//printf("time %ld, %ld \n ", s_tvbase.tv_sec, s_tvbase.tv_usec);

	diff = (e_tvbase.tv_sec - s_tvbase.tv_sec)*1000*1000;
	//	printf("time diff %ld, %ld \n ", diff, (e_tvbase.tv_usec - s_tvbase.tv_usec)); 
	//	printf("buf_size, %ld, buf_size/sizeof(long), %ld \n", buf_size, buf_size/sizeof(long));
	diff = diff + e_tvbase.tv_usec - s_tvbase.tv_usec;
	bw = buf_size * repeat_c/ diff * 1000;
	printf("Bandwidth move %'d KB/sec, time %'d[us]\n", bw, diff);

	return(0);

}
static int
last_free_elem(int rand_array_size, int *s_rand_arr)
{
	int i, cnt = 0;
	
	for (i = 0; i < rand_array_size; i++) {
	  if (*(s_rand_arr+i) == INVALID_RAND) {
			cnt++;
			if (cnt > 1) {
				return (0);
			}	
		}
	}
	
	if (cnt == 1) {
		return (1);
	}
	
	return (0);	
}

static void
rand_array_init(int rand_array_size, int *s_rand_arr)
{
	int i, r, index = 0;

	printf("rand_array_init, rand_array_size %'u, s_rand_arr %'u\n", rand_array_size, s_rand_arr);

	if (s_randseed == 0) {
		s_randseed = time(0);
	}

	srand(s_randseed);
	
	for (i = 0; i < rand_array_size; i++) {
	         *(s_rand_arr+i) = INVALID_RAND;
	}
	
	while (1) {
		for (;;) {
			r = rand() % rand_array_size;
			if (*(s_rand_arr+r) == INVALID_RAND) {
				break;
			}
		}
		
		if ((*(s_rand_arr+index) == INVALID_RAND) &&
		    (index != r)) {
		        *(s_rand_arr+index) = r;
			index = r;
		}

		if (last_free_elem(rand_array_size, s_rand_arr)) {
			*(s_rand_arr+index) = rand_array_size;
			break;
		}		
	}		
}

static void
rand_buf_init(void *buf, uint64_t size, int rand_array_size, int *s_rand_arr)
{
	int nblk = size / (rand_array_size * BUF_ELE_SIZE);
	int i, j;
	uint64_t **p, **blk_start, **end = NULL;

	printf("number of block %d, buffer size %llx \n", nblk, size);
	p = (uint64_t **)buf;
	for (i = 0; i < nblk; i++) {
		blk_start = p;
		for (j = 0; j < rand_array_size; j++) {
			if (s_rand_arr[j] == rand_array_size) {
				end = p;
			}

			*p = (uint64_t *)((char *)blk_start + (s_rand_arr[j] * BUF_ELE_SIZE));
			p = (uint64_t **)((char *)p + BUF_ELE_SIZE);
		}
	}
	
	if (end != NULL) {
		*end = (uint64_t *)buf;	
	}
}

static void
buf_init(void *buf, uint64_t size, int rand_array_size, int *s_rand_arr)
{
        rand_array_init(rand_array_size, s_rand_arr);
	rand_buf_init(buf, size, rand_array_size, s_rand_arr);
}

static void *
buf_create(int node_alloc, int rand_array_size)
{
	void *buf;

	buf = numa_alloc_onnode(BUF_SIZE, node_alloc);
	if (buf != NULL)
	  buf_init(buf, BUF_SIZE, rand_array_size, (int *)&s_rand_arr);

	return (buf);
}

static void *
buf_persistent_create(int node_alloc, int is_pmem, size_t mapped_len, char *path, int rand_array_size)
{
	void *buf;

	strcpy(path,"/mnt/pmem_fsdax");
	sprintf(path+strlen(path), "%d", node_alloc);
	sprintf(path+strlen(path), "/");

	// The path+strlen(path) part of the sprintf call below writes the data after the end of the current string
	sprintf(path+strlen(path), "pstream_test_file");

	// Add the rank number onto the file name to ensure we have unique files for each MPI rank participating in the exercise.
	sprintf(path+strlen(path), "%d", node_alloc);

	if ((buf = pmem_map_file(path, BUF_SIZE,
			PMEM_FILE_CREATE,
			0666, &mapped_len, &is_pmem)) == NULL) {
		perror("pmem_map_file");
		fprintf(stderr, "Failed to pmem_map_file for filename: %s\n", path);
		exit(-100);
	}

	if (buf != NULL && rand_array_size != 0)
	  buf_init(buf, BUF_SIZE, rand_array_size, (int *)&s_rand_arr);

	printf("Using file %s for pmem, address %llx\n",path, buf);

	return (buf);
}

static void *
buf_persistent_create_read(void *arg)
{
        long n = (long)arg;
	char path[MAX_FILE_NAME_LENGTH];
	char nvmpath[MAX_FILE_NAME_LENGTH];
	long buf_size = BUF_SIZE;
	int* s_rand_arr; //[RAND_ARRAY_SIZE*1024L]
	FILE *fp;
	int is_pmem;
	size_t mapped_len;

	printf("buf_persistent_create_read %02d\n", n);
	strcpy(path, "/tmp/pmemlog");
	sprintf(path+strlen(path), "%02d", n);
	fp = fopen(path, "w");
	s_rand_arr = (int *)malloc_(RAND_ARRAY_SIZE*1024L, 0, 0);

	if(fp==NULL){
	  printf("fopen error thread %02d\n", n);
	  pthread_exit(NULL);
	}
	fprintf(fp, "pmemlog is created; %s, thread %02d\n", path, n);

	s_bufth[n] = (void *)malloc_(buf_size, 0, 0);
	d_bufth[n] = (void *)malloc_(buf_size+0x100, 0, 0) + 0x100;
	fprintf(fp, "thread %02d, numa malloc size s_bufth%'llu(%'llx), address %llx \n", (int)n, buf_size,buf_size, s_bufth[n]);
	fprintf(fp, "thread %02d, numa malloc size d_bufth%'llu(%'llx), address %llx \n", (int)n, (void *)(buf_size+0x100),(void *)(buf_size+0x100), d_bufth[n]);

	rand_array_init(rand_array_size, s_rand_arr);
	rand_buf_init(s_bufth[n], buf_size, rand_array_size, s_rand_arr);

	fprintf(fp, "thread %02d, buf_init\n", (int)n);

	strcpy(path,"/mnt/pmem_fsdax0/");
	sprintf(path+strlen(path), "nvmtest%02d", (int)n);
	if ((s_nvmbufth[n] = pmem_map_file(path, buf_size,
			PMEM_FILE_CREATE,
			0666, &mapped_len, &is_pmem)) == NULL) {
		perror("pmem_map_file");
		fprintf(stderr, "Failed to pmem_map_file for filename: %s\n", path);
		exit(-100);
	}

	printf("thread %02d,nvm malloc size s_nvmbufth %'llu(%'llx), address %llx \n", (int)n, buf_size,buf_size, s_nvmbufth[n]);

	if (s_nvmbufth[n] != NULL && rand_array_size != 0)
	  rand_buf_init(s_nvmbufth[n], buf_size, rand_array_size, s_rand_arr);
	  

	fprintf(fp, "thread %02d, nvmbuf_init\n", (int)n);

	fprintf(fp, "thread %02d, throughput\n", (int)n);

	fclose(fp);
	pthread_exit(NULL);

}

static int
nvm_release(int n)
{
	char path[MAX_FILE_NAME_LENGTH];
	long buf_size = BUF_SIZE;

	pmem_unmap(s_nvmbufth[n], buf_size);
	strcpy(path,"/mnt/pmem_fsdax0/");
	sprintf(path+strlen(path), "nvmtest%02d", (int)n);
	remove(path);
	printf("removing %s\n", path);
}

static void *
dependent_read_bw_th(void *arg)
{
        long n = (long)arg;
	char path[MAX_FILE_NAME_LENGTH];
	long buf_size = BUF_SIZE;
	int* s_rand_arr; //[RAND_ARRAY_SIZE*1024L]
	FILE *fp;
	double bw;

	//	printf("dependent_read_bw_th %02d\n", n);
	strcpy(path, "/tmp/pmemlog");
	sprintf(path+strlen(path), "%02d", n);
	fp = fopen(path, "a");
	fprintf(fp, "pmemlog is opened; %s, thread %02d\n", path, n);

	//	printf("arch__dependent_bw_read_th, address %llx, nvm address %llx\n", s_bufth[n], s_nvmbufth[n]);
	arch__dependent_bw_read_th(s_bufth[n], 1, bw_func[n], fp, &bw);
	bw_number[n] = bw;
	fprintf(fp, "nvm:");
	arch__dependent_bw_read_th(s_nvmbufth[n], 1, bw_func[n], fp, &bw);
	bw_nvmnumber[n] = bw;
	//	printf("BW %'13.1f MB/s; nvm %'13.1f MB/s\n", bw_number[n], bw_nvmnumber[n]);

	fprintf(fp, "thread %02d, arch__dependent_bw_read_th\n", (int)n);

	fclose(fp);
	pthread_exit(NULL);

}

static void
buf_release(void *buf, size_t buf_size)
{
	numa_free(buf, buf_size);	
}

static int
dependent_read(void *buf, int cpu_consumer, int node_alloc, int meas_sec)
{
        int i;

	if (processor_bind(cpu_consumer) != 0) {
		return (-1);
	}

	fprintf(stdout, "\n!!! The reported latency is not the official data\n");
	fprintf(stdout, "    from " CORP ", it's just a tool to test numatop !!!\n");

	fprintf(stdout, "\nGenerating memory access from cpu %d to node %d for ~%ds ...\n",
	    cpu_consumer, node_alloc, meas_sec);

	fprintf(stdout, "(random seed to build random address array is %u.)\n", s_randseed);

	if (optvalue[5] == 0 || optvalue[5] == 2) {
	  printf("dependent_read, address %llx\n", buf);
	  arch__dependent_read(buf, meas_sec);
	  arch__dependent_bw_read(buf, meas_sec, (FILE *)stdout);
	}

	//	/*
	fprintf(stdout, "\n-------------------------------------------------\n");
	fprintf(stdout, "\n!!! The reported latency is not the official data\n");
	printf("arch__dependent_write, address %llx\n", buf);
	arch__dependent_write(buf, meas_sec);
	//	*/

	return (0);
}

static int
dependent_write(void *buf, int cpu_consumer, int node_alloc, int meas_sec)
{
	if (processor_bind(cpu_consumer) != 0) {
		return (-1);
	}

	fprintf(stdout, "\n---NVM-------------------------------------------\n");
	fprintf(stdout, "\n!!! The reported latency is not the official data\n");
	fprintf(stdout, "    from " CORP ", it's just a tool to test numatop !!!\n");

	fprintf(stdout, "\nGenerating memory access from cpu%d to node%d for ~%ds ...\n",
	    cpu_consumer, node_alloc, meas_sec);

	// fprintf(stdout, "(random seed to build random address array is %u.)\n", s_randseed);

	// arch__dependent_read(buf, meas_sec);
	arch__dependent_write(buf, meas_sec);

	return (0);
}

// ---------------------------------------------------------------

static int
persistent_read(void *buf, int cpu_consumer, int node_alloc, int meas_sec)
{
	initialise_benchmark_results(&b_results);

	world_comm.size = 1;
	world_comm.rank = ROOT;
	node_comm.size = 1;
	node_comm.rank = ROOT;
	root_comm.size = 1;
	root_comm.rank = ROOT;

	stream_persistent_memory_task(&b_results, world_comm, node_comm, &array_size, socket, collective);
	collect_results(b_results, &a_results, &node_results, world_comm, node_comm, root_comm);

	print_results(a_results, node_results, world_comm, array_size, node_comm);

	free_benchmark_results(&b_results);
		
	return(0);

}

// Initialise the benchmark results structure to enable proper collection of data
void initialise_benchmark_results(benchmark_results *b_results){

	int name_length;

	b_results->Copy.avg = 0;
	b_results->Copy.min = FLT_MAX;
	b_results->Copy.max= 0;
	b_results->Copy.raw_result = malloc(NTIMES * sizeof(double));
	b_results->Scale.avg = 0;
	b_results->Scale.min = FLT_MAX;
	b_results->Scale.max= 0;
	b_results->Scale.raw_result = malloc(NTIMES * sizeof(double));
	b_results->Add.avg = 0;
	b_results->Add.min = FLT_MAX;
	b_results->Add.max= 0;
	b_results->Add.raw_result = malloc(NTIMES * sizeof(double));
	b_results->Triad.avg = 0;
	b_results->Triad.min = FLT_MAX;
	b_results->Triad.max= 0;
	b_results->Triad.raw_result = malloc(NTIMES * sizeof(double));

	/* MPI_Get_processor_name(b_results->name, &name_length); */

}

// Initialise the benchmark results structure to enable proper collection of data
void free_benchmark_results(benchmark_results *b_results){

	free(b_results->Copy.raw_result);
	free(b_results->Scale.raw_result);
	free(b_results->Add.raw_result);
	free(b_results->Triad.raw_result);

}

// Print out aggregate results. The intention is that this will only
// be called from the root process as the overall design is that
// only the root process (the process which has ROOT rank) will
// have this data.
void print_results(aggregate_results a_results, aggregate_results node_results, communicator world_comm, int array_size, communicator node_comm){

	int omp_thread_num;
	double bandwidth_avg, bandwidth_max, bandwidth_min;
	double copy_size = 2 * sizeof(STREAM_TYPE) * array_size;
	double scale_size = 2 * sizeof(STREAM_TYPE) * array_size;
	double add_size	= 3 * sizeof(STREAM_TYPE) * array_size;
	double triad_size = 3 * sizeof(STREAM_TYPE) * array_size;


	printf("Running with %d MPI processes, each with %d OpenMP threads. %d processes per node\n", world_comm.size, omp_thread_num, node_comm.size);
	printf("Benchmark     Avg Bandwidth      Avg Time    Max Bandwidth   Min Time    Min Bandwidth   Max Time   Max Time Location\n");
	printf("                  (MB/s)         (seconds)       (MB/s)      (seconds)       (MB/s)      (seconds)      (proc name)\n");
	printf("----------------------------------------------------------------------------------------------------------------------\n");

	// Calculate the bandwidths. Max bandwidth is achieved using the min time (i.e. the fast time). This is
	// why max and min are opposite either side of the "=" below
	bandwidth_avg = (1.0E-06 * copy_size)/a_results.Copy.avg;
	bandwidth_max = (1.0E-06 * copy_size)/a_results.Copy.min;
	bandwidth_min = (1.0E-06 * copy_size)/a_results.Copy.max;
	printf("Copy:     %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f   %s\n", bandwidth_avg, a_results.Copy.avg, bandwidth_max, a_results.Copy.min, bandwidth_min, a_results.Copy.max, a_results.copy_max);

	// Calculate the bandwidths. Max bandwidth is achieved using the min time (i.e. the fast time). This is
	// why max and min are opposite either side of the "=" below
	bandwidth_avg = (1.0E-06 * scale_size)/a_results.Scale.avg;
	bandwidth_max = (1.0E-06 * scale_size)/a_results.Scale.min;
	bandwidth_min = (1.0E-06 * scale_size)/a_results.Scale.max;
	printf("Scale:    %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f   %s\n", bandwidth_avg, a_results.Scale.avg, bandwidth_max, a_results.Scale.min, bandwidth_min, a_results.Scale.max, a_results.scale_max);

	// Calculate the bandwidths. Max bandwidth is achieved using the min time (i.e. the fast time). This is
	// why max and min are opposite either side of the "=" below
	bandwidth_avg = (1.0E-06 * add_size)/a_results.Add.avg;
	bandwidth_max = (1.0E-06 * add_size)/a_results.Add.min;
	bandwidth_min = (1.0E-06 * add_size)/a_results.Add.max;
	printf("Add:      %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f   %s\n", bandwidth_avg, a_results.Add.avg, bandwidth_max, a_results.Add.min, bandwidth_min, a_results.Add.max, a_results.add_max);

	// Calculate the bandwidths. Max bandwidth is achieved using the min time (i.e. the fast time). This is
	// why max and min are opposite either side of the "=" below
	bandwidth_avg = (1.0E-06 * triad_size)/a_results.Triad.avg;
	bandwidth_max = (1.0E-06 * triad_size)/a_results.Triad.min;
	bandwidth_min = (1.0E-06 * triad_size)/a_results.Triad.max;
	printf("Triad:    %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f   %s\n", bandwidth_avg, a_results.Triad.avg, bandwidth_max, a_results.Triad.min, bandwidth_min, a_results.Triad.max, a_results.triad_max);

	// Calculate the node bandwidths.
	bandwidth_avg = ((1.0E-06 * copy_size * node_comm.size)/node_results.Copy.avg);
	bandwidth_max = ((1.0E-06 * copy_size * node_comm.size)/node_results.Copy.min);
	bandwidth_min = ((1.0E-06 * copy_size * node_comm.size)/node_results.Copy.max);
	printf("Node Copy:  %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f\n", bandwidth_avg, node_results.Copy.avg, bandwidth_max, node_results.Copy.min, bandwidth_min, node_results.Copy.max);

	// Calculate the node bandwidths.
	bandwidth_avg = ((1.0E-06 * scale_size * node_comm.size)/node_results.Scale.avg);
	bandwidth_max = ((1.0E-06 * scale_size * node_comm.size)/node_results.Scale.min);
	bandwidth_min = ((1.0E-06 * scale_size * node_comm.size)/node_results.Scale.max);
	printf("Node Scale: %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f\n", bandwidth_avg, node_results.Scale.avg, bandwidth_max, node_results.Scale.min, bandwidth_min, node_results.Scale.max);

	// Calculate the node bandwidths.
	bandwidth_avg = ((1.0E-06 * add_size * node_comm.size)/node_results.Add.avg);
	bandwidth_max = ((1.0E-06 * add_size * node_comm.size)/node_results.Add.min);
	bandwidth_min = ((1.0E-06 * add_size * node_comm.size)/node_results.Add.max);
	printf("Node Add:   %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f\n", bandwidth_avg, node_results.Add.avg, bandwidth_max, node_results.Add.min, bandwidth_min, node_results.Add.max);

	// Calculate the node bandwidths.
	bandwidth_avg = ((1.0E-06 * triad_size * node_comm.size)/node_results.Triad.avg);
	bandwidth_max = ((1.0E-06 * triad_size * node_comm.size)/node_results.Triad.min);
	bandwidth_min = ((1.0E-06 * triad_size * node_comm.size)/node_results.Triad.max);
	printf("Node Triad: %12.1f:   %11.6f:  %12.1f:   %11.6f:   %12.1f:   %11.6f\n", bandwidth_avg, node_results.Triad.avg, bandwidth_max, node_results.Triad.min, bandwidth_min, node_results.Triad.max);
}

int stream_persistent_memory_task(benchmark_results *b_results, communicator world_comm, communicator node_comm, int *array_size, int socket, persist_state persist_level){
	char path[MAX_FILE_NAME_LENGTH];
	char *pmemaddr = NULL;
	int array_element_size;
	int is_pmem;
	size_t mapped_len;
	int	quantum;
	int	BytesPerWord;
	int	k;
	ssize_t	j;
	STREAM_TYPE scalar;
	long long array_length;
	double t, times[4][NTIMES];

	*array_size = (LAST_LEVEL_CACHE_SIZE*4)/node_comm.size;

	/* --- SETUP --- determine precision and check timing --- */

	//printf("STREAM version $Revision: 5.10 $\n");
	BytesPerWord = sizeof(STREAM_TYPE);

		printf("This system uses %d bytes per array element.\n",BytesPerWord);

		printf("Array size = %llu (elements), Offset = %d (elements)\n" , (unsigned long long) *array_size, OFFSET);
		printf("Memory per array = %.1f MiB (= %.1f GiB).\n",
				BytesPerWord * ( (double) *array_size / 1024.0/1024.0),
				BytesPerWord * ( (double) *array_size / 1024.0/1024.0/1024.0));
		printf("Total memory required per process = %.1f MiB (= %.1f GiB).\n",
				(3.0 * BytesPerWord) * ( (double) *array_size / 1024.0/1024.),
				(3.0 * BytesPerWord) * ( (double) *array_size / 1024.0/1024./1024.));
		printf("Total memory required per node = %.1f MiB (= %.1f GiB).\n",
				node_comm.size * (3.0 * BytesPerWord) * ( (double) *array_size / 1024.0/1024.),
				node_comm.size * (3.0 * BytesPerWord) * ( (double) *array_size / 1024.0/1024./1024.));
		printf("Each kernel will be executed %d times.\n", NTIMES);
		printf("The first iteration is excluded from reported results\n");

		printf("Stream Persistent Memory Task\n");


	strcpy(path,"/mnt/pmem_fsdax");
	sprintf(path+strlen(path), "%d", (socket+1) % 2);
	sprintf(path+strlen(path), "/");

	// The path+strlen(path) part of the sprintf call below writes the data after the end of the current string
	sprintf(path+strlen(path), "pstream_test_file");

		printf("Using file %s[rank] for pmem\n",path);

	// Add the rank number onto the file name to ensure we have unique files for each MPI rank participating in the exercise.
	sprintf(path+strlen(path), "%d", world_comm.rank);

	printf("Using file %s for pmem\n",path);

	// Calculate the size of the file/persistent memory area to create.
	// This needs to be done as a separate variable to stop issues with integer
	// overflow affecting large memory area size requests.
	array_length = (*array_size+OFFSET);
	array_length = array_length*BytesPerWord*3;

	if ((pmemaddr = pmem_map_file(path, array_length,
			PMEM_FILE_CREATE,
			0666, &mapped_len, &is_pmem)) == NULL) {
		perror("pmem_map_file");
		fprintf(stderr, "Failed to pmem_map_file for filename: %s\n", path);
		exit(-100);
	}

		if(persist_level == none){
			printf("Not persisting data.\n");
		}else if(persist_level == individual){
			printf("Persisting individual writes\n");
		}else if(persist_level == collective){
			printf("Persisting writes at the end of each benchmark iteration\n");
		}else{
			printf("No persist option specified, this is likely a mistake\n");
		}

		a = (STREAM_TYPE *) pmemaddr;
		b = (STREAM_TYPE *)(pmemaddr + (*array_size+OFFSET)*BytesPerWord);
		c = (STREAM_TYPE *)(pmemaddr + (*array_size+OFFSET)*BytesPerWord*2);

	/* Get initial value for system clock. */
	for (j=0; j<*array_size; j++) {
		a[j] = 1.0;
		b[j] = 2.0;
		c[j] = 0.0;
	}

	if  ( (quantum = checktick()) >= 1)
		printf("Your clock granularity/precision appears to be "
				"%d microseconds.\n", quantum);
	else {
		printf("Your clock granularity appears to be "
				"less than one microsecond.\n");
		quantum = 1;
	}

	t = mysecond();
	for (j = 0; j < *array_size; j++)
		a[j] = 2.0E0 * a[j];
	t = 1.0E6 * (mysecond() - t);

	printf("Each test below will take on the order"
			" of %d microseconds.\n", (int) t  );
	printf("   (= %d clock ticks)\n", (int) (t/quantum) );
	printf("Increase the size of the arrays if this shows that\n");
	printf("you are not getting at least 20 clock ticks per test.\n");

	printf("WARNING -- The above is only a rough guideline.\n");
	printf("For best results, please be sure you know the\n");
	printf("precision of your system timer.\n");

	/*	--- MAIN LOOP --- repeat test cases NTIMES times --- */

	scalar = 3.0;
	for (k=0; k<NTIMES; k++)
	{
		// Add in a barrier synchronisation to ensure all processes on a node are undertaking the
		// benchmark at the same time. This ensures the node level results are fair as all
		// operations are synchronised on the node.

		times[0][k] = mysecond();
		if(persist_level == individual){
			for (j=0; j<*array_size; j++){
				c[j] = a[j];
				pmem_persist(&c[j], BytesPerWord);
			}
		}else if(persist_level == collective){
			for (j=0; j<*array_size; j++){
				c[j] = a[j];
			}
			pmem_persist(c, *array_size*BytesPerWord);
		}else{
			for (j=0; j<*array_size; j++){
				c[j] = a[j];
			}
		}
		times[0][k] = mysecond() - times[0][k];
		b_results->Copy.raw_result[k] = times[0][k];

		times[1][k] = mysecond();
		if(persist_level == individual){
			for (j=0; j<*array_size; j++){
				b[j] = scalar*c[j];
				pmem_persist(&b[j], BytesPerWord);
			}
		}
		else if(persist_level == collective){
			for (j=0; j<*array_size; j++){
				b[j] = scalar*c[j];
			}
			pmem_persist(b, *array_size*BytesPerWord);
		}else{
			for (j=0; j<*array_size; j++){
				b[j] = scalar*c[j];
			}
		}
		times[1][k] = mysecond() - times[1][k];
		b_results->Scale.raw_result[k] = times[1][k];

		times[2][k] = mysecond();
		if(persist_level == individual){
			for (j=0; j<*array_size; j++){
				c[j] = a[j]+b[j];
				pmem_persist(&c[j], BytesPerWord);
			}
		}else if(persist_level == collective){
			for (j=0; j<*array_size; j++){
				c[j] = a[j]+b[j];
			}
			pmem_persist(c, *array_size*BytesPerWord);
		}else{
			for (j=0; j<*array_size; j++){
				c[j] = a[j]+b[j];
			}
		}
		times[2][k] = mysecond() - times[2][k];
		b_results->Add.raw_result[k] = times[2][k];

		times[3][k] = mysecond();
		if(persist_level == individual){
			for (j=0; j<*array_size; j++){
				a[j] = b[j]+scalar*c[j];
				pmem_persist(&a[j], BytesPerWord);
			}
		}else if(persist_level == collective){
			for (j=0; j<*array_size; j++){
				a[j] = b[j]+scalar*c[j];
			}
			pmem_persist(a, *array_size*BytesPerWord);
		}else{
			for (j=0; j<*array_size; j++){
				a[j] = b[j]+scalar*c[j];
			}
		}
		times[3][k] = mysecond() - times[3][k];
		b_results->Triad.raw_result[k] = times[3][k];

	}

	/*	--- SUMMARY --- */
	/* note -- skip first iteration */
	for (k=1; k<NTIMES; k++) {
		b_results->Copy.avg = b_results->Copy.avg + times[0][k];
		b_results->Copy.min = MIN(b_results->Copy.min, times[0][k]);
		b_results->Copy.max = MAX(b_results->Copy.max, times[0][k]);
		b_results->Scale.avg = b_results->Scale.avg + times[1][k];
		b_results->Scale.min = MIN(b_results->Scale.min, times[1][k]);
		b_results->Scale.max = MAX(b_results->Scale.max, times[1][k]);
		b_results->Add.avg = b_results->Add.avg + times[2][k];
		b_results->Add.min = MIN(b_results->Add.min, times[2][k]);
		b_results->Add.max = MAX(b_results->Add.max, times[2][k]);
		b_results->Triad.avg = b_results->Triad.avg + times[3][k];
		b_results->Triad.min = MIN(b_results->Triad.min, times[3][k]);
		b_results->Triad.max = MAX(b_results->Triad.max, times[3][k]);
	}

	b_results->Copy.avg = b_results->Copy.avg/(double)(NTIMES-1);
	b_results->Scale.avg = b_results->Scale.avg/(double)(NTIMES-1);
	b_results->Add.avg = b_results->Add.avg/(double)(NTIMES-1);
	b_results->Triad.avg = b_results->Triad.avg/(double)(NTIMES-1);


	/* --- Check Results --- */
	checkSTREAMresults(*array_size);

	pmem_unmap(pmemaddr, mapped_len);
	// Delete the file used to store the persistent data on the persistent memory
	remove(path);

	return 0;
}

# define	M	20

static int checktick(){
	int		i, minDelta, Delta;
	double	t1, t2, timesfound[M];

	/*  Collect a sequence of M unique time values from the system. */

	for (i = 0; i < M; i++) {
		t1 = mysecond();
		while( ((t2=mysecond()) - t1) < 1.0E-6 )
			;
		timesfound[i] = t1 = t2;
	}

	/*
	 * Determine the minimum difference between these M values.
	 * This result will be our estimate (in microseconds) for the
	 * clock granularity.
	 */

	minDelta = 1000000;
	for (i = 1; i < M; i++) {
		Delta = (int)( 1.0E6 * (timesfound[i]-timesfound[i-1]));
		minDelta = MIN(minDelta, MAX(Delta,0));
	}

	return(minDelta);
}

/* A gettimeofday routine to give access to the wall
   clock timer on most UNIX-like systems.  */
static double mysecond(){
	struct timeval tp;
	struct timezone tzp;
	int i;

	i = gettimeofday(&tp,&tzp);
	return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

#ifndef abs
#define abs(a) ((a) >= 0 ? (a) : -(a))
#endif
static void checkSTREAMresults (int array_size){
	STREAM_TYPE aj,bj,cj,scalar;
	STREAM_TYPE aSumErr,bSumErr,cSumErr;
	STREAM_TYPE aAvgErr,bAvgErr,cAvgErr;
	double epsilon;
	ssize_t	j;
	int	k,ierr,err;

	/* reproduce initialization */
	aj = 1.0;
	bj = 2.0;
	cj = 0.0;
	/* a[] is modified during timing check */
	aj = 2.0E0 * aj;
	/* now execute timing loop */
	scalar = 3.0;
	for (k=0; k<NTIMES; k++)
	{
		cj = aj;
		bj = scalar*cj;
		cj = aj+bj;
		aj = bj+scalar*cj;
	}

	/* accumulate deltas between observed and expected results */
	aSumErr = 0.0;
	bSumErr = 0.0;
	cSumErr = 0.0;
	for (j=0; j<array_size; j++) {
		aSumErr += abs(a[j] - aj);
		bSumErr += abs(b[j] - bj);
		cSumErr += abs(c[j] - cj);
		// if (j == 417) printf("Index 417: c[j]: %f, cj: %f\n",c[j],cj);	// MCCALPIN
	}
	aAvgErr = aSumErr / (STREAM_TYPE) array_size;
	bAvgErr = bSumErr / (STREAM_TYPE) array_size;
	cAvgErr = cSumErr / (STREAM_TYPE) array_size;

	if (sizeof(STREAM_TYPE) == 4) {
		epsilon = 1.e-6;
	}
	else if (sizeof(STREAM_TYPE) == 8) {
		epsilon = 1.e-13;
	}
	else {
		printf("WEIRD: sizeof(STREAM_TYPE) = %lu\n",sizeof(STREAM_TYPE));
		epsilon = 1.e-6;
	}

	err = 0;
	if (abs(aAvgErr/aj) > epsilon) {
		err++;
		printf ("Failed Validation on array a[], AvgRelAbsErr > epsilon (%e)\n",epsilon);
		printf ("     Expected Value: %e, AvgAbsErr: %e, AvgRelAbsErr: %e\n",aj,aAvgErr,abs(aAvgErr)/aj);
		ierr = 0;
		for (j=0; j<array_size; j++) {
			if (abs(a[j]/aj-1.0) > epsilon) {
				ierr++;
#ifdef VERBOSE
				if (ierr < 10) {
					printf("         array a: index: %ld, expected: %e, observed: %e, relative error: %e\n",
							j,aj,a[j],abs((aj-a[j])/aAvgErr));
				}
#endif
			}
		}
		printf("     For array a[], %d errors were found.\n",ierr);
	}
	if (abs(bAvgErr/bj) > epsilon) {
		err++;
		printf ("Failed Validation on array b[], AvgRelAbsErr > epsilon (%e)\n",epsilon);
		printf ("     Expected Value: %e, AvgAbsErr: %e, AvgRelAbsErr: %e\n",bj,bAvgErr,abs(bAvgErr)/bj);
		printf ("     AvgRelAbsErr > Epsilon (%e)\n",epsilon);
		ierr = 0;
		for (j=0; j<array_size; j++) {
			if (abs(b[j]/bj-1.0) > epsilon) {
				ierr++;
#ifdef VERBOSE
				if (ierr < 10) {
					printf("         array b: index: %ld, expected: %e, observed: %e, relative error: %e\n",
							j,bj,b[j],abs((bj-b[j])/bAvgErr));
				}
#endif
			}
		}
		printf("     For array b[], %d errors were found.\n",ierr);
	}
	if (abs(cAvgErr/cj) > epsilon) {
		err++;
		printf ("Failed Validation on array c[], AvgRelAbsErr > epsilon (%e)\n",epsilon);
		printf ("     Expected Value: %e, AvgAbsErr: %e, AvgRelAbsErr: %e\n",cj,cAvgErr,abs(cAvgErr)/cj);
		printf ("     AvgRelAbsErr > Epsilon (%e)\n",epsilon);
		ierr = 0;
		for (j=0; j<array_size; j++) {
			if (abs(c[j]/cj-1.0) > epsilon) {
				ierr++;
#ifdef VERBOSE
				if (ierr < 10) {
					printf("         array c: index: %ld, expected: %e, observed: %e, relative error: %e\n",
							j,cj,c[j],abs((cj-c[j])/cAvgErr));
				}
#endif
			}
		}
		printf("     For array c[], %d errors were found.\n",ierr);
	}

#ifdef VERBOSE
	printf ("Results Validation Verbose Results: \n");
	printf ("    Expected a(1), b(1), c(1): %f %f %f \n",aj,bj,cj);
	printf ("    Observed a(1), b(1), c(1): %f %f %f \n",a[1],b[1],c[1]);
	printf ("    Rel Errors on a, b, c:     %e %e %e \n",abs(aAvgErr/aj),abs(bAvgErr/bj),abs(cAvgErr/cj));
#endif
}


void collect_results(benchmark_results b_results, aggregate_results *a_results, aggregate_results *node_results, communicator world_comm, communicator node_comm, communicator root_comm){

	collect_individual_result(b_results.Copy, &a_results->Copy, &node_results->Copy, a_results->copy_max, b_results.name, world_comm, node_comm, root_comm);
	collect_individual_result(b_results.Scale, &a_results->Scale, &node_results->Scale, a_results->scale_max,  b_results.name, world_comm, node_comm, root_comm);
	collect_individual_result(b_results.Add, &a_results->Add, &node_results->Add, a_results->add_max, b_results.name, world_comm, node_comm, root_comm);
	collect_individual_result(b_results.Triad, &a_results->Triad, &node_results->Triad, a_results->triad_max, b_results.name, world_comm, node_comm, root_comm);

}

void collect_individual_result(performance_result indivi, performance_result *result, performance_result *node_result, char *max_name, char *name, communicator world_comm, communicator node_comm, communicator root_comm){

	// Structure to hold both a value and a rank for MAXLOC and MINLOC operations.
	// This *may* be problematic on some MPI implementations as it assume MPI_DOUBLE_INT
	// matches this specification.
	typedef struct resultloc {
		double value;
		int   rank;
	} resultloc;

	double temp_value;
	double temp_result;

	double temp_store;

	int k;

	// Variable for the result of the reduction
	resultloc rloc;
	// Variable for the data to be reduced
	resultloc iloc;

	int root = ROOT;

	// https://docs.microsoft.com/en-us/message-passing-interface/mpi-status-structure
	// http://formalverification.cs.utah.edu/sawaya/html/d8/d69/mpi_8h-source.html
	typedef struct MPI_Status {
	  int count;
	  int cancelled;
	  int MPI_SOURCE;
	  int MPI_TAG;
	  int MPI_ERROR;
	} MPI_Status;
	MPI_Status status;

	/*	MPI_Reduce(&indivi.avg, &result->avg, 1, MPI_DOUBLE, MPI_SUM, root, world_comm.comm); */
	if(world_comm.rank == root){
		result->avg = indivi.avg/world_comm.size;
	}

	// Get the total avg value summed across all processes in a node to enable calculation
	// of the avg bandwidth for a node.
	temp_store = 0;
	for(k=1; k<NTIMES; k++) {
		temp_value = indivi.raw_result[k];
		/* MPI_Reduce(&temp_value, &temp_result, 1, MPI_DOUBLE, MPI_MAX, ROOT, node_comm.comm); */
		temp_store = temp_store + temp_value;
	}
	temp_store = temp_store/(NTIMES-1);
	node_result->avg = temp_store;

	if(node_comm.rank == root){
		temp_value = node_result->avg;
		/* MPI_Reduce(&temp_value, &temp_result, 1, MPI_DOUBLE, MPI_SUM, ROOT, root_comm.comm); */
		if(world_comm.rank == root){
			node_result->avg = temp_value/root_comm.size;
		}
	}

	iloc.value = indivi.max;
	iloc.rank = world_comm.rank;
	rloc.value = indivi.max;
	rloc.rank = world_comm.rank;
	/* MPI_Allreduce(&iloc, &rloc, 1, MPI_DOUBLE_INT, MPI_MAXLOC, world_comm.comm);*/
	if(rloc.rank == world_comm.rank && rloc.value != indivi.max){
		printf("Error with the output of MPI_MAXLOC reduction.\n");
	}
	result->max = rloc.value;
	// Communicate which node has the biggest max value so outlier nodes can be identified
	if(rloc.rank == world_comm.rank && rloc.rank != root){
	  /* MPI_Ssend(name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, root, 0, world_comm.comm); */
	}else if(world_comm.rank == root && rloc.rank != root){
	  /* MPI_Recv(max_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, rloc.rank, 0, world_comm.comm, &status); */
	}else if(rloc.rank == root){
		strcpy(max_name, name);
	}

	// Get the total max value summed across all processes in a node to enable calculation
	// of the minimum bandwidth for a node.
	temp_store = 0;
	for(k=1; k<NTIMES; k++) {
		temp_store = indivi.raw_result[k];
		/* MPI_Reduce(&temp_value, &temp_result, 1, MPI_DOUBLE, MPI_MAX, ROOT, node_comm.comm); */
		if(temp_result > temp_store){
			temp_store = temp_result;
		}
	}

	node_result->max = temp_store;

	// Get the total max value across all the nodes
	if(node_comm.rank == root){
		temp_result = node_result->max;
		/* MPI_Reduce(&temp_value, &temp_result, 1, MPI_DOUBLE, MPI_MAX, ROOT, root_comm.comm); */
		if(world_comm.rank == root){
			node_result->max = temp_result;
		}
	}

	iloc.value = indivi.min;
	iloc.rank = world_comm.rank;
	rloc.value = indivi.min;
	rloc.rank = world_comm.rank;
	/* MPI_Allreduce(&iloc, &rloc, 1, MPI_DOUBLE_INT, MPI_MINLOC, world_comm.comm); */
	result->min = rloc.value;

	// Get the total min value summed across all processes in a node to enable calculation
	// of the maximum bandwidth for a node.
	temp_store = FLT_MAX;
	for(k=1; k<NTIMES; k++) {
		temp_result = indivi.raw_result[k];
		/* MPI_Reduce(&temp_value, &temp_result, 1, MPI_DOUBLE, MPI_MAX, ROOT, node_comm.comm);*/
		if(temp_result < temp_store){
			temp_store = temp_result;
		}
	}
	node_result->min = temp_store;

	// Get the total min value across all the nodes
	if(node_comm.rank == root){
		temp_result = node_result->min;
		/* MPI_Reduce(&temp_value, &temp_result, 1, MPI_DOUBLE, MPI_MIN, ROOT, root_comm.comm); */
		if(world_comm.rank == root){
			node_result->min = temp_result;
		}
	}


}
