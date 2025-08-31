#ifndef __DAXA_UTILS_MEM_H__
#define __DAXA_UTILS_MEM_H__

#if !DAXA_BUILT_WITH_UTILS_MEM
#error "[package management error] You must build Daxa with the DAXA_ENABLE_UTILS_MEM CMake option enabled, or request the utils-mem feature in vcpkg"
#endif

#include <daxa/c/core.h>
#include <daxa/c/device.h>

typedef struct daxa_ImplRingBuffer* daxa_RingBuffer;
typedef daxa_RingBuffer daxa_TransferMemoryPool;

typedef struct
{
    daxa_Device device;
    daxa_u32 capacity;
    daxa_Bool8 prefer_device_memory;
    const char* name;
} daxa_RingBufferInfo;

typedef daxa_RingBufferInfo daxa_TransferMemoryPoolInfo;

typedef struct
{
    daxa_DeviceAddress device_address;
    void* host_address;
    daxa_u32 buffer_offset;
    size_t size;
    daxa_u64 submit_index;
} daxa_RingBufferAllocation;

DAXA_EXPORT void
daxa_create_ring_buffer(daxa_RingBufferInfo const* info, daxa_RingBuffer* out_buffer);

DAXA_EXPORT void 
daxa_destroy_ring_buffer(daxa_RingBuffer buffer);

DAXA_EXPORT DAXA_NO_DISCARD daxa_Bool8
daxa_ring_buffer_allocate(daxa_RingBuffer buffer, daxa_u32 size, daxa_u32 alignment_requirement, daxa_RingBufferAllocation* out_allocation);

DAXA_EXPORT DAXA_NO_DISCARD daxa_BufferId
daxa_ring_buffer_get_buffer(daxa_RingBuffer buffer);

DAXA_EXPORT void 
daxa_ring_buffer_get_info(daxa_RingBuffer buffer, daxa_RingBufferInfo* out_info);

DAXA_EXPORT void 
daxa_ring_buffer_reuse_memory_after_pending_submits(daxa_RingBuffer buffer);

#endif
