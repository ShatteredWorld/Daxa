#pragma once

#include "impl_core.hpp"

#include "impl_instance.hpp"
#include "impl_command_recorder.hpp"
#include "impl_pipeline.hpp"
#include "impl_swapchain.hpp"
#include "impl_gpu_resources.hpp"
#include "impl_timeline_query.hpp"

#include <daxa/c/device.h>

using namespace daxa;

struct SubmitZombie
{
    std::vector<daxa_BinarySemaphore> binary_semaphores = {};
    std::vector<daxa_TimelineSemaphore> timeline_semaphores = {};
};

static inline constexpr u64 MAX_SUBMITS_IN_FLIGHT = 64;
static inline constexpr u64 MAIN_QUEUE_INDEX = 0;
static inline constexpr u64 FIRST_COMPUTE_QUEUE_IDX = 1;
static inline constexpr u64 FIRST_TRANSFER_QUEUE_IDX = FIRST_COMPUTE_QUEUE_IDX + DAXA_MAX_COMPUTE_QUEUE_COUNT;

struct daxa_ImplDevice final : public ImplHandle
{
    // General data:
    daxa_Instance instance = {};
    DeviceInfo info = {};
    VkPhysicalDevice vk_physical_device = {};
    daxa_DeviceProperties physical_device_properties = {};
    VkDevice vk_device = {};
    VmaAllocator vma_allocator = {};

    // Dynamic State:
    PFN_vkCmdSetRasterizationSamplesEXT vkCmdSetRasterizationSamplesEXT = {};

    // Debug utils:
    PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = {};
    PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT = {};
    PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT = {};
    PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR = {};

    // Mesh shader:
    PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT = {};
    PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT = {};
    PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT = {};
    VkPhysicalDeviceMeshShaderPropertiesEXT mesh_shader_properties = {};

    // Ray tracing:
    PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = {};
    PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR = {};
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = {};
    PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR = {};
    PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR = {};
    PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = {};
    PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR = {};
    PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR = {};
    PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR = {};
    PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR = {};

    VkBuffer buffer_device_address_buffer = {};
    u64 * buffer_device_address_buffer_host_ptr = {};
    VmaAllocation buffer_device_address_buffer_allocation = {};

    // 'Null' resources, used to fill empty slots in the resource table after a resource is destroyed.
    // This is not necessary, as it is valid to have "garbage" in the descriptor slots given our enabled features.
    // BUT, accessing garbage descriptors normally causes a device lost immediately, making debugging much harder.
    // So instead of leaving dead descriptors dangle, daxa overwrites them with 'null' descriptors that just contain some debug value (pink 0xFF00FFFF).
    // This in particular prevents device hang in the case of a use after free if the device does not encounter a race condition on the descriptor update before.
    VkBuffer vk_null_buffer = {};
    VkImage vk_null_image = {};
    VkImageView vk_null_image_view = {};
    VkSampler vk_null_sampler = {};
    VmaAllocation vk_null_buffer_vma_allocation = {};
    VmaAllocation vk_null_image_vma_allocation = {};

    // Command Buffer/Pool recycling:
    std::mutex main_queue_command_pool_buffer_recycle_mtx = {};
    CommandPoolPool buffer_pool_pool = {};

    // Gpu Shader Resource Object table:
    GPUShaderResourceTable gpu_sro_table = {};

    // Every submit to any queue increments the global submit timeline
    // Each queue stores a mapping between local submit index and global submit index for each of their in flight submits.
    // When destroying a resource it becomes a zombie, the zombie remembers the current global timeline value.
    // When collect garbage is called, the zombies timeline values are compared against submits running in all queues.
    // If the zombies global submit index is smaller then global index of all submits currently in flight (on all queues), we can safely clean the resource up.
    std::atomic_uint64_t global_submit_timeline = {};
    std::recursive_mutex zombies_mtx = {};
    std::deque<std::pair<u64, CommandRecorderZombie>> main_queue_command_list_zombies = {};
    std::deque<std::pair<u64, BufferId>> buffer_zombies = {};
    std::deque<std::pair<u64, ImageId>> image_zombies = {};
    std::deque<std::pair<u64, ImageViewId>> image_view_zombies = {};
    std::deque<std::pair<u64, SamplerId>> sampler_zombies = {};
    std::deque<std::pair<u64, TlasId>> tlas_zombies = {};
    std::deque<std::pair<u64, BlasId>> blas_zombies = {};
    std::deque<std::pair<u64, SemaphoreZombie>> semaphore_zombies = {};
    std::deque<std::pair<u64, EventZombie>> split_barrier_zombies = {};
    std::deque<std::pair<u64, PipelineZombie>> pipeline_zombies = {};
    std::deque<std::pair<u64, TimelineQueryPoolZombie>> timeline_query_pool_zombies = {};
    std::deque<std::pair<u64, MemoryBlockZombie>> memory_block_zombies = {};

    // Used to sync access to the queues submits in flight lists.
    std::recursive_mutex queue_mtx = {};
    // Queues
    struct ImplQueue
    {
        u32 vk_queue_family_index = {};
        VkQueue vk_queue = {};
        VkSemaphore gpu_queue_local_timeline = {};
        std::atomic_uint64_t cpu_queue_local_timeline = {};
        /// WARNING: In flight submit queues must be synchronized with queue_mtx!
        ///          This is because collect garbage (which pops from the in_flight_submit_global_indices) can race with 
        ///          submit operation running on another thread (which pushes into in_flight_submit_global_indices)
        // Stores the global submission index of all in flight submits in the order they were made on this queue
        std::array<u64, MAX_SUBMITS_IN_FLIGHT> in_flight_submit_global_indices = {};
        u64 in_flight_submit_count = {};
    };
    
    u32 main_queue_family_index = ~0u;
    u32 compute_queue_family_index = ~0u;
    u32 transfer_queue_family_index = ~0u;
    u32 compute_queue_count = ~0u;
    u32 transfer_queue_count = ~0u;
    std::array<ImplQueue, DAXA_MAX_TRANSFER_QUEUE_COUNT + DAXA_MAX_COMPUTE_QUEUE_COUNT + 1 /*main queue*/> queues = {};
    u64 queue_count = {};

    auto validate_image_slice(daxa_ImageMipArraySlice const & slice, daxa_ImageId id) -> daxa_ImageMipArraySlice;
    auto validate_image_slice(daxa_ImageMipArraySlice const & slice, daxa_ImageViewId id) -> daxa_ImageMipArraySlice;
    auto new_swapchain_image(VkImage swapchain_image, VkFormat format, u32 index, ImageUsageFlags usage, ImageInfo const & image_info) -> std::pair<daxa_Result, ImageId>;

    auto slot(daxa_BufferId id) const -> ImplBufferSlot const &;
    auto slot(daxa_ImageId id) const -> ImplImageSlot const &;
    auto slot(daxa_ImageViewId id) const -> ImplImageViewSlot const &;
    auto slot(daxa_SamplerId id) const -> ImplSamplerSlot const &;
    auto slot(daxa_TlasId id) const -> ImplTlasSlot const &;
    auto slot(daxa_BlasId id) const -> ImplBlasSlot const &;

    void cleanup_buffer(BufferId id);
    void cleanup_image(ImageId id);
    void cleanup_image_view(ImageViewId id);
    void cleanup_sampler(SamplerId id);
    void cleanup_tlas(TlasId id);
    void cleanup_blas(BlasId id);

    void zombify_buffer(BufferId id);
    void zombify_image(ImageId id);
    void zombify_image_view(ImageViewId id);
    void zombify_sampler(SamplerId id);
    void zombify_tlas(TlasId id);
    void zombify_blas(BlasId id);

    // TODO: Give physical device in info so that this function can be removed.
    // TODO: Better device selection.
    static auto create(daxa_Instance instance, daxa_DeviceInfo const & info, VkPhysicalDevice physical_device, daxa_Device device) -> daxa_Result;
    static void zero_ref_callback(ImplHandle const * handle);
};
