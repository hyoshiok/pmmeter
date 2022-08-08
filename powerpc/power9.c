/*
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

#include <inttypes.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "../common/include/os/linux/perf_event.h"
#include "../common/include/os/plat.h"
#include "include/power9.h"

static plat_event_config_t s_power9_profiling[PERF_COUNT_NUM] = {
	{ PERF_TYPE_RAW, 0x600f4, 0, 0, "PM_RUN_CYC" },
	{ PERF_TYPE_RAW, 0x4c04c, 0, 0, "PM_DATA_FROM_DMEM" },
	{ PERF_TYPE_RAW, 0x1001e, 0, 0, "PM_CYC" },
	{ PERF_TYPE_RAW, 0x500fa, 0, 0, "PM_RUN_INST_CMPL" },
	{ PERF_TYPE_RAW, 0x2c048, 0, 0, "PM_DATA_FROM_LMEM" },
	{ PERF_TYPE_RAW, 0x3c04a, 0, 0, "PM_DATA_FROM_RMEM" },
};

static plat_event_config_t s_power9_ll = {
	PERF_TYPE_RAW, 0x0000, 0, 0, "PM_SUSPENDED"
};

void
power9_profiling_config(perf_count_id_t perf_count_id, plat_event_config_t *cfg)
{
	plat_config_get(perf_count_id, cfg, s_power9_profiling);
}

void
power9_ll_config(plat_event_config_t *cfg)
{
	memcpy(cfg, &s_power9_ll, sizeof (plat_event_config_t));
}

int
power9_offcore_num(void)
{
	return (3);
}
