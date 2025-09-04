#ifndef __DAXA_UTILS_TASK_GRAPH_H__
#define __DAXA_UTILS_TASK_GRAPH_H__

#if !DAXA_BUILT_WITH_UTILS_TASK_GRAPH
#error "[package management error] You must build Daxa with the DAXA_ENABLE_UTILS_TASK_GRAPH CMake option enabled, or request the utils-task-graph feature in vcpkg"
#endif

#include <daxa/c/utils/task_graph_types.h>

typedef struct daxa_TaskGraphImpl* daxa_TaskGraph;
typedef struct daxa_TaskImpl* daxa_Task;

typedef struct
{
    daxa_u32 size;
    const char* name;
} daxa_TaskTransientBufferInfo;

typedef daxa_TaskTransientBufferInfo daxa_TaskTransientTlasInfo;

typedef struct
{
    daxa_u32 dimensions;
    VkFormat format;
    VkExtent3D size;
    daxa_u32 mip_level_count;
    daxa_u32 array_layer_count;
    daxa_u32 sample_count;
    const char* name;
} daxa_TaskTransientImageInfo;

static const daxa_TaskTransientImageInfo DAXA_DEFAULT_TASK_TRANSIENT_IMAGE_INFO = {
    2,
    VK_FORMAT_R8G8B8A8_SRGB,
    {0,0,0},
    1,
    1,
    1,
    NULL
};

typedef struct
{
    daxa_Device device;
    daxa_Optional(daxa_Swapchain) swapchain;
    daxa_Bool8 reorder_tasks;
    daxa_Bool8 alias_transient;
    daxa_Bool8 use_split_barriers;
    daxa_Bool8 jit_compile_permutations;
    size_t permutation_condition_count;
    daxa_Bool8 enable_command_labels;
    daxa_f32vec4 task_graph_label_color;
    daxa_f32vec4 task_batch_label_color;
    daxa_f32vec4 task_label_color;
    daxa_Bool8 record_debug_information;
    daxa_u32 staging_memory_pool_size;
    daxa_u32 task_memory_pool_size;
    daxa_ImageUsageFlags additional_transient_image_usage_flags;
    void (*pre_task_callback)(daxa_TaskInterface* task_interface, void* user_data);
    void (*post_task_callback)(daxa_TaskInterface* task_interface, void* user_data);
    daxa_Queue default_queue;
    const char* name;
    void* user_data;
} daxa_TaskGraphInfo;

static const daxa_TaskGraphInfo DAXA_DEFAULT_TASK_GRAPH_INFO = {
    NULL,
    {NULL, 0},
    1,
    0,
    1,
    0,
    0,
    1,
    {0.463f, 0.333f, 0.671f, 1.0f},
    {0.563f, 0.433f, 0.771f, 1.0f},
    {0.663f, 0.533f, 0.871f, 1.0f},
    0,
    1u << 17u,
    1u << 19u,
    0,
    NULL,
    NULL,
    DAXA_QUEUE_MAIN,
    NULL
};

typedef struct
{
    daxa_u32 condition_index;
    void (*when_true)(void);
    void (*when_false)(void);
} daxa_TaskGraphConditionalInfo;

typedef struct
{
    daxa_SpanToConst(daxa_Bool8) permutation_condition_values;
    daxa_Bool8 record_debug_string;
} daxa_TaskExecutionInfo;

typedef struct
{
    VkPipelineStageFlags* additional_src_stages;
    daxa_SpanToConst(daxa_ExecutableCommandList)* additional_command_lists;
    daxa_SpanToConst(daxa_BinarySemaphore)* additional_wait_binary_semaphores;
    daxa_SpanToConst(daxa_BinarySemaphore)* additional_signal_binary_semaphores;
    daxa_SpanToConst(daxa_TimelinePair)* additional_wait_timeline_semaphores;
    daxa_SpanToConst(daxa_TimelinePair)* additional_signal_timeline_semaphores;
} daxa_TaskSubmitInfo;

typedef struct
{
    daxa_Queue queue;
    daxa_SpanToConst(daxa_BinarySemaphore)* additional_binary_semaphores;
} daxa_TaskPresentInfo;

typedef struct
{
    char padding;
} daxa_TaskCompleteInfo;

typedef struct
{
    daxa_TaskBufferView buffer;
    daxa_u64 offset;
    daxa_u64 size;
    daxa_u32 clear_value;
    daxa_Queue queue;
    const char* name;
} daxa_TaskBufferClearInfo;

typedef struct
{
    daxa_TaskImageView view;
    VkClearValue clear_value;
    daxa_Queue queue;
    const char* name;
} daxa_TaskImageClearInfo;

typedef struct
{
    daxa_TaskBufferView src;
    daxa_TaskBufferView dst;
    daxa_Queue queue;
    const char* name;
} daxa_TaskBufferCopyInfo;

typedef struct
{
    daxa_TaskImageView src;
    daxa_TaskImageView dst;
    daxa_Queue queue;
    const char* name;
} daxa_TaskImageCopyInfo;

typedef struct
{
    const char* name;
    daxa_TaskType task_type;
    daxa_Queue queue;
    void* user_data;
} daxa_TaskInfo;

DAXA_EXPORT void daxa_create_task(daxa_TaskInfo const* info, daxa_Task* out);
DAXA_EXPORT void daxa_task_uses(daxa_Task task, daxa_TaskAttachmentInfo attachment);
DAXA_EXPORT void daxa_task_executes(daxa_Task task, void (*execute_callback)(daxa_TaskInterface* ti, void* user_data));
DAXA_EXPORT void daxa_destroy_task(daxa_Task task);

DAXA_EXPORT void daxa_create_task_graph(daxa_TaskGraphInfo const* info, daxa_TaskGraph* out);
DAXA_EXPORT void daxa_destroy_task_graph(daxa_TaskGraph graph);

DAXA_EXPORT void daxa_tg_use_persistent_buffer(daxa_TaskGraph graph, daxa_TaskBuffer buffer);
DAXA_EXPORT void daxa_tg_use_persistent_blas(daxa_TaskGraph graph, daxa_TaskBlas blas);
DAXA_EXPORT void daxa_tg_use_persistent_tlas(daxa_TaskGraph graph, daxa_TaskTlas tlas);
DAXA_EXPORT void daxa_tg_use_persistent_image(daxa_TaskGraph graph, daxa_TaskImage image);

DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskBufferView daxa_tg_create_transient_buffer(daxa_TaskGraph graph, daxa_TaskTransientBufferInfo const* info);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskTlasView daxa_tg_create_transient_tlas(daxa_TaskGraph graph, daxa_TaskTransientTlasInfo const* info);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskImageView daxa_tg_create_transient_image(daxa_TaskGraph graph, daxa_TaskTransientImageInfo const* info);

DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskTransientBufferInfo const* daxa_tg_transient_buffer_info(daxa_TaskGraph graph, daxa_TaskBufferView transient);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskTransientTlasInfo const* daxa_tg_transient_tlas_info(daxa_TaskGraph graph, daxa_TaskTlasView transient);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskTransientImageInfo const* daxa_tg_transient_image_info(daxa_TaskGraph graph, daxa_TaskImageView transient);

DAXA_EXPORT void daxa_tg_clear_buffer(daxa_TaskGraph graph, daxa_TaskBufferClearInfo const* info);
DAXA_EXPORT void daxa_tg_clear_image(daxa_TaskGraph graph, daxa_TaskImageClearInfo const* info);

DAXA_EXPORT void daxa_tg_copy_buffer_to_buffer(daxa_TaskGraph graph, daxa_TaskBufferCopyInfo const* info);
DAXA_EXPORT void daxa_tg_copy_image_to_image(daxa_TaskGraph graph, daxa_TaskImageCopyInfo const* info);

DAXA_EXPORT void daxa_tg_add_task(daxa_TaskGraph graph, daxa_Task task);

DAXA_EXPORT void daxa_tg_conditional(daxa_TaskGraph graph, daxa_TaskGraphConditionalInfo const* info);
DAXA_EXPORT void daxa_tg_submit(daxa_TaskGraph graph, daxa_TaskSubmitInfo const* info);
DAXA_EXPORT void daxa_tg_present(daxa_TaskGraph graph, daxa_TaskPresentInfo const* info);

DAXA_EXPORT void daxa_tg_complete(daxa_TaskGraph graph, daxa_TaskCompleteInfo const* info);
DAXA_EXPORT void daxa_tg_execute(daxa_TaskGraph graph, daxa_TaskExecutionInfo const* info);

#endif

