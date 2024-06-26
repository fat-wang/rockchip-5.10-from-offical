/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) Rockchip Electronics Co., Ltd.
 *
 * Author:
 *  Cerf Yu <cerf.yu@rock-chips.com>
 */

#ifndef __LINUX_RKRGA_MM_H_
#define __LINUX_RKRGA_MM_H_

#include "rga_drv.h"

enum rga_mm_flag {
	/* It will identify whether the buffer is within 0 ~ 4G. */
	RGA_MEM_UNDER_4G		= 1 << 0,
	/* Logo enable IOMMU */
	RGA_MEM_NEED_USE_IOMMU		= 1 << 1,
	/* Flag this is a physical contiguous memory. */
	RGA_MEM_PHYSICAL_CONTIGUOUS	= 1 << 2,
	/* need force flush cache */
	RGA_MEM_FORCE_FLUSH_CACHE	= 1 << 3,
};

struct rga_mm {
	struct mutex lock;

	/*
	 * @memory_idr:
	 *
	 * Mapping of memory object handles to object pointers. Used by the GEM
	 * subsystem. Protected by @memory_lock.
	 */
	struct idr memory_idr;

	/* the count of buffer in the cached_list */
	int buffer_count;
};

static inline bool rga_mm_is_invalid_dma_buffer(struct rga_dma_buffer *buffer)
{
	if (buffer == NULL)
		return true;

	return buffer->scheduler == NULL ? true : false;
}

struct rga_internal_buffer *rga_mm_lookup_handle(struct rga_mm *mm_session, uint32_t handle);
int rga_mm_lookup_flag(struct rga_mm *mm_session, uint64_t handle);
dma_addr_t rga_mm_lookup_iova(struct rga_internal_buffer *buffer);
struct sg_table *rga_mm_lookup_sgt(struct rga_internal_buffer *buffer);

void rga_mm_dump_buffer(struct rga_internal_buffer *dump_buffer);
void rga_mm_dump_info(struct rga_mm *session);

int rga_mm_map_job_info(struct rga_job *job);
void rga_mm_unmap_job_info(struct rga_job *job);

int rga_mm_import_buffer(struct rga_external_buffer *external_buffer,
			 struct rga_session *session);
int rga_mm_release_buffer(uint32_t handle);
int rga_mm_session_release_buffer(struct rga_session *session);

int rga_mm_init(struct rga_mm **session);
int rga_mm_remove(struct rga_mm **session);

#endif
