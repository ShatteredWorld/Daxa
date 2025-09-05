#ifndef __DAXA_UTILS_PIPELINE_MANAGER_H__
#define __DAXA_UTILS_PIPELINE_MANAGER_H__

#if !DAXA_BUILT_WITH_UTILS_PIPELINE_MANAGER_GLSLANG && !DAXA_BUILT_WITH_UTILS_PIPELINE_MANAGER_SLANG
#error "[package management error] You must build Daxa with the DAXA_ENABLE_UTILS_PIPELINE_MANAGER_(GLSLANG|SLANG) CMake option enabled, or request the utils-pipeline-manager-(glslang|slang) feature in vcpkg"
#endif

#include <daxa/c/daxa.h>

typedef struct daxa_PipelineManagerImpl* daxa_PipelineManager;

typedef daxa_SharedPtr(daxa_RayTracingPipeline) daxa_RayTracingPipelineSharedPtr;
typedef daxa_SharedPtr(daxa_ComputePipeline) daxa_ComputePipelineSharedPtr;
typedef daxa_SharedPtr(daxa_RasterPipeline) daxa_RasterPipelineSharedPtr;

typedef struct
{
    const char* path;
} daxa_ShaderFile;

typedef struct
{
    const char* string;
} daxa_ShaderCode;

typedef union
{
    daxa_Monostate mono;
    daxa_ShaderFile file;
    daxa_ShaderCode code;
} daxa_ShaderSourceUnion;

typedef daxa_Variant(daxa_ShaderSourceUnion) daxa_ShaderSource;

typedef struct
{
    const char* name;
    const char* value;
} daxa_ShaderDefine;

typedef enum
{
    DAXA_SHADER_LANGUAGE_GLSL = 0,
    DAXA_SHADER_LANGUAGE_SLANG = 1,
    DAXA_SHADER_LANGUAGE_MAX_ENUM = 0x7fffffff,
} daxa_ShaderLanguage;

typedef struct
{
    daxa_u32 major;
    daxa_u32 minor;
} daxa_ShaderModel;

typedef struct
{
    daxa_ShaderSource source;
    daxa_Optional(const char*) entry_point;
    daxa_Optional(daxa_ShaderLanguage) language;
    daxa_Span(daxa_ShaderDefine) defines;
    daxa_Optional(daxa_Bool8) enable_debug_info;
    daxa_Optional(VkPipelineShaderStageCreateFlags) create_flags;
    daxa_Optional(daxa_u32) required_subgroup_size;
} daxa_ShaderCompileInfo2;

typedef struct
{
    daxa_Span(daxa_ShaderCompileInfo2) ray_gen_infos;
    daxa_Span(daxa_ShaderCompileInfo2) intersection_infos;
    daxa_Span(daxa_ShaderCompileInfo2) any_hit_infos;
    daxa_Span(daxa_ShaderCompileInfo2) callable_infos;
    daxa_Span(daxa_ShaderCompileInfo2) closest_hit_infos;
    daxa_Span(daxa_ShaderCompileInfo2) miss_hit_infos;
    daxa_SpanToConst(daxa_RayTracingShaderGroupInfo) shader_groups_infos;
    daxa_u32 max_ray_recursion_depth;
    daxa_u32 push_constant_size;
    const char* name;
} daxa_RayTracingPipelineCompileInfo2;

typedef struct
{
    daxa_ShaderSource source;
    daxa_Optional(const char*) entry_point;
    daxa_Optional(daxa_ShaderLanguage) language;
    daxa_Span(daxa_ShaderDefine) defines;
    daxa_Optional(daxa_Bool8) enable_debug_info;
    daxa_Optional(VkPipelineShaderStageCreateFlags) create_flags;
    daxa_Optional(daxa_u32) required_subgroup_size;
    daxa_u32 push_constant_size;
    const char* name;
} daxa_ComputePipelineCompileInfo2;

typedef struct
{
    daxa_Optional(daxa_ShaderCompileInfo2) mesh_shader_info;
    daxa_Optional(daxa_ShaderCompileInfo2) vertex_shader_info;
    daxa_Optional(daxa_ShaderCompileInfo2) tesselation_control_shader_info;
    daxa_Optional(daxa_ShaderCompileInfo2) tesselation_evaluation_shader_info;
    daxa_Optional(daxa_ShaderCompileInfo2) fragment_shader_info;
    daxa_Optional(daxa_ShaderCompileInfo2) task_shader_info;
    daxa_Span(daxa_RenderAttachment) color_attachments;
    daxa_Optional(daxa_DepthTestInfo) depth_test;
    daxa_RasterizerInfo raster;
    daxa_TesselationInfo tesselation;
    daxa_u32 push_constant_size;
    const char* name;
} daxa_RasterPipelineCompileInfo2;

typedef struct
{
    daxa_Device device;
    daxa_Span(const char*) root_paths;
    daxa_Optional(const char*) write_out_preprocessed_code;
    daxa_Optional(const char*) write_out_shader_binary;
    daxa_Optional(const char*) spirv_cache_folder;
    daxa_Bool8 register_null_pipelines_when_first_compile_fails;
    void (*custom_preprocessor)(const char** code, const char* path);
    daxa_Optional(const char*) default_entry_point;
    daxa_Optional(daxa_ShaderLanguage) default_language;
    daxa_Span(daxa_ShaderDefine) default_defines;
    daxa_Optional(daxa_Bool8) default_enable_debug_info;
    daxa_Optional(VkPipelineShaderStageCreateFlags) default_create_flags;
    daxa_Optional(daxa_u32) default_required_subgroup_size;
    const char* name;
} daxa_PipelineManagerInfo2;

typedef struct
{
    const char* name;
    const char* contents;
} daxa_VirtualFileInfo;

typedef struct
{
    char padding;
} daxa_PipelineReloadSuccess;

typedef struct
{
    const char* message;
} daxa_PipelineReloadError;

typedef daxa_Monostate daxa_NoPipelineChanged;

typedef union
{
    daxa_NoPipelineChanged no_change;
    daxa_PipelineReloadSuccess success;
    daxa_PipelineReloadError error;
} daxa_PipelineReloadResultUnion;

typedef daxa_Variant(daxa_PipelineReloadResultUnion) daxa_PipelineReloadResult;

DAXA_EXPORT void daxa_create_pipeline_manager(daxa_PipelineManagerInfo2 const* info, daxa_PipelineManager* out);
DAXA_EXPORT void daxa_destroy_pipeline_manager(daxa_PipelineManager pipeline_manager);

DAXA_EXPORT DAXA_NO_DISCARD daxa_Bool8
daxa_pipeline_manager_add_ray_tracing_pipeline(daxa_PipelineManager pipeline_manager, daxa_RayTracingPipelineCompileInfo2 const* info, daxa_RayTracingPipelineSharedPtr* out);

DAXA_EXPORT DAXA_NO_DISCARD daxa_Bool8
daxa_pipeline_manager_add_compute_pipeline(daxa_PipelineManager pipeline_manager, daxa_ComputePipelineCompileInfo2 const* info, daxa_ComputePipelineSharedPtr* out);

DAXA_EXPORT DAXA_NO_DISCARD daxa_Bool8
daxa_pipeline_manager_add_raster_pipeline(daxa_PipelineManager pipeline_manager, daxa_RasterPipelineCompileInfo2 const* info, daxa_RasterPipelineSharedPtr* out);

DAXA_EXPORT void daxa_pipeline_manager_remove_ray_tracing_pipeline(daxa_PipelineManager pipeline_manager, daxa_RayTracingPipelineSharedPtr* pipeline);
DAXA_EXPORT void daxa_pipeline_manager_remove_compute_pipeline(daxa_PipelineManager pipeline_manager, daxa_ComputePipelineSharedPtr* pipeline);
DAXA_EXPORT void daxa_pipeline_manager_remove_raster_pipeline(daxa_PipelineManager pipeline_manager, daxa_RasterPipelineSharedPtr* pipeline);

DAXA_EXPORT void daxa_pipeline_manager_get_latest_error(daxa_PipelineManager pipeline_manager, const char** out);

DAXA_EXPORT void daxa_pipeline_manager_add_virtual_file(daxa_PipelineManager pipeline_manager, daxa_VirtualFileInfo const* virtual_info);

DAXA_EXPORT DAXA_NO_DISCARD daxa_PipelineReloadResult
daxa_pipeline_manager_reload_all(daxa_PipelineManager pipeline_manager);

DAXA_EXPORT DAXA_NO_DISCARD daxa_Bool8
daxa_pipeline_manager_all_pipelines_valid(daxa_PipelineManager pipeline_manager);

#endif
