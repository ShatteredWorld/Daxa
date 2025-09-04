#ifndef __DAXA_UTILS_TASK_GRAPH_TYPES_H__
#define __DAXA_UTILS_TASK_GRAPH_TYPES_H__

#if !DAXA_BUILT_WITH_UTILS_TASK_GRAPH
#error "[package management error] You must build Daxa with the DAXA_ENABLE_UTILS_TASK_GRAPH CMake option enabled, or request the utils-task-graph feature in vcpkg"
#endif

#include <daxa/c/daxa.h>
#include <daxa/c/utils/mem.h>

#define DAXA_MAX_TASK_ATTACHMENTS 48

typedef struct daxa_TaskBufferImpl* daxa_TaskBuffer;
typedef struct daxa_TaskBlasImpl* daxa_TaskBlas;
typedef struct daxa_TaskTlasImpl* daxa_TaskTlas;
typedef struct daxa_TaskImageImpl* daxa_TaskImage;

typedef uint8_t daxa_TaskAttachmentType;
static const daxa_TaskAttachmentType DAXA_TASK_ATTACHMENT_TYPE_UNDEFINED = 0;
static const daxa_TaskAttachmentType DAXA_TASK_ATTACHMENT_TYPE_BUFFER = 1;
static const daxa_TaskAttachmentType DAXA_TASK_ATTACHMENT_TYPE_BLAS = 2;
static const daxa_TaskAttachmentType DAXA_TASK_ATTACHMENT_TYPE_TLAS = 3;
static const daxa_TaskAttachmentType DAXA_TASK_ATTACHMENT_TYPE_IMAGE = 4;

typedef uint8_t daxa_TaskAccessType;
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_NONE = 0;
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT = 1;
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_READ = (1 << 1) | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT;
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_SAMPLED = 1 << 2 | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT;
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_WRITE = 1 << 3;
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_READ_WRITE = (1 << 1) | (1 << 3);
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_WRITE_CONCURRENT = DAXA_TASK_ACCESS_TYPE_WRITE | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT;
static const daxa_TaskAccessType DAXA_TASK_ACCESS_TYPE_READ_WRITE_CONCURRENT = DAXA_TASK_ACCESS_TYPE_READ_WRITE | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT;

typedef uint16_t daxa_TaskStage;
static const daxa_TaskStage DAXA_TASK_STAGE_NONE = 0;
static const daxa_TaskStage DAXA_TASK_STAGE_VERTEX_SHADER = 1;
static const daxa_TaskStage DAXA_TASK_STAGE_TESSELLATION_CONTROL_SHADER = 2;
static const daxa_TaskStage DAXA_TASK_STAGE_TESSELLATION_EVALUATION_SHADER = 3;
static const daxa_TaskStage DAXA_TASK_STAGE_GEOMETRY_SHADER = 4;
static const daxa_TaskStage DAXA_TASK_STAGE_FRAGMENT_SHADER = 5;
static const daxa_TaskStage DAXA_TASK_STAGE_TASK_SHADER = 6;
static const daxa_TaskStage DAXA_TASK_STAGE_MESH_SHADER = 7;
static const daxa_TaskStage DAXA_TASK_STAGE_PRE_RASTERIZATION_SHADERS = 8;
static const daxa_TaskStage DAXA_TASK_STAGE_RASTER_SHADER = 9;
static const daxa_TaskStage DAXA_TASK_STAGE_COMPUTE_SHADER = 10;
static const daxa_TaskStage DAXA_TASK_STAGE_RAY_TRACING_SHADER = 11;
static const daxa_TaskStage DAXA_TASK_STAGE_SHADER = 12;
static const daxa_TaskStage DAXA_TASK_STAGE_COLOR_ATTACHMENT = 13;
static const daxa_TaskStage DAXA_TASK_STAGE_DEPTH_STENCIL_ATTACHMENT = 14;
static const daxa_TaskStage DAXA_TASK_STAGE_RESOLVE = 15;
static const daxa_TaskStage DAXA_TASK_STAGE_PRESENT = 16;
static const daxa_TaskStage DAXA_TASK_STAGE_INDIRECT_COMMAND = 17;
static const daxa_TaskStage DAXA_TASK_STAGE_INDEX_INPUT = 18;
static const daxa_TaskStage DAXA_TASK_STAGE_TRANSFER = 19;
static const daxa_TaskStage DAXA_TASK_STAGE_HOST = 20;
static const daxa_TaskStage DAXA_TASK_STAGE_AS_BUILD = 21;
static const daxa_TaskStage DAXA_TASK_STAGE_ANY_COMMAND = 22;

typedef struct
{
    daxa_TaskStage stage;
    daxa_TaskAccessType type;
    daxa_TaskAttachmentType restricition;
} daxa_TaskAccess;

typedef struct
{
    daxa_u32 task_graph_index;
    daxa_u32 index;
} daxa_TaskBufferView;

typedef struct 
{
    daxa_u32 task_graph_index;
    daxa_u32 index;
} daxa_TaskBlasView;

typedef struct 
{
    daxa_u32 task_graph_index;
    daxa_u32 index;
} daxa_TaskTlasView;

typedef struct 
{
    daxa_u32 task_graph_index;
    daxa_u32 index;
    daxa_ImageMipArraySlice slice;
} daxa_TaskImageView;

typedef struct
{
    daxa_Access latest_access;
    daxa_ImageLayout latest_layout;
    daxa_ImageMipArraySlice slice;
} daxa_ImageSliceState;

typedef uint8_t daxa_TaskHeadImageArrayType;
static const daxa_TaskHeadImageArrayType DAXA_TASK_HEAD_IMAGE_ARRAY_TYPE_RUNTIME_IMAGES = 0;
static const daxa_TaskHeadImageArrayType DAXA_TASK_HEAD_IMAGE_ARRAY_TYPE_MIP_LEVELS = 1;

typedef struct
{
    daxa_SpanToConst(daxa_BufferId) buffers;
    daxa_Access latest_access;
} daxa_TrackedBuffers;

typedef struct
{
    daxa_SpanToConst(daxa_BlasId) buffers;
    daxa_Access latest_access;
} daxa_TrackedBlas;

typedef struct
{
    daxa_SpanToConst(daxa_TlasId) buffers;
    daxa_Access latest_access;
} daxa_TrackedTlas;

typedef struct
{
    daxa_SpanToConst(daxa_ImageId) images;
    daxa_SpanToConst(daxa_ImageSliceState) latest_slice_stats;
} daxa_TrackedImages;

typedef struct
{
    daxa_TrackedBuffers initial_buffers;
    const char* name;
} daxa_TaskBufferInfo;

typedef struct
{
    daxa_TrackedBlas initial_blas;
    const char* name;
} daxa_TaskBlasInfo;

typedef struct
{
    daxa_TrackedTlas initial_tlas;
    const char* name;
} daxa_TaskTlasInfo;

typedef struct
{
    daxa_TrackedImages initial_images;
    daxa_Bool8 swapchain_image;
    const char* name;
} daxa_TaskImageInfo;

typedef struct
{
    daxa_u32 value;
} daxa_TaskBufferAttachmentIndex;

typedef struct
{
    daxa_u32 value;
} daxa_TaskBlasAttachmentIndex;

typedef struct
{
    daxa_u32 value;
} daxa_TaskTlasAttachmentIndex;

typedef struct
{
    daxa_u32 value;
} daxa_TaskImageAttachmentIndex;

typedef uint16_t daxa_TaskType;
static const daxa_TaskType DAXA_TASK_TYPE_UNDEFINED = 0;
static const daxa_TaskType DAXA_TASK_TYPE_GENERAL = 1;
static const daxa_TaskType DAXA_TASK_TYPE_RASTER = 2;
static const daxa_TaskType DAXA_TASK_TYPE_COMPUTE = 3;
static const daxa_TaskType DAXA_TASK_TYPE_RAY_TRACING = 4;
static const daxa_TaskType DAXA_TASK_TYPE_TRANSFER = 5;

typedef daxa_u32 daxa_TaskResourceIndex;

typedef struct 
{
    char padding;
} daxa_UndefinedAttachment;

typedef struct
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;
    uint8_t shader_array_size;
    daxa_Bool8 shader_as_address;
} daxa_TaskBufferAttachment;

typedef struct
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;
} daxa_TaskBlasAttachment;

typedef struct
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;
    daxa_Bool8 shader_as_address;
} daxa_TaskTlasAttachment;

typedef struct 
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;
    VkImageViewType view_type;
    uint8_t shader_array_size;
    daxa_Bool8 shader_as_index;
    daxa_TaskHeadImageArrayType shader_array_type;
} daxa_TaskImageAttachment;

typedef struct
{
    daxa_TaskAttachmentType type;
    union
    {
        daxa_UndefinedAttachment undefined;
        daxa_TaskBufferAttachment buffer;
        daxa_TaskBlasAttachment blas;
        daxa_TaskTlasAttachment tlas;
        daxa_TaskImageAttachment image;
    } value;
} daxa_TaskAttachment;

typedef struct 
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;
    uint8_t shader_array_size;
    daxa_Bool8 shader_as_address;

    daxa_TaskBufferView view;
    daxa_TaskBufferView translated_view;
    daxa_SpanToConst(daxa_BufferId) ids;
} daxa_TaskBufferAttachmentInfo;

typedef struct 
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;

    daxa_TaskBlasView view;
    daxa_TaskBlasView translated_view;
    daxa_SpanToConst(daxa_BlasId) ids;
} daxa_TaskBlasAttachmentInfo;

typedef struct 
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;
    daxa_Bool8 shader_as_address;

    daxa_TaskTlasView view;
    daxa_TaskTlasView translated_view;
    daxa_SpanToConst(daxa_TlasId) ids;
} daxa_TaskTlasAttachmentInfo;

typedef struct
{
    char const * name;
    daxa_TaskAccess task_access;
    daxa_Access access;
    VkImageViewType view_type;
    uint8_t shader_array_size;
    daxa_Bool8 shader_as_index;
    daxa_TaskHeadImageArrayType shader_array_type;

    daxa_TaskImageView view;
    daxa_TaskImageView translated_view;
    daxa_ImageLayout layout;
    daxa_SpanToConst(daxa_ImageId) ids;
    daxa_SpanToConst(daxa_ImageViewId) view_ids;
} daxa_TaskImageAttachmentInfo;

typedef struct
{
    daxa_TaskAttachmentType type;
    union
    {
        daxa_UndefinedAttachment undefined;
        daxa_TaskBufferAttachmentInfo buffer;
        daxa_TaskBlasAttachmentInfo blas;
        daxa_TaskTlasAttachmentInfo tlas;
        daxa_TaskImageAttachmentInfo image;
    } value;
} daxa_TaskAttachmentInfo;

typedef struct
{
    daxa_Device* device;
    daxa_CommandRecorder* recorder;
    daxa_SpanToConst(daxa_TaskAttachmentInfo) attachment_infos;
    daxa_TransferMemoryPool* allocator;
    daxa_SpanToConst(uint8_t) attachment_shader_blob;
    const char* task_name;
    size_t task_index;
    daxa_Queue queue;
} daxa_TaskInterface;

DAXA_EXPORT void daxa_create_task_buffer(daxa_TaskBufferInfo const* info, daxa_TaskBuffer* out);
DAXA_EXPORT void daxa_destroy_task_buffer(daxa_TaskBuffer buffer);
DAXA_EXPORT void daxa_task_buffer_set_buffers(daxa_TaskBuffer buffer, daxa_TrackedBuffers const* buffers);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskBufferView daxa_task_buffer_get_view(daxa_TaskBuffer buffer);

DAXA_EXPORT void daxa_create_task_blas(daxa_TaskBlasInfo const * info, daxa_TaskBlas* out);
DAXA_EXPORT void daxa_destroy_task_blas(daxa_TaskBlas blas);
DAXA_EXPORT void daxa_task_blas_set_blas(daxa_TaskBlas blas, daxa_TrackedBlas const* tracked_blas);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskBlasView daxa_task_blas_get_view(daxa_TaskBlas blas);

DAXA_EXPORT void daxa_create_task_tlas(daxa_TaskTlasInfo const * info, daxa_TaskTlas* out);
DAXA_EXPORT void daxa_destroy_task_tlas(daxa_TaskTlas tlas);
DAXA_EXPORT void daxa_task_tlas_set_tlas(daxa_TaskTlas tlas, daxa_TrackedTlas const* tracked_tlas);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskTlasView daxa_task_tlas_get_view(daxa_TaskTlas tlas);

DAXA_EXPORT void daxa_create_task_image(daxa_TaskImageInfo const * info, daxa_TaskImage* out);
DAXA_EXPORT void daxa_destroy_task_image(daxa_TaskImage image);
DAXA_EXPORT void daxa_task_image_set_images(daxa_TaskImage image, daxa_TrackedImages const* images);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskImageView daxa_task_image_get_view(daxa_TaskImage image);

DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskImageView daxa_task_image_view_mips(daxa_TaskImageView view, daxa_u32 base_mip_level, daxa_u32 level_count);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskImageView daxa_task_image_view_layers(daxa_TaskImageView view, daxa_u32 base_array_layer, daxa_u32 layer_count);

DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskBufferAttachmentInfo const* daxa_ti_get_buffer(daxa_TaskInterface* ti,daxa_TaskBufferAttachmentIndex index);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskBufferAttachmentInfo const* daxa_ti_get_buffer_from_view(daxa_TaskInterface* ti,daxa_TaskBufferView view);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskBlasAttachmentInfo const* daxa_ti_get_blas(daxa_TaskInterface* ti,daxa_TaskBlasAttachmentIndex index);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskBlasAttachmentInfo const* daxa_ti_get_blas_from_view(daxa_TaskInterface* ti,daxa_TaskBlasView view);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskTlasAttachmentInfo const* daxa_ti_get_tlas(daxa_TaskInterface* ti,daxa_TaskTlasAttachmentIndex index);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskTlasAttachmentInfo const* daxa_ti_get_tlas_from_view(daxa_TaskInterface* ti,daxa_TaskTlasView view);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskImageAttachmentInfo const* daxa_ti_get_image(daxa_TaskInterface* ti,daxa_TaskImageAttachmentIndex index);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskImageAttachmentInfo const* daxa_ti_get_image_from_view(daxa_TaskInterface* ti,daxa_TaskImageView view);
DAXA_EXPORT DAXA_NO_DISCARD daxa_TaskAttachmentInfo const* daxa_ti_get_attachment(daxa_TaskInterface* ti,size_t index);

#endif
