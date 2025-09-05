#if DAXA_BUILT_WITH_UTILS_PIPELINE_MANAGER_GLSLANG || DAXA_BUILT_WITH_UTILS_PIPELINE_MANAGER_SLANG

#include "../impl_core.hpp"
#include <daxa/utils/pipeline_manager.hpp>
#include <daxa/c/utils/pipeline_manager.h>

struct daxa_PipelineManagerImpl
{
    daxa::PipelineManager impl;
    std::string latest_error;
};

auto create_shader_compile_info(daxa_ShaderCompileInfo2 const *info) -> daxa::ShaderCompileInfo2
{
    daxa::ShaderSource source = daxa::Monostate{};
    if(info->source.index == 1)
    {
        source = daxa::ShaderFile{.path = info->source.values.file.path};
    }
    else if(info->source.index == 2)
    {
        source = daxa::ShaderCode{.string = info->source.values.code.string};
    }

    std::vector<daxa::ShaderDefine> defines(info->defines.size);
    for(size_t i = 0; i < defines.size(); i++)
    {
        defines[i] = {.name = info->defines.data[i].name, .value = info->defines.data[i].value};
    }
    return daxa::ShaderCompileInfo2{
        .source = source,
        .entry_point = (info->entry_point.has_value != 0) ? std::optional<std::string>{info->entry_point.value} : std::nullopt,
        .language = (info->language.has_value != 0) ? std::optional<daxa::ShaderLanguage>{static_cast<daxa::ShaderLanguage>(info->language.value)} : std::nullopt,
        .defines = std::move(defines),
        .enable_debug_info = (info->enable_debug_info.has_value != 0) ? std::optional<bool>{info->enable_debug_info.value != 0} : std::nullopt,
        .create_flags = (info->create_flags.has_value != 0) ? std::optional<daxa::ShaderCreateFlags>{static_cast<daxa::ShaderCreateFlags>(info->create_flags.value)} : std::nullopt,
        .required_subgroup_size = (info->required_subgroup_size.has_value != 0) ? std::optional<daxa::u32>{info->required_subgroup_size.value} : std::nullopt,
    };
}

auto daxa_create_pipeline_manager(const daxa_PipelineManagerInfo2 *info, daxa_PipelineManager *out) -> void
{
    if(info == nullptr || out == nullptr)
    {
        return;
    }

    std::vector<std::filesystem::path> root_paths{info->root_paths.size};
    for(size_t i = 0; i < root_paths.size(); i++)
    {
        root_paths[i] = info->root_paths.data[i];
    }

    std::vector<ShaderDefine> default_defines{info->default_defines.size};
    for(size_t i = 0; i < default_defines.size(); i++)
    {
        default_defines[i] = {.name = info->default_defines.data[i].name, .value = info->default_defines.data[i].value};
    }

    daxa::PipelineManagerInfo2 cpp_info = {
        .device = *rc_cast<daxa::Device*>(&info->device),
        .root_paths = std::move(root_paths),
        .write_out_preprocessed_code = (info->write_out_preprocessed_code.has_value != 0) ? std::optional<std::filesystem::path>{info->write_out_preprocessed_code.value} : std::nullopt,
        .write_out_spirv = (info->write_out_shader_binary.has_value != 0) ? std::optional<std::filesystem::path>{info->write_out_shader_binary.value} : std::nullopt,
        .spirv_cache_folder = (info->spirv_cache_folder.has_value != 0) ? std::optional<std::filesystem::path>{info->spirv_cache_folder.value} : std::nullopt,
        .register_null_pipelines_when_first_compile_fails = info->register_null_pipelines_when_first_compile_fails != 0,
        .custom_preprocessor = [custom_pre = info->custom_preprocessor](std::string & code, std::filesystem::path const & path)
        {
            if(custom_pre != nullptr)
            {
                const char* code_cstr = code.c_str();
                custom_pre(&code_cstr, path.string().c_str());
                code = code_cstr;
            }
        },
        .default_entry_point = (info->default_entry_point.has_value != 0) ? std::optional<std::string>{info->default_entry_point.value} : std::nullopt,
        .default_language = (info->default_language.has_value != 0) ? std::optional<daxa::ShaderLanguage>{static_cast<daxa::ShaderLanguage>(info->default_language.value)} : std::nullopt,
        .default_defines = std::move(default_defines),
        .default_enable_debug_info = (info->default_enable_debug_info.has_value != 0) ? std::optional<bool>{info->default_enable_debug_info.value} : std::nullopt,
        .default_create_flags = (info->default_create_flags.has_value != 0) ? std::optional<daxa::ShaderCreateFlags>{static_cast<daxa::ShaderCreateFlags>(info->default_create_flags.value)} : std::nullopt,
        .default_required_subgroup_size = (info->default_required_subgroup_size.has_value != 0) ? std::optional<daxa::u32>{info->default_required_subgroup_size.value} : std::nullopt,
        .name = info->name != nullptr ? info->name : "",
    };

    (*out) = new daxa_PipelineManagerImpl();
    (*out)->impl = {cpp_info};

    //daxa_dvc_inc_refcnt(info->device);
}

auto daxa_destroy_pipeline_manager(daxa_PipelineManager pipeline_manager) -> void
{
    delete pipeline_manager;
}

auto daxa_pipeline_manager_add_ray_tracing_pipeline(daxa_PipelineManager pipeline_manager, const daxa_RayTracingPipelineCompileInfo2 *info, daxa_RayTracingPipelineSharedPtr *out) -> daxa_Bool8
{
    if(pipeline_manager == nullptr || info == nullptr || out == nullptr)
    {
        return 0;
    }

    std::vector<daxa::ShaderCompileInfo2> ray_gen_infos{info->ray_gen_infos.size};
    for(size_t i = 0; i < ray_gen_infos.size(); i++)
    {
        ray_gen_infos[i] = create_shader_compile_info(&info->ray_gen_infos.data[i]);
    }

    std::vector<daxa::ShaderCompileInfo2> intersection_shaders{info->intersection_infos.size};
    for(size_t i = 0; i < intersection_shaders.size(); i++)
    {
        intersection_shaders[i] = create_shader_compile_info(&info->intersection_infos.data[i]);
    }

    std::vector<daxa::ShaderCompileInfo2> any_hit_shaders{info->any_hit_infos.size};
    for(size_t i = 0; i < any_hit_shaders.size(); i++)
    {
        any_hit_shaders[i] = create_shader_compile_info(&info->any_hit_infos.data[i]);
    }

    std::vector<daxa::ShaderCompileInfo2> callable_shaders{info->callable_infos.size};
    for(size_t i = 0; i < callable_shaders.size(); i++)
    {
        callable_shaders[i] = create_shader_compile_info(&info->callable_infos.data[i]);
    }

    std::vector<daxa::ShaderCompileInfo2> closest_hit_shaders{info->closest_hit_infos.size};
    for(size_t i = 0; i < closest_hit_shaders.size(); i++)
    {
        closest_hit_shaders[i] = create_shader_compile_info(&info->closest_hit_infos.data[i]);
    }

    std::vector<daxa::ShaderCompileInfo2> miss_hit_shaders{info->miss_hit_infos.size};
    for(size_t i = 0; i < miss_hit_shaders.size(); i++)
    {
        miss_hit_shaders[i] = create_shader_compile_info(&info->miss_hit_infos.data[i]);
    }

    std::vector<daxa::RayTracingShaderGroupInfo> shader_groups_infos{info->shader_groups_infos.size};
    for(size_t i = 0; i < shader_groups_infos.size(); i++)
    {
        shader_groups_infos[i] = std::bit_cast<daxa::RayTracingShaderGroupInfo>(info->shader_groups_infos.data[i]);
    }

    daxa::RayTracingPipelineCompileInfo2 cpp_info = {
        .ray_gen_infos = std::move(ray_gen_infos),
        .intersection_infos = std::move(intersection_shaders),
        .any_hit_infos = std::move(any_hit_shaders),
        .callable_infos = std::move(callable_shaders),
        .closest_hit_infos = std::move(closest_hit_shaders),
        .miss_hit_infos = std::move(miss_hit_shaders),
        .shader_groups_infos = std::move(shader_groups_infos),
        .max_ray_recursion_depth = info->max_ray_recursion_depth,
        .push_constant_size = info->push_constant_size,
        .name = info->name != nullptr ? info->name : "",
    };

    auto result = pipeline_manager->impl.add_ray_tracing_pipeline2(cpp_info);
    pipeline_manager->latest_error = "";
    if(result.is_err())
    {
        pipeline_manager->latest_error = result.message();
        return 0;
    }
    else
    {
        //(*out) = (result.value())->get();
        (*out) = *r_cast<daxa_RayTracingPipelineSharedPtr*>(&result.value());
        return 1;
    }
}

auto daxa_pipeline_manager_add_compute_pipeline(daxa_PipelineManager pipeline_manager, const daxa_ComputePipelineCompileInfo2 *info, daxa_ComputePipelineSharedPtr *out) -> daxa_Bool8
{
    if(pipeline_manager == nullptr || info == nullptr || out == nullptr)
    {
        return 0;
    }

    daxa::ShaderSource source = daxa::Monostate{};
    if(info->source.index == 1)
    {
        source = daxa::ShaderFile{.path = info->source.values.file.path};
    }
    else if(info->source.index == 2)
    {
        source = daxa::ShaderCode{.string = info->source.values.code.string};
    }

    std::vector<daxa::ShaderDefine> defines(info->defines.size);
    for(size_t i = 0; i < defines.size(); i++)
    {
        defines[i] = {.name = info->defines.data[i].name, .value = info->defines.data[i].value};
    }

    daxa::ComputePipelineCompileInfo2 cpp_info = {
        .source = source,
        .entry_point = (info->entry_point.has_value != 0) ? std::optional<std::string>{info->entry_point.value} : std::nullopt,
        .language = (info->language.has_value != 0) ? std::optional<daxa::ShaderLanguage>{static_cast<daxa::ShaderLanguage>(info->language.value)} : std::nullopt,
        .defines = std::move(defines),
        .enable_debug_info = (info->enable_debug_info.has_value != 0) ? std::optional<bool>{info->enable_debug_info.value != 0} : std::nullopt,
        .create_flags = (info->create_flags.has_value != 0) ? std::optional<daxa::ShaderCreateFlags>{static_cast<daxa::ShaderCreateFlags>(info->create_flags.value)} : std::nullopt,
        .required_subgroup_size = (info->required_subgroup_size.has_value != 0) ? std::optional<daxa::u32>{info->required_subgroup_size.value} : std::nullopt,
        .push_constant_size = info->push_constant_size,
        .name = info->name != nullptr ? info->name : "",
    };

    auto result = pipeline_manager->impl.add_compute_pipeline2(cpp_info);
    pipeline_manager->latest_error = "";
    if(result.is_err())
    {
        pipeline_manager->latest_error = result.message();
        return 0;
    }
    else
    {
        //(*out) = (result.value())->get();
        (*out) = *r_cast<daxa_ComputePipelineSharedPtr*>(&result.value());
        return 1;
    }
}

auto daxa_pipeline_manager_add_raster_pipeline(daxa_PipelineManager pipeline_manager, const daxa_RasterPipelineCompileInfo2 *info, daxa_RasterPipelineSharedPtr *out) -> daxa_Bool8
{
    if(pipeline_manager == nullptr || info == nullptr || out == nullptr)
    {
        return 0;
    }

    std::vector<daxa::RenderAttachment> color_attachments(info->color_attachments.size);
    for(size_t i = 0; i < color_attachments.size(); i++)
    {
        color_attachments[i] = std::bit_cast<daxa::RenderAttachment>(info->color_attachments.data[i]);
    }

    daxa::RasterPipelineCompileInfo2 cpp_info = {
        .mesh_shader_info = (info->mesh_shader_info.has_value != 0) ? daxa::Optional<daxa::ShaderCompileInfo2>{create_shader_compile_info(&info->mesh_shader_info.value)} : daxa::Optional<daxa::ShaderCompileInfo2>{},
        .vertex_shader_info = (info->vertex_shader_info.has_value != 0) ? daxa::Optional<daxa::ShaderCompileInfo2>{create_shader_compile_info(&info->vertex_shader_info.value)} : daxa::Optional<daxa::ShaderCompileInfo2>{},
        .tesselation_control_shader_info = (info->tesselation_control_shader_info.has_value != 0) ? daxa::Optional<daxa::ShaderCompileInfo2>{create_shader_compile_info(&info->tesselation_control_shader_info.value)} : daxa::Optional<daxa::ShaderCompileInfo2>{},
        .tesselation_evaluation_shader_info = (info->tesselation_evaluation_shader_info.has_value != 0) ? daxa::Optional<daxa::ShaderCompileInfo2>{create_shader_compile_info(&info->tesselation_evaluation_shader_info.value)} : daxa::Optional<daxa::ShaderCompileInfo2>{},
        .fragment_shader_info = (info->fragment_shader_info.has_value != 0) ? daxa::Optional<daxa::ShaderCompileInfo2>{create_shader_compile_info(&info->fragment_shader_info.value)} : daxa::Optional<daxa::ShaderCompileInfo2>{},
        .task_shader_info = (info->task_shader_info.has_value != 0) ? daxa::Optional<daxa::ShaderCompileInfo2>{create_shader_compile_info(&info->task_shader_info.value)} : daxa::Optional<daxa::ShaderCompileInfo2>{},
        .color_attachments = std::move(color_attachments),
        .depth_test = (info->depth_test.has_value != 0) ? daxa::Optional<daxa::DepthTestInfo>{std::bit_cast<daxa::DepthTestInfo>(info->depth_test.value)} : daxa::Optional<daxa::DepthTestInfo>{},
        .raster = std::bit_cast<daxa::RasterizerInfo>(info->raster),
        .tesselation = std::bit_cast<daxa::TesselationInfo>(info->tesselation),
        .push_constant_size = info->push_constant_size,
        .name = info->name != nullptr ? info->name : "",
    };

    auto result = pipeline_manager->impl.add_raster_pipeline2(cpp_info);
    pipeline_manager->latest_error = "";
    if(result.is_err())
    {
        pipeline_manager->latest_error = result.message();
        return 0;
    }
    else
    {
        //(*out) = (result.value())->get();
        (*out) = *r_cast<daxa_RasterPipelineSharedPtr*>(&result.value());
        return 1;
    }
}

auto daxa_pipeline_manager_remove_ray_tracing_pipeline(daxa_PipelineManager pipeline_manager, daxa_RayTracingPipelineSharedPtr* pipeline) -> void
{
    if(pipeline_manager == nullptr || pipeline == nullptr)
    {
        return;
    }
    
    pipeline_manager->impl.remove_ray_tracing_pipeline(*r_cast<std::shared_ptr<daxa::RayTracingPipeline>*>(pipeline));
}

auto daxa_pipeline_manager_remove_compute_pipeline(daxa_PipelineManager pipeline_manager, daxa_ComputePipelineSharedPtr* pipeline) -> void
{
    if(pipeline_manager == nullptr || pipeline == nullptr)
    {
        return;
    }
    
    pipeline_manager->impl.remove_compute_pipeline(*r_cast<std::shared_ptr<daxa::ComputePipeline>*>(pipeline));
}

auto daxa_pipeline_manager_remove_raster_pipeline(daxa_PipelineManager pipeline_manager, daxa_RasterPipelineSharedPtr* pipeline) -> void
{
    if(pipeline_manager == nullptr || pipeline == nullptr)
    {
        return;
    }
    
    pipeline_manager->impl.remove_raster_pipeline(*r_cast<std::shared_ptr<daxa::RasterPipeline>*>(pipeline));
}

auto daxa_pipeline_manager_get_latest_error(daxa_PipelineManager pipeline_manager, const char **out) -> void
{
    if(pipeline_manager == nullptr || out == nullptr)
    {
        return;
    }

    (*out) = pipeline_manager->latest_error.c_str();
}

auto daxa_pipeline_manager_add_virtual_file(daxa_PipelineManager pipeline_manager, const daxa_VirtualFileInfo *virtual_info) -> void
{
    if(pipeline_manager == nullptr || virtual_info == nullptr)
    {
        return;
    }

    pipeline_manager->impl.add_virtual_file({.name = virtual_info->name != nullptr ? virtual_info->name : "", .contents = virtual_info->contents != nullptr ? virtual_info->contents : ""});
}

auto daxa_pipeline_manager_reload_all(daxa_PipelineManager pipeline_manager) -> daxa_PipelineReloadResult
{
    daxa_PipelineReloadResult out{};
    if(pipeline_manager == nullptr)
    {
        out.index = 2;
        out.values.error.message = "Invalid pipeline manager";
        return out;
    }
    
    auto result = pipeline_manager->impl.reload_all();
    if(result.index() == 0)
    {
        out.index = 0;
        out.values.no_change = {};
    }
    else if(result.index() == 1)
    {
        out.index = 1;
        out.values.success = {};
    }
    else if(result.index() == 2)
    {
        out.index = 2;
        pipeline_manager->latest_error = daxa::get<daxa::PipelineReloadError>(result).message;
        out.values.error.message = pipeline_manager->latest_error.c_str();
    }
    return out;
}

auto daxa_pipeline_manager_all_pipelines_valid(daxa_PipelineManager pipeline_manager) -> daxa_Bool8
{
    if(pipeline_manager == nullptr)
    {
        return 0;
    }
    
    return pipeline_manager->impl.all_pipelines_valid() ? 1 : 0;
}

#endif