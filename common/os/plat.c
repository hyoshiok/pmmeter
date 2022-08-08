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

/* This file contains a general layer of functions. */

#include <inttypes.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "../include/types.h"
#include "../include/util.h"
#include "../include/os/plat.h"

cpu_type_t s_cpu_type = CPU_UNSUP;

/*
 * Platform-independent function to get the event configuration for profiling.
 */
void
plat_profiling_config(perf_count_id_t perf_count_id, plat_event_config_t *cfg)
{
	pfn_plat_profiling_config_t pfn =
	    s_plat_profiling_config[s_cpu_type];

	if (pfn != NULL) {
		pfn(perf_count_id, cfg);
	}
}

/*
 * Platform-independent function to get the event configuration for LL.
 */
void
plat_ll_config(plat_event_config_t *cfg)
{
	pfn_plat_ll_config_t pfn =
	    s_plat_ll_config[s_cpu_type];

	if (pfn != NULL) {
		pfn(cfg);
	}
}

void
plat_config_get(perf_count_id_t perf_count_id, plat_event_config_t *cfg,
	plat_event_config_t *cfg_arr)
{
	cfg->type = cfg_arr[perf_count_id].type;
	cfg->config = cfg_arr[perf_count_id].config;
	cfg->other_attr = cfg_arr[perf_count_id].other_attr;
	cfg->extra_value = cfg_arr[perf_count_id].extra_value;
	strncpy(cfg->desc, cfg_arr[perf_count_id].desc, PLAT_EVENT_DESC_SIZE);
	cfg->desc[PLAT_EVENT_DESC_SIZE - 1] = 0;
}

/*
 * Platform-independent function to return the number of offcore counters.
 */
int
plat_offcore_num(void)
{
	pfn_plat_offcore_num_t pfn =
	    s_plat_offcore_num[s_cpu_type];

	if (pfn != NULL) {
		return (pfn());
	}
	
	return (0);
}
