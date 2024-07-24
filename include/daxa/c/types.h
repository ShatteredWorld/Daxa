#ifndef __DAXA_TYPES_H__
#define __DAXA_TYPES_H__

#include <vulkan/vulkan.h>
#include <daxa/c/core.h>

typedef enum
{
    DAXA_RESULT_SUCCESS = 0,
    DAXA_RESULT_NOT_READY = 1,
    DAXA_RESULT_TIMEOUT = 2,
    DAXA_RESULT_EVENT_SET = 3,
    DAXA_RESULT_EVENT_RESET = 4,
    DAXA_RESULT_INCOMPLETE = 5,
    DAXA_RESULT_ERROR_OUT_OF_HOST_MEMORY = -1,
    DAXA_RESULT_ERROR_OUT_OF_DEVICE_MEMORY = -2,
    DAXA_RESULT_ERROR_INITIALIZATION_FAILED = -3,
    DAXA_RESULT_ERROR_DEVICE_LOST = -4,
    DAXA_RESULT_ERROR_MEMORY_MAP_FAILED = -5,
    DAXA_RESULT_ERROR_LAYER_NOT_PRESENT = -6,
    DAXA_RESULT_ERROR_EXTENSION_NOT_PRESENT = -7,
    DAXA_RESULT_ERROR_FEATURE_NOT_PRESENT = -8,
    DAXA_RESULT_ERROR_INCOMPATIBLE_DRIVER = -9,
    DAXA_RESULT_ERROR_TOO_MANY_OBJECTS = -10,
    DAXA_RESULT_ERROR_FORMAT_NOT_SUPPORTED = -11,
    DAXA_RESULT_ERROR_FRAGMENTED_POOL = -12,
    DAXA_RESULT_ERROR_UNKNOWN = -13,
    DAXA_RESULT_ERROR_OUT_OF_POOL_MEMORY = -1000069000,
    DAXA_RESULT_ERROR_INVALID_EXTERNAL_HANDLE = -1000072003,
    DAXA_RESULT_ERROR_FRAGMENTATION = -1000161000,
    DAXA_RESULT_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS = -1000257000,
    DAXA_RESULT_PIPELINE_COMPILE_REQUIRED = 1000297000,
    DAXA_RESULT_ERROR_SURFACE_LOST_KHR = -1000000000,
    DAXA_RESULT_ERROR_NATIVE_WINDOW_IN_USE_KHR = -1000000001,
    DAXA_RESULT_SUBOPTIMAL_KHR = 1000001003,
    DAXA_RESULT_ERROR_OUT_OF_DATE_KHR = -1000001004,
    DAXA_RESULT_ERROR_INCOMPATIBLE_DISPLAY_KHR = -1000003001,
    DAXA_RESULT_ERROR_VALIDATION_FAILED_EXT = -1000011001,
    DAXA_RESULT_ERROR_INVALID_SHADER_NV = -1000012000,
    DAXA_RESULT_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR = -1000023000,
    DAXA_RESULT_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR = -1000023001,
    DAXA_RESULT_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR = -1000023002,
    DAXA_RESULT_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR = -1000023003,
    DAXA_RESULT_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR = -1000023004,
    DAXA_RESULT_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR = -1000023005,
    DAXA_RESULT_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT = -1000158000,
    DAXA_RESULT_ERROR_NOT_PERMITTED_KHR = -1000174001,
    DAXA_RESULT_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT = -1000255000,
    DAXA_RESULT_THREAD_IDLE_KHR = 1000268000,
    DAXA_RESULT_THREAD_DONE_KHR = 1000268001,
    DAXA_RESULT_OPERATION_DEFERRED_KHR = 1000268002,
    DAXA_RESULT_OPERATION_NOT_DEFERRED_KHR = 1000268003,
    DAXA_RESULT_MISSING_EXTENSION = (1 << 30) + 0,
    DAXA_RESULT_INVALID_BUFFER_ID = (1 << 30) + 1,
    DAXA_RESULT_INVALID_IMAGE_ID = (1 << 30) + 2,
    DAXA_RESULT_INVALID_IMAGE_VIEW_ID = (1 << 30) + 3,
    DAXA_RESULT_INVALID_SAMPLER_ID = (1 << 30) + 4,
    DAXA_RESULT_BUFFER_DOUBLE_FREE = (1 << 30) + 5,
    DAXA_RESULT_IMAGE_DOUBLE_FREE = (1 << 30) + 6,
    DAXA_RESULT_IMAGE_VIEW_DOUBLE_FREE = (1 << 30) + 7,
    DAXA_RESULT_SAMPLER_DOUBLE_FREE = (1 << 30) + 8,
    DAXA_RESULT_INVALID_BUFFER_INFO = (1 << 30) + 9,
    DAXA_RESULT_INVALID_IMAGE_INFO = (1 << 30) + 10,
    DAXA_RESULT_INVALID_IMAGE_VIEW_INFO = (1 << 30) + 11,
    DAXA_RESULT_INVALID_SAMPLER_INFO = (1 << 30) + 12,
    DAXA_RESULT_COMMAND_LIST_COMPLETED = (1 << 30) + 13,
    DAXA_RESULT_COMMAND_LIST_NOT_COMPLETED = (1 << 30) + 14,
    DAXA_RESULT_INVALID_CLEAR_VALUE = (1 << 30) + 15,
    DAXA_RESULT_BUFFER_NOT_HOST_VISIBLE = (1 << 30) + 16,
    DAXA_RESULT_BUFFER_NOT_DEVICE_VISIBLE = (1 << 30) + 17,
    DAXA_RESULT_INCOMPLETE_COMMAND_LIST = (1 << 30) + 18,
    DAXA_RESULT_DEVICE_DOES_NOT_SUPPORT_BUFFER_COUNT = (1 << 30) + 19,
    DAXA_RESULT_DEVICE_DOES_NOT_SUPPORT_IMAGE_COUNT = (1 << 30) + 20,
    DAXA_RESULT_DEVICE_DOES_NOT_SUPPORT_SAMPLER_COUNT = (1 << 30) + 21,
    DAXA_RESULT_FAILED_TO_CREATE_NULL_BUFFER = (1 << 30) + 22,
    DAXA_RESULT_FAILED_TO_CREATE_NULL_IMAGE = (1 << 30) + 23,
    DAXA_RESULT_FAILED_TO_CREATE_NULL_IMAGE_VIEW = (1 << 30) + 24,
    DAXA_RESULT_FAILED_TO_CREATE_NULL_SAMPLER = (1 << 30) + 25,
    DAXA_RESULT_FAILED_TO_CREATE_BUFFER = (1 << 30) + 26,
    DAXA_RESULT_FAILED_TO_CREATE_IMAGE = (1 << 30) + 27,
    DAXA_RESULT_FAILED_TO_CREATE_IMAGE_VIEW = (1 << 30) + 28,
    DAXA_RESULT_FAILED_TO_CREATE_DEFAULT_IMAGE_VIEW = (1 << 30) + 29,
    DAXA_RESULT_FAILED_TO_CREATE_SAMPLER = (1 << 30) + 30,
    DAXA_RESULT_FAILED_TO_CREATE_BDA_BUFFER = (1 << 30) + 31,
    DAXA_RESULT_FAILED_TO_SUBMIT_DEVICE_INIT_COMMANDS = (1 << 30) + 32,
    DAXA_RESULT_INVALID_BUFFER_RANGE = (1 << 30) + 33,
    DAXA_RESULT_INVALID_BUFFER_OFFSET = (1 << 30) + 34,
    DAXA_RESULT_NO_SUITABLE_FORMAT_FOUND = (1 << 30) + 36,
    DAXA_RESULT_RANGE_OUT_OF_BOUNDS = (1 << 30) + 37,
    DAXA_RESULT_NO_SUITABLE_DEVICE_FOUND = (1 << 30) + 38,
    DAXA_RESULT_EXCEEDED_MAX_BUFFERS = (1 << 30) + 39,
    DAXA_RESULT_EXCEEDED_MAX_IMAGES = (1 << 30) + 40,
    DAXA_RESULT_EXCEEDED_MAX_IMAGE_VIEWS = (1 << 30) + 41,
    DAXA_RESULT_EXCEEDED_MAX_SAMPLERS = (1 << 30) + 42,
    DAXA_RESULT_DEVICE_SURFACE_UNSUPPORTED_PRESENT_MODE = (1 << 30) + 43,
    DAXA_RESULT_COMMAND_REFERENCES_INVALID_BUFFER_ID = (1 << 30) + 44,
    DAXA_RESULT_COMMAND_REFERENCES_INVALID_IMAGE_ID = (1 << 30) + 45,
    DAXA_RESULT_COMMAND_REFERENCES_INVALID_IMAGE_VIEW_ID = (1 << 30) + 46,
    DAXA_RESULT_COMMAND_REFERENCES_INVALID_SAMPLER_ID = (1 << 30) + 47,
    DAXA_RESULT_INVALID_ACCELERATION_STRUCTURE_ID = (1 << 30) + 48,
    DAXA_RESULT_EXCEEDED_MAX_ACCELERATION_STRUCTURES = (1 << 30) + 49,
    DAXA_RESULT_DEVICE_DOES_NOT_SUPPORT_RAYTRACING = (1 << 30) + 50,
    DAXA_RESULT_DEVICE_DOES_NOT_SUPPORT_MESH_SHADER = (1 << 30) + 51,
    DAXA_RESULT_INVALID_TLAS_ID = (1 << 30) + 52,
    DAXA_RESULT_INVALID_BLAS_ID = (1 << 30) + 53,
    DAXA_RESULT_INVALID_WITHOUT_ENABLING_RAY_TRACING = (1 << 30) + 54,
    DAXA_RESULT_NO_COMPUTE_PIPELINE_BOUND = (1 << 30) + 55,
    DAXA_RESULT_NO_RASTER_PIPELINE_BOUND = (1 << 30) + 56,
    DAXA_RESULT_NO_RAYTRACING_PIPELINE_BOUND = (1 << 30) + 57,
    DAXA_RESULT_NO_PIPELINE_BOUND = (1 << 30) + 58,
    DAXA_RESULT_PUSHCONSTANT_RANGE_EXCEEDED = (1 << 30) + 59,
    DAXA_RESULT_MESH_SHADER_NOT_DEVICE_ENABLED = (1 << 30) + 60,
    DAXA_RESULT_ERROR_COPY_OUT_OF_BOUNDS = (1 << 30) + 61,
    DAXA_RESULT_ERROR_NO_GRAPHICS_QUEUE_FOUND = (1 << 30) + 62,
    DAXA_RESULT_MAX_ENUM = 0x7FFFFFFF,
} daxa_Result;

// ImageLayout matches vulkan's image layouts
typedef enum
{
    DAXA_IMAGE_LAYOUT_UNDEFINED = 0,
    DAXA_IMAGE_LAYOUT_GENERAL = 1,
    DAXA_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL = 6,
    DAXA_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL = 7,
    DAXA_IMAGE_LAYOUT_READ_ONLY_OPTIMAL = 1000314000,
    DAXA_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL = 1000314001,
    DAXA_IMAGE_LAYOUT_PRESENT_SRC = 1000001002,
    DAXA_IMAGE_LAYOUT_MAX_ENUM = 0x7FFFFFFF,
} daxa_ImageLayout;

/// ABI STABLE OPTIONAL TYPE.
/// THIS TYPE MUST STAY IN SYNC WITH daxa::Optional
#define daxa_Optional(T)      \
    struct                    \
    {                         \
        T value;              \
        daxa_Bool8 has_value; \
    }

#define _DAXA_FIXED_LIST_SIZE_T uint8_t

/// ABI STABLE FIXED LIST TYPE.
/// THIS TYPE MUST STAY IN SYNC WITH daxa::FixedList
#define daxa_FixedList(T, CAPACITY)   \
    struct                            \
    {                                 \
        T data[CAPACITY];             \
        _DAXA_FIXED_LIST_SIZE_T size; \
    }

#define daxa_SpanToConst(T) \
    struct                  \
    {                       \
        T const * data;     \
        size_t size;        \
    }

#define _DAXA_VARIANT_INDEX_TYPE uint8_t

/// ABI STABLE VARIANT TYPE.
/// THIS TYPE MUST STAY IN SYNC WITH daxa::Variant
#define daxa_Variant(UNION)             \
    struct                              \
    {                                   \
        UNION values;                   \
        _DAXA_VARIANT_INDEX_TYPE index; \
    }

typedef daxa_FixedList(char, DAXA_SMALL_STRING_CAPACITY) daxa_SmallString;

typedef struct
{
    uint32_t base_mip_level;
    uint32_t level_count;
    uint32_t base_array_layer;
    uint32_t layer_count;
} daxa_ImageMipArraySlice;

typedef struct
{
    uint32_t mip_level;
    uint32_t base_array_layer;
    uint32_t layer_count;
} daxa_ImageArraySlice;

typedef struct
{
    uint32_t mip_level;
    uint32_t array_layer;
} daxa_ImageSlice;

typedef uint32_t daxa_MemoryFlags;
static daxa_MemoryFlags const DAXA_MEMORY_FLAG_NONE = 0x00000000;
static daxa_MemoryFlags const DAXA_MEMORY_FLAG_DEDICATED_MEMORY = 0x00000001;
static daxa_MemoryFlags const DAXA_MEMORY_FLAG_CAN_ALIAS = 0x00000200;
static daxa_MemoryFlags const DAXA_MEMORY_FLAG_HOST_ACCESS_SEQUENTIAL_WRITE = 0x00000400;
static daxa_MemoryFlags const DAXA_MEMORY_FLAG_HOST_ACCESS_RANDOM = 0x00000800;
static daxa_MemoryFlags const DAXA_MEMORY_FLAG_STRATEGY_MIN_MEMORY = 0x00010000;
static daxa_MemoryFlags const DAXA_MEMORY_FLAG_STRATEGY_MIN_TIME = 0x00020000;

typedef struct
{
    VkMemoryRequirements requirements;
    daxa_MemoryFlags flags;
} daxa_MemoryBlockInfo;

DAXA_EXPORT daxa_MemoryBlockInfo const *
daxa_memory_block_info(daxa_MemoryBlock memory_block);

DAXA_EXPORT uint64_t
daxa_memory_block_inc_refcnt(daxa_MemoryBlock memory_block);
DAXA_EXPORT uint64_t
daxa_memory_block_dec_refcnt(daxa_MemoryBlock memory_block);

typedef struct
{
    uint32_t query_count;
    daxa_SmallString name;
} daxa_TimelineQueryPoolInfo;

DAXA_EXPORT daxa_TimelineQueryPoolInfo const *
daxa_timeline_query_pool_info(daxa_TimelineQueryPool timeline_query_pool);

DAXA_EXPORT DAXA_NO_DISCARD daxa_Result
daxa_timeline_query_pool_query_results(daxa_TimelineQueryPool timeline_query_pool, uint32_t start, uint32_t count, uint64_t * out_results);

DAXA_EXPORT uint64_t
daxa_timeline_query_pool_inc_refcnt(daxa_TimelineQueryPool timeline_query_pool);
DAXA_EXPORT uint64_t
daxa_timeline_query_pool_dec_refcnt(daxa_TimelineQueryPool timeline_query_pool);

#endif // #ifndef __DAXA_TYPES_H__
