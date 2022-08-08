/*
 * Copyright (c) 2013, Intel Corporation
 * Copyright (c) 2017, IBM Corporation
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

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "../include/util.h"
#include "../../common/include/util.h"
#include "../lattester.h"
#include "../common.h"

void buf_move(void *src, void *dst, int read_num)
{
	int i;
	asm  volatile (
		"xor %3, %3\n\t"
"LOOP2:\n\t"
		"movq (%0), %%r15\n\t"
		"movq %%r15, (%2)\n\t"
		"inc %3\n\t"
		"inc %0\n\t"
		"inc %2\n\t"
		"cmp %1,%3\n\t"
		"jb LOOP2\n\t"
"STOP2:\n\t"
		:
		:"r"(src), "r"(read_num), "r" (dst), "r" (i)
	);
}

static
void buf_read(void *buf, int read_num)
{
	asm  volatile (
		"xor %0, %0\n\t"
"LOOP1:\n\t"
		"mov (%1),%1\n\t"
		"inc %0\n\t"
		"cmp %2,%0\n\t"
		"jb LOOP1\n\t"
"STOP:\n\t"
		::"b"(0), "d"(buf), "r"(read_num)
	);
}

static
void buf_read64(void *buf, int read_num)
{
	asm  volatile (
		"xor %0, %0\n\t"
"LOOP_read64:\n\t"
		"mov (%1),%1\n\t"
		"movq 8*8(%1), %%r15 \n"
		"movq 9*8(%1), %%r15 \n"
		"movq 10*8(%1), %%r15 \n"
		"movq 11*8(%1), %%r15 \n"
		"movq 12*8(%1), %%r15 \n"
		"movq 13*8(%1), %%r15 \n"
		"movq 14*8(%1), %%r15 \n"
		"movq 15*8(%1), %%r15 \n"
		"inc %0\n\t"
		"cmp %2,%0\n\t"
		"jb LOOP_read64\n\t"
"STOP_read64:\n\t"
		::"b"(0), "d"(buf), "r"(read_num)
	);
}

static
void buf_write64(void *buf, int read_num)
{
	asm  volatile (
		"xor %0, %0\n\t"
"LOOP_write64:\n\t"
		"mov (%1),%1\n\t"
		"movq %%r15, 8*8(%1)\n"
		"movq %%r15, 9*8(%1)\n"
		"movq %%r15, 10*8(%1)\n"
		"movq %%r15, 11*8(%1)\n"
		"movq %%r15, 12*8(%1)\n"
		"movq %%r15, 13*8(%1)\n"
		"movq %%r15, 14*8(%1)\n"
		"movq %%r15, 15*8(%1)\n"
		"inc %0\n\t"
		"cmp %2,%0\n\t"
		"jb LOOP_write64\n\t"
"STOP_write64:\n\t"
		::"b"(0), "d"(buf), "r"(read_num)
	);
}

static void
latency_calculate(uint64_t count, uint64_t dur_cyc, uint64_t total_cyc)
{
	double sec, lat;

	sec = (double)total_cyc / (double)s_clkofsec;
	lat = ((double)dur_cyc * s_nsofclk) / (double)count;
	printf("%8.1fs  %13.1f\n", sec, lat);
	fflush(stdout);
}

void
arch__dependent_read(void *buf, int meas_sec)
{
	uint64_t total_count = 0, dur_count = 0;
	uint64_t start_tsc, end_tsc, prev_tsc;
	uint64_t run_cyc, total_cyc, dur_cyc;
	uint64_t end_buf;

	end_buf = (uint64_t)buf + (uint64_t)BUF_SIZE;

	printf("buf address %llx, buf end %llx\n", buf, end_buf);

	printf("\n%9s   %13s\n", "Time", "Latency(ns)");
	printf("-------------------------\n");

	run_cyc = (uint64_t)((uint64_t)meas_sec *
	    (uint64_t)((double)(NS_SEC) * (1.0 / s_nsofclk)));

	start_tsc = rdtsc();
	end_tsc = start_tsc;
	prev_tsc = start_tsc;

	while (1) {
		total_cyc = end_tsc - start_tsc;
		dur_cyc = end_tsc - prev_tsc;

		if (((uint64_t)buf + (uint64_t)(READ_NUM * BUF_ELE_SIZE)) > end_buf) {
		        break;
		}

		if (dur_cyc >= s_clkofsec) {
		  	latency_calculate(dur_count, dur_cyc, total_cyc);
			prev_tsc = rdtsc();
			dur_count = 0;
		}

		if (total_cyc >= run_cyc) {
			break;
		}

		if (total_count > 0) {
			s_latest_avglat = ((double)total_cyc * s_nsofclk) / (double)total_count;
		}

		buf_read(buf, READ_NUM);

		dur_count += READ_NUM;
		total_count += READ_NUM;
		end_tsc = rdtsc();
	}

	printf("-------------------------\n");

	if (total_count > 0) {
		printf("%9s  %13.1f\n\n", "Average",
		    ((double)total_cyc * s_nsofclk) / (double)total_count);
		printf("total_count %ld\n", total_count);
	} else {
		printf("%9s  %13.1f\n\n", "Average", 0.0);
	}
}

void
arch__dependent_bw_read(void *buf, int meas_sec, FILE *fp)
{
        int i;
	double bw_number;

	fprintf(fp, "arch__dependent_bw_read\n");
	//           1234567890123456789012345678901234567890123456789012345678901234567890123
	fprintf(fp, "                                pattern;  Latency (ns); bandwidth (mb/s) \n");

	for (i = 0; i < BW_RS_BASIC_OPS_TASK_COUNT; i++) {
	  //	  printf("[%d]%s;.......\n", i, bandwidth_rs_tasks_str[i]);
	  arch__dependent_bw_read_th(buf, meas_sec, i, fp, &bw_number);
	}

}

void
arch__dependent_bw_read_th(void *buf, int meas_sec, int i, FILE *fp, double *bw_number)
{
	uint64_t total_count = 0, dur_count = 0;
	uint64_t start_tsc, end_tsc, prev_tsc;
	uint64_t run_cyc, total_cyc, dur_cyc;
	uint64_t end_buf;
	char *tmp_buf;
	struct timeval s_time, e_time;
	struct tm * t1, t2;
	long long time_diff;
	int k;

	end_buf = (uint64_t)buf + (uint64_t)BUF_SIZE;

	//	printf("bandwidth ......\n");
	//	printf("buf address %llx, buf end %llx\n", buf, end_buf);

	k = BW_RS_BASIC_OPS_TASK_COUNT;
	//	printf("BW_RS_BASIC_OPS_TASK_COUNT = %d, func id = %d, %s\n", k, i, bandwidth_rs_tasks_str[i]);

	//	printf("\n%9s   %13s\n", "Time", "Latency(ns)");
	//	printf("-------------------------\n");

	run_cyc = (uint64_t)((uint64_t)meas_sec *
	    (uint64_t)((double)(NS_SEC) * (1.0 / s_nsofclk)));

	//	  printf("[%d]%s  ", i, bandwidth_rs_tasks_str[i]);

	  gettimeofday(&s_time, 0);
	  start_tsc = rdtsc();
	  end_tsc = start_tsc;
	  prev_tsc = start_tsc;

	  while (1) {
		total_cyc = end_tsc - start_tsc;
		dur_cyc = end_tsc - prev_tsc;
		tmp_buf = buf;

		// buf_read64(buf, READ_NUM);
		bw_rs_bench_func[i](tmp_buf, READ_NUM);
		gettimeofday(&e_time, 0);

		dur_count += READ_NUM;
		total_count += READ_NUM;
		end_tsc = rdtsc();

		if (((uint64_t)buf + (uint64_t)(READ_NUM * BUF_ELE_SIZE)) > end_buf) {
		  printf("buf %lx\n", buf);
		        break;
		}
		
		if (dur_cyc >= s_clkofsec) {
		  //			latency_calculate(dur_count, dur_cyc, total_cyc);
			prev_tsc = rdtsc();
			dur_count = 0;
		}

		if (total_cyc >= run_cyc) {
			break;
		}

		if (total_count > 0) {
			s_latest_avglat = ((double)total_cyc * s_nsofclk) / (double)total_count;
		}

	  }
	  
	  //	printf("-------------------------\n");

	  if (total_count > 0) {
	    time_diff = (e_time.tv_sec - s_time.tv_sec)*1000*1000 + e_time.tv_usec - s_time.tv_usec;
	    *bw_number =  ((double)(total_count * 64))/((double)time_diff);
	    fprintf(fp, "[%02d]%35s; %'13.1f; ", i, bandwidth_rs_tasks_str[i], 
		   ((double)(time_diff * 1000) / (double)total_count));
	    fprintf(fp, " %'13.1f\n", ((double)(total_count * 64))/((double)time_diff));
	    /*
	    printf("[%d]%s Latency Ave %6.1f ns; ", i, bandwidth_rs_tasks_str[i], 
		    ((double)total_cyc * s_nsofclk) / (double)total_count);
	    printf("BW %'13.1f B/s; \n", (total_count * 64)/((double)total_cyc/s_clkofsec));
	    */
	    //	    printf("total count %'u; total cycle %'u\n", total_count, total_cyc);
	  } else {
		printf("%9s  %13.1f\n\n", "Average", 0.0);
	  }
}

void
arch__dependent_write(void *buf, int meas_sec)
{
	uint64_t total_count = 0, dur_count = 0;
	uint64_t start_tsc, end_tsc, prev_tsc;
	uint64_t run_cyc, total_cyc, dur_cyc;

	printf("-----write latency-------\n");
	printf("\n%9s   %13s\n", "Time", "Latency(ns)");
	printf("-------------------------\n");

	latency_job(buf);
	printf("-------------------------\n");

}

const char * get_funcname_str(int k)
{

  return bandwidth_rs_tasks_str[k];

}

const int get_no_of_rs_tasks()
{

  return BW_RS_BASIC_OPS_TASK_COUNT;

}

/* 
The following code is copied from 

https://github.com/NVSL/OptaneStudy/src
task.c

 */

#ifndef KSTAT
#define KSTAT
#endif

#define BENCHMARK_BEGIN(flags) \
	drop_cache(); 
	/*local_irq_save(flags);*/		\
	/*local_irq_disable();*/		\
	/*kernel_fpu_begin();*/

#define BENCHMARK_END(flags) 
        /*kernel_fpu_end();*/				\
	/*local_irq_restore(flags);*/		\
	/*local_irq_enable();*/


/* Latency Test
 * 1 Thread
 * Run a series of microbenchmarks on combination of store, flush and fence operations.
 * Sequential -> Random
 */
int latency_job(void *buf)
{
	int i, j;
	static unsigned long flags;
	unsigned long result;

	long rbuf[LATENCY_OPS_COUNT+BASIC_OPS_TASK_COUNT];
	int rpages;
#ifdef KSTAT
	long total;
	long average, stddev;
	unsigned long min, max;
#endif
	int skip;
	int smp_processor;
	int loop_count;
#define MAX_LATENCY 10000
	int histgram[MAX_LATENCY];
	int uniq_count;

	//smp_processor = smp_processor_id():
	//printf("BASIC_OP at smp id %d\n", smp_processor);
	// rpages = roundup((2 * BASIC_OPS_TASK_COUNT + 1) * LATENCY_OPS_COUNT * sizeof(long), PAGE_SIZE) >> PAGE_SHIFT;
	printf("BASIC_OPS_TASK_COUNT = %d\n LATENCY_OPS_COUNT = %d\n", BASIC_OPS_TASK_COUNT, LATENCY_OPS_COUNT);
	printf("                                pattern; Latency (ns) \n");

	/* Sequential */
	for (i = 0; i < BASIC_OPS_TASK_COUNT; i++)
	{
	  // printf("Running %s\n", latency_tasks_str[i]);

		skip = latency_tasks_skip[i]; /* request size */
#ifdef KSTAT
		total = 0;
		min = 0xffffffff;
		max = 0;
		loop_count = 0;
#endif
		for (j = 0; j < MAX_LATENCY; j++)
		{
		        histgram[j] = 0;
		}

		for (j = 0; j < LATENCY_OPS_COUNT; j++)
		{
			result = bench_func[i](buf);
			buf += skip;
			loop_count++;
			//printf("bench_func %d result %d, buf %x\n", i, result, buf);
#ifdef KSTAT
			if (result < min)
				min = result;
			if (result >= MAX_LATENCY){
			        loop_count--;
				result = 0;
			} else if (result > max)
				max = result;
			if (result != 0)
			  histgram[result]++;
			rbuf[j] = result;
			total += result;
#endif
		}
#ifdef KSTAT
		/*
		average = total / loop_count;
		stddev = 0;
		for (j = 0; j < loop_count; j++)
		{
			stddev += (rbuf[j] - average) * (rbuf[j] - average);
			rbuf[j] = 0;
		}
		// printf("[%d]%s avg %lu stddev^2 %lu max %lu min %lu loop count %lu: ", i, latency_tasks_str[i], average, stddev / loop_count, max, min, loop_count);
		uniq_count = 0;
		for (j = 0; j < MAX_LATENCY; j++)
		{
		        if (histgram[j] != 0)
			{
			  //printf("%s, %d, %d, \n", latency_tasks_str[i], j, histgram[j]);
				histgram[j] = 0;
				uniq_count++;
			}
		}
		*/

		printf("[%02d]%35s;   %6.1f \n", i, latency_tasks_str[i], (double)total/(double)loop_count);
#else
		printf("[%02d]%35s Done\n", i, latency_tasks_str[i]);
#endif
	}

	/* Random */
#ifdef RANDOM
	for (i = 0; i < BASIC_OPS_TASK_COUNT; i++)
	{
	        //printf("Generating random bytes at %p, size %lx", loc, LATENCY_OPS_COUNT * 8);
		//get_random_bytes(sbi->rep->virt_addr, LATENCY_OPS_COUNT * 8);

		printf("Random Running %s\n", latency_tasks_str[i]);
		//buf = ctx->addr;
#ifdef KSTAT
		total = 0;
		min = 0xffffffff;
		max = 0;
#endif
		//rbuf = (long *)(sbi->rep->virt_addr + (i + BASIC_OPS_TASK_COUNT + 1) * LATENCY_OPS_COUNT * sizeof(long));
		BENCHMARK_BEGIN(flags);
		for (j = 0; j < 1; j++)
		{
		  result = bench_func[i](buf); //(&buf[loc[j] & BASIC_OP_MASK]);
			rbuf[j] = result;
#ifdef KSTAT
			if (result < min)
				min = result;
			if (result > max)
				max = result;
			total += result;
#endif
		}
		BENCHMARK_END(flags);

#ifdef KSTAT
		average = total / LATENCY_OPS_COUNT;
		stddev = 0;
		for (j = 0; j < LATENCY_OPS_COUNT; j++)
		{
			stddev += (rbuf[i] - average) * (rbuf[i] - average);
		}
		printf("[%d]%s avg %lu, stddev^2 %lu, max %lu, min %lu\n", i, latency_tasks_str[i], average, stddev / LATENCY_OPS_COUNT, max, min);
#else
		printf("[%d]%s done\n", i, latency_tasks_str[i]);
#endif
	}
#endif // 0
	return 0;
}

/* Bandwidth Test
 * 1 Thread
 * Run a series of microbenchmarks on combination of store, flush and fence operations.
 * Sequential -> Random
 */
int bandwidth_job(void *buf, void *d_buf, long buf_size, long d_buf_size, long bw_ops_count)
{
	int i, j;
	static unsigned long flags;
	uint64_t result;

	uint64_t rbuf[LATENCY_OPS_COUNT+BW_RS_BASIC_OPS_TASK_COUNT];
	int rpages;
#ifdef KSTAT
	uint64_t total;
	uint64_t average, stddev;
	uint64_t min, max;
#endif
	int skip;
	int smp_processor;
	int loop_count;
#define MAX_LATENCY 10000
	int histgram[MAX_LATENCY];
	unsigned long bandwidth;
#define CLOCK 2500 // MHz check /proc/cpuinfo
	unsigned long load_store_size;
#define DATA_SIZE 64 // byte
	struct timeval s_time, e_time;
	struct tm * t1, t2;
	long long time_diff;

	//smp_processor = smp_processor_id():
	//printf("BASIC_OP at smp id %d\n", smp_processor);
	// rpages = roundup((2 * BW_BASIC_OPS_TASK_COUNT + 1) * LATENCY_OPS_COUNT * sizeof(long), PAGE_SIZE) >> PAGE_SHIFT;
	printf("BW_BASIC_OPS_TASK_COUNT = %d\n bw_ops_count = %'d\n", BW_RS_BASIC_OPS_TASK_COUNT, bw_ops_count);

	load_store_size = DATA_SIZE * bw_ops_count;

	printf("Bandwidth load_store_size %'u Kbyte\n", load_store_size/1000);
	printf("Buffer address %llx \n", (void *)buf);

	/* Sequential */
	for (i = 0; i < BW_RS_BASIC_OPS_TASK_COUNT; i++)
	{
	  // printf("Running %s\n", bandwidth_tasks_str[i]);
#ifdef KSTAT
		total = 0;
		min = 0xffffffffffffffff;
		max = 0;
		loop_count = 0;
#endif
		for (j = 0; j < 3; j++)
		{
		  gettimeofday(&s_time, 0);
		  result = bw_rs_bench_func[i](buf, bw_ops_count);
		  gettimeofday(&e_time, 0);
		  time_diff = (e_time.tv_sec - s_time.tv_sec)*1000*1000 + e_time.tv_usec - s_time.tv_usec;
		  t1 = localtime(&e_time.tv_sec);
		  printf("result %'u, result/CLOCK %'u, time_diff (gettimeofday) %'u; %d/%02d/%02d %02d:%02d:%02d.%d\n", result, result/CLOCK, time_diff, t1->tm_year+1900, t1->tm_mon+1, t1->tm_mday, t1->tm_hour, t1->tm_min, t1->tm_sec, e_time.tv_usec);
		  if (result/CLOCK > time_diff ){
		    printf("rdtsc number (result/CLOCK) %'u is bigger than gettimeofday() %'u \n", result/CLOCK, time_diff);
		    result = time_diff * CLOCK;
		  };

		  loop_count++;
#ifdef KSTAT

		  if (result < min)
		    min = result;

		  if (result > max)
		    max = result;

		  rbuf[j] = result;
		  total += result;
		  printf("bw_bench_func %d, %d result %'u, max %'u, min %'u, total %'u, time %'u[us], bandwidth %'u MB/s\n", i, j, result, max, min, total, result/CLOCK, load_store_size*CLOCK/result);
#endif
		}
#ifdef KSTAT
		average = total / loop_count;
		stddev = 0;
		printf("[%d]%s, clock ", i, bandwidth_rs_tasks_str[i]);
		for (j = 0; j < loop_count; j++)
		{
			stddev += (rbuf[j] - average) * (rbuf[j] - average);
			printf("%'lu, %'lu[us],  ", rbuf[j], rbuf[j]/CLOCK);
			rbuf[j] = 0;
		}
		// printf("[%d]%s avg %lu stddev^2 %lu max %lu min %lu loop count %lu: ", i, latency_tasks_str[i], average, stddev / loop_count, max, min, loop_count);
		printf("\n[%d]%s avg %'lu, stddev^2 %'lu, max %'lu, min %'lu, loop count %lu, time %'u[us], bandwidth %'u MB/s\n", i, bandwidth_rs_tasks_str[i], average, stddev / loop_count, max, min, loop_count, average/CLOCK, load_store_size*CLOCK/average);
#else
		printf("[%d]%s Done\n", i, bandwidth_tasks_str [i]);
#endif
	}
	return 0;
}
