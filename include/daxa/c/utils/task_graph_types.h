#ifndef __DAXA_UTILS_TASK_GRAPH_TYPES_H__
#define __DAXA_UTILS_TASK_GRAPH_TYPES_H__

#include "daxa/c/types.h"
#include <daxa/c/daxa.h>
#include <daxa/c/utils/mem.h>

#if !DAXA_BUILT_WITH_UTILS_TASK_GRAPH
#error "[package management error] You must build Daxa with the DAXA_ENABLE_UTILS_TASK_GRAPH CMake option enabled, or request the utils-task-graph feature in vcpkg"
#endif
/*
#define DAXA_MAX_TASK_ATTACHMENTS 48

typedef enum
{
    DAXA_TASK_ATTACHMENT_TYPE_UNDEFINED,
    DAXA_TASK_ATTACHMENT_TYPE_BUFFER,
    DAXA_TASK_ATTACHMENT_TYPE_BLAS,
    DAXA_TASK_ATTACHMENT_TYPE_TLAS,
    DAXA_TASK_ATTACHMENT_TYPE_IMAGE
} daxa_TaskAttachmentType;

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

typedef enum
{
    DAXA_TASK_ACCESS_TYPE_NONE = 0,
    DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT = 1,
    DAXA_TASK_ACCESS_TYPE_READ = (1 << 1) | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT,
    DAXA_TASK_ACCESS_TYPE_SAMPLED = 1 << 2 | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT,
    DAXA_TASK_ACCESS_TYPE_WRITE = 1 << 3,
    DAXA_TASK_ACCESS_TYPE_READ_WRITE = (1 << 1) | (1 << 3),
    DAXA_TASK_ACCESS_TYPE_WRITE_CONCURRENT = DAXA_TASK_ACCESS_TYPE_WRITE | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT,
    DAXA_TASK_ACCESS_TYPE_READ_WRITE_CONCURRENT = DAXA_TASK_ACCESS_TYPE_READ_WRITE | DAXA_TASK_ACCESS_TYPE_CONCURRENT_BIT,
} daxa_TaskAccessType;

typedef enum
{
    DAXA_TASK_STAGE_NONE,
    DAXA_TASK_STAGE_VERTEX_SHADER,
    DAXA_TASK_STAGE_TESSELLATION_CONTROL_SHADER,
    DAXA_TASK_STAGE_TESSELLATION_EVALUATION_SHADER,
    DAXA_TASK_STAGE_GEOMETRY_SHADER,
    DAXA_TASK_STAGE_FRAGMENT_SHADER,
    DAXA_TASK_STAGE_TASK_SHADER,
    DAXA_TASK_STAGE_MESH_SHADER,
    DAXA_TASK_STAGE_PRE_RASTERIZATION_SHADERS,
    DAXA_TASK_STAGE_RASTER_SHADER,
    DAXA_TASK_STAGE_COMPUTE_SHADER,
    DAXA_TASK_STAGE_RAY_TRACING_SHADER,
    DAXA_TASK_STAGE_SHADER,
    DAXA_TASK_STAGE_COLOR_ATTACHMENT,
    DAXA_TASK_STAGE_DEPTH_STENCIL_ATTACHMENT,
    DAXA_TASK_STAGE_RESOLVE,
    DAXA_TASK_STAGE_PRESENT,
    DAXA_TASK_STAGE_INDIRECT_COMMAND,
    DAXA_TASK_STAGE_INDEX_INPUT,
    DAXA_TASK_STAGE_TRANSFER,
    DAXA_TASK_STAGE_HOST,
    DAXA_TASK_STAGE_AS_BUILD,
    DAXA_TASK_STAGE_ANY_COMMAND,
} daxa_TaskStage;

typedef struct
{
    daxa_TaskStage stage;
    daxa_TaskAccessType type;
    daxa_TaskAttachmentType restricition;
} daxa_TaskAccess;

#define DEFINE_DAXA_TASK_ACCESS_CONSTS_RESTRICTED(NAME, STAGE, ATTACHMENT_TYPE_RESTRICTION)              \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_NONE = {0};                                            \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_READ = {STAGE, DAXA_TASK_ACCESS_TYPE_READ, ATTACHMENT_TYPE_RESTRICTION};                \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_WRITE = {STAGE, DAXA_TASK_ACCESS_TYPE_WRITE, ATTACHMENT_TYPE_RESTRICTION};              \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_WRITE_CONCURRENT = {STAGE, DAXA_TASK_ACCESS_TYPE_WRITE_CONCURRENT, ATTACHMENT_TYPE_RESTRICTION}; \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_READ_WRITE = {STAGE, DAXA_TASK_ACCESS_TYPE_READ_WRITE, ATTACHMENT_TYPE_RESTRICTION};    \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_READ_WRITE_CONCURRENT = {STAGE, DAXA_TASK_ACCESS_TYPE_READ_WRITE_CONCURRENT, ATTACHMENT_TYPE_RESTRICTION}; \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_SAMPLED = {STAGE, DAXA_TASK_ACCESS_TYPE_SAMPLED, DAXA_TASK_ATTACHMENT_TYPE_IMAGE};           \
                                                                                         \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_R = DAXA_ACCESS_CONST_##NAME##_READ;                                      \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_W = DAXA_ACCESS_CONST_##NAME##_WRITE;                                     \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_WC = DAXA_ACCESS_CONST_##NAME##_WRITE_CONCURRENT;                         \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_RW = DAXA_ACCESS_CONST_##NAME##_READ_WRITE_CONCURRENT;                    \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_RWC = DAXA_ACCESS_CONST_##NAME##_READ_WRITE_CONCURRENT;                   \
    static const daxa_TaskAccess DAXA_ACCESS_CONST_##NAME##_S = DAXA_ACCESS_CONST_##NAME##_SAMPLED;

#define DEFINE_DAXA_TASK_ACCESS_CONSTS(NAME, STAGE) DEFINE_DAXA_TASK_ACCESS_CONSTS_RESTRICTED(NAME, STAGE, DAXA_TASK_ATTACHMENT_TYPE_UNDEFINED)

DEFINE_DAXA_TASK_ACCESS_CONSTS(VERTEX_SHADER, DAXA_TASK_STAGE_VERTEX_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(TESSELLATION_CONTROL_SHADER, DAXA_TASK_STAGE_TESSELLATION_CONTROL_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(TESSELLATION_EVALUATION_SHADER, DAXA_TASK_STAGE_TESSELLATION_EVALUATION_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(GEOMETRY_SHADER, DAXA_TASK_STAGE_GEOMETRY_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(FRAGMENT_SHADER, DAXA_TASK_STAGE_FRAGMENT_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(COMPUTE_SHADER, DAXA_TASK_STAGE_COMPUTE_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(RAY_TRACING_SHADER, DAXA_TASK_STAGE_RAY_TRACING_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(TASK_SHADER, DAXA_TASK_STAGE_TASK_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(MESH_SHADER, DAXA_TASK_STAGE_MESH_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(PRE_RASTERIZATION_SHADERS, DAXA_TASK_STAGE_PRE_RASTERIZATION_SHADERS)
DEFINE_DAXA_TASK_ACCESS_CONSTS(RASTER_SHADER, DAXA_TASK_STAGE_RASTER_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(SHADER, DAXA_TASK_STAGE_SHADER)
DEFINE_DAXA_TASK_ACCESS_CONSTS_RESTRICTED(DEPTH_STENCIL_ATTACHMENT, DAXA_TASK_STAGE_DEPTH_STENCIL_ATTACHMENT, DAXA_TASK_ATTACHMENT_TYPE_IMAGE)
DEFINE_DAXA_TASK_ACCESS_CONSTS_RESTRICTED(RESOLVE, DAXA_TASK_STAGE_RESOLVE, DAXA_TASK_ATTACHMENT_TYPE_IMAGE)
DEFINE_DAXA_TASK_ACCESS_CONSTS(TRANSFER, DAXA_TASK_STAGE_TRANSFER)
DEFINE_DAXA_TASK_ACCESS_CONSTS(HOST, DAXA_TASK_STAGE_HOST)
DEFINE_DAXA_TASK_ACCESS_CONSTS(ACCELERATION_STRUCTURE_BUILD, DAXA_TASK_STAGE_AS_BUILD)
DEFINE_DAXA_TASK_ACCESS_CONSTS(ANY_COMMAND, DAXA_TASK_STAGE_ANY_COMMAND)

static const daxa_TaskAccess DAXA_ACCESS_CONST_NONE = {DAXA_TASK_STAGE_NONE, DAXA_TASK_ACCESS_TYPE_NONE};
static const daxa_TaskAccess DAXA_ACCESS_CONST_READ = {DAXA_TASK_STAGE_NONE, DAXA_TASK_ACCESS_TYPE_READ};
static const daxa_TaskAccess DAXA_ACCESS_CONST_WRITE = {DAXA_TASK_STAGE_NONE, DAXA_TASK_ACCESS_TYPE_WRITE};
static const daxa_TaskAccess DAXA_ACCESS_CONST_WRITE_CONCURRENT = {DAXA_TASK_STAGE_NONE, DAXA_TASK_ACCESS_TYPE_WRITE_CONCURRENT};
static const daxa_TaskAccess DAXA_ACCESS_CONST_READ_WRITE = {DAXA_TASK_STAGE_NONE, DAXA_TASK_ACCESS_TYPE_READ_WRITE};
static const daxa_TaskAccess DAXA_ACCESS_CONST_READ_WRITE_CONCURRENT = {DAXA_TASK_STAGE_NONE, DAXA_TASK_ACCESS_TYPE_READ_WRITE_CONCURRENT};
static const daxa_TaskAccess DAXA_ACCESS_CONST_SAMPLED = {DAXA_TASK_STAGE_NONE, DAXA_TASK_ACCESS_TYPE_SAMPLED, DAXA_TASK_ATTACHMENT_TYPE_IMAGE};

static const daxa_TaskAccess DAXA_ACCESS_CONST_COLOR_ATTACHMENT = {DAXA_TASK_STAGE_COLOR_ATTACHMENT, DAXA_TASK_ACCESS_TYPE_READ_WRITE, DAXA_TASK_ATTACHMENT_TYPE_IMAGE};

static const daxa_TaskAccess DAXA_ACCESS_CONST_DEPTH_ATTACHMENT = DAXA_ACCESS_CONST_DEPTH_STENCIL_ATTACHMENT_READ_WRITE;
static const daxa_TaskAccess DAXA_ACCESS_CONST_STENCIL_ATTACHMENT = DAXA_ACCESS_CONST_DEPTH_STENCIL_ATTACHMENT_READ_WRITE;
static const daxa_TaskAccess DAXA_ACCESS_CONST_DEPTH_ATTACHMENT_READ = DAXA_ACCESS_CONST_DEPTH_STENCIL_ATTACHMENT_SAMPLED;
static const daxa_TaskAccess DAXA_ACCESS_CONST_STENCIL_ATTACHMENT_READ = DAXA_ACCESS_CONST_DEPTH_STENCIL_ATTACHMENT_SAMPLED;

static const daxa_TaskAccess DAXA_ACCESS_CONST_PRESENT = {DAXA_TASK_STAGE_PRESENT, DAXA_TASK_ACCESS_TYPE_READ, DAXA_TASK_ATTACHMENT_TYPE_IMAGE};
static const daxa_TaskAccess DAXA_ACCESS_CONST_INDIRECT_COMMAND_READ = {DAXA_TASK_STAGE_INDIRECT_COMMAND, DAXA_TASK_ACCESS_TYPE_READ, DAXA_TASK_ATTACHMENT_TYPE_BUFFER};
static const daxa_TaskAccess DAXA_ACCESS_CONST_INDEX_INPUT_READ = {DAXA_TASK_STAGE_INDEX_INPUT, DAXA_TASK_ACCESS_TYPE_READ, DAXA_TASK_ATTACHMENT_TYPE_BUFFER};

typedef enum
{
    DAXA_TASK_TYPE_UNDEFINED,
    DAXA_TASK_TYPE_GENERAL,
    DAXA_TASK_TYPE_RASTER,
    DAXA_TASK_TYPE_COMPUTE,
    DAXA_TASK_TYPE_RAY_TRACING,
    DAXA_TASK_TYPE_TRANSFER
} daxa_TaskType;

typedef daxa_u32 daxa_TaskResourceIndex;

typedef struct {} daxa_UndefinedAttachment;

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
    //TaskHeadImageArrayType shader_array_type;
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
} daxa_TaskAttachmentInfo;

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
*/
#endif
