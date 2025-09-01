#if DAXA_BUILT_WITH_UTILS_TASK_GRAPH

#include "../impl_core.hpp"
#include <daxa/c/utils/task_graph.h>
#include <daxa/utils/task_graph.hpp>

struct daxa_TaskBufferImpl 
{
    daxa::TaskBuffer internal;
};

struct daxa_TaskBlasImpl 
{
    daxa::TaskBlas internal;
};

struct daxa_TaskTlasImpl 
{
    daxa::TaskTlas internal;
};

struct daxa_TaskImageImpl 
{
    daxa::TaskImage internal;
};


auto daxa_create_task_buffer(daxa_TaskBufferInfo const* info, daxa_TaskBuffer* out) -> void 
{
    daxa::TaskBufferInfo i{
        .initial_buffers = std::bit_cast<TrackedBuffers>(info->initial_buffers),
        .name=std::string(info->name)
    };

    (*out) = new daxa_TaskBufferImpl();
    (*out)->internal = {i};
}

auto daxa_destroy_task_buffer(daxa_TaskBuffer buffer) -> void 
{
    delete buffer;
}

auto daxa_task_buffer_set_buffers(daxa_TaskBuffer buffer, daxa_TrackedBuffers const* buffers) -> void
{
    buffer->internal.set_buffers(*rc_cast<daxa::TrackedBuffers*>(buffers));
}

auto daxa_task_buffer_get_view(daxa_TaskBuffer buffer) -> daxa_TaskBufferView 
{
    return std::bit_cast<daxa_TaskBufferView>(buffer->internal.view());
}

auto daxa_create_task_blas(daxa_TaskBlasInfo const * info, daxa_TaskBlas* out) -> void 
{
    daxa::TaskBlasInfo i{
        .initial_blas = std::bit_cast<TrackedBlas>(info->initial_blas),
        .name=std::string(info->name)
    };

    (*out) = new daxa_TaskBlasImpl();
    (*out)->internal = {i};
}

auto daxa_destroy_task_blas(daxa_TaskBlas blas) -> void
{
    delete blas;
}

auto daxa_task_blas_set_blas(daxa_TaskBlas blas, daxa_TrackedBlas const* tracked_blas) -> void
{
    blas->internal.set_blas(*rc_cast<daxa::TrackedBlas*>(tracked_blas));
}

auto daxa_task_blas_get_view(daxa_TaskBlas blas) -> daxa_TaskBlasView 
{
    return std::bit_cast<daxa_TaskBlasView>(blas->internal.view());
}

auto daxa_create_task_tlas(daxa_TaskTlasInfo const * info, daxa_TaskTlas* out) -> void 
{
    daxa::TaskTlasInfo i{
        .initial_tlas = std::bit_cast<daxa::TrackedTlas>(info->initial_tlas),
        .name = std::string(info->name)
    };

    (*out) = new daxa_TaskTlasImpl();
    (*out)->internal = {i};
}

auto daxa_destroy_task_tlas(daxa_TaskTlas tlas) -> void 
{
    delete tlas;
}

auto daxa_task_tlas_set_tlas(daxa_TaskTlas tlas, daxa_TrackedTlas const* tracked_tlas) -> void
{
    tlas->internal.set_tlas(*rc_cast<daxa::TrackedTlas*>(tracked_tlas));
}

auto daxa_task_tlas_get_view(daxa_TaskTlas tlas) -> daxa_TaskTlasView 
{
    return std::bit_cast<daxa_TaskTlasView>(tlas->internal.view());
}

auto daxa_create_task_image(daxa_TaskImageInfo const * info, daxa_TaskImage* out) -> void
{
    daxa::TaskImageInfo i{
        .initial_images = std::bit_cast<daxa::TrackedImages>(info->initial_images),
        .swapchain_image = bool(info->swapchain_image),
        .name=std::string(info->name)
    };

    (*out) = new daxa_TaskImageImpl();
    (*out)->internal = {i};
}

auto daxa_destroy_task_image(daxa_TaskImage image) -> void 
{
    delete image;
}

auto daxa_task_image_set_images(daxa_TaskImage image, daxa_TrackedImages const* images) -> void
{
    image->internal.set_images(*rc_cast<daxa::TrackedImages*>(images));
}

auto daxa_task_image_get_view(daxa_TaskImage image) -> daxa_TaskImageView 
{
    return std::bit_cast<daxa_TaskImageView>(image->internal.view());
}

auto daxa_task_image_view_mips(daxa_TaskImageView view, daxa_u32 base_mip_level, daxa_u32 level_count) -> daxa_TaskImageView 
{
    auto cpp_view = std::bit_cast<daxa::TaskImageView>(view);
    return std::bit_cast<daxa_TaskImageView>(cpp_view.mips(base_mip_level, level_count));
}

auto daxa_task_image_view_layers(daxa_TaskImageView view, daxa_u32 base_array_layer, daxa_u32 layer_count) -> daxa_TaskImageView 
{
    auto cpp_view = std::bit_cast<daxa::TaskImageView>(view);
    return std::bit_cast<daxa_TaskImageView>(cpp_view.layers(base_array_layer, layer_count));
}

auto daxa_ti_get_buffer(daxa_TaskInterface* ti,daxa_TaskBufferAttachmentIndex index) -> daxa_TaskBufferAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskBufferAttachmentIndex>(index));
    return (daxa_TaskBufferAttachmentInfo*)&attachment;
}

auto daxa_ti_get_buffer_from_view(daxa_TaskInterface* ti,daxa_TaskBufferView view) -> daxa_TaskBufferAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskBufferView>(view));
    return (daxa_TaskBufferAttachmentInfo*)&attachment;
}

auto daxa_ti_get_blas(daxa_TaskInterface* ti,daxa_TaskBlasAttachmentIndex index) -> daxa_TaskBlasAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskBlasAttachmentIndex>(index));
    return (daxa_TaskBlasAttachmentInfo*)&attachment;
}

auto daxa_ti_get_blas_from_view(daxa_TaskInterface* ti,daxa_TaskBlasView view) -> daxa_TaskBlasAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskBlasView>(view));
    return (daxa_TaskBlasAttachmentInfo*)&attachment;
}

auto daxa_ti_get_tlas(daxa_TaskInterface* ti,daxa_TaskTlasAttachmentIndex index) -> daxa_TaskTlasAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskTlasAttachmentIndex>(index));
    return (daxa_TaskTlasAttachmentInfo*)&attachment;
}

auto daxa_ti_get_tlas_from_view(daxa_TaskInterface* ti,daxa_TaskTlasView view) -> daxa_TaskTlasAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskTlasView>(view));
    return (daxa_TaskTlasAttachmentInfo*)&attachment;
}

auto daxa_ti_get_image(daxa_TaskInterface* ti,daxa_TaskImageAttachmentIndex index) -> daxa_TaskImageAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskImageAttachmentIndex>(index));
    return (daxa_TaskImageAttachmentInfo*)&attachment;
}

auto daxa_ti_get_image_from_view(daxa_TaskInterface* ti,daxa_TaskImageView view) -> daxa_TaskImageAttachmentInfo const* 
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(std::bit_cast<daxa::TaskImageView>(view));
    return (daxa_TaskImageAttachmentInfo*)&attachment;
}

auto daxa_ti_get_attachment(daxa_TaskInterface* ti,size_t index) -> daxa_TaskAttachmentInfo const*
{
    if(ti == nullptr)
    {
        return nullptr;
    }
    const auto& attachment = ((daxa::TaskInterface*)ti)->get(index);
    return (daxa_TaskAttachmentInfo*)&attachment;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

struct daxa_TaskGraphImpl 
{
    daxa::TaskGraph internal;
};

struct daxa_TaskImpl 
{
    std::unique_ptr<daxa::InlineTask> internal;
    void* user_data;
};

auto daxa_create_task(daxa_TaskInfo const* info, daxa_Task* out) -> void
{
    if(info == nullptr || out == nullptr)
    {
        return;
    }

    (*out) = new daxa_TaskImpl();
    (*out)->internal = std::make_unique<daxa::InlineTask>(daxa::InlineTask{std::string_view{info->name}, (daxa::TaskType)info->task_type});
    (*out)->internal->uses_queue(std::bit_cast<daxa::Queue>(info->queue));
    (*out)->user_data = info->user_data;
}

auto daxa_task_uses(daxa_Task task, daxa_TaskAttachmentInfo attachment) -> void
{
    if(task == nullptr)
    {
        return;
    }
    task->internal->uses(std::bit_cast<daxa::TaskAttachmentInfo>(attachment));
}

auto daxa_task_executes(daxa_Task task, void (*execute_callback)(daxa_TaskInterface* ti, void* user_data)) -> void
{
    if(task == nullptr || execute_callback == nullptr)
    {
        return;
    }
    task->internal->executes([user = task->user_data, execute_callback](daxa::TaskInterface ti)
    {
        if(execute_callback)
        {
            execute_callback((daxa_TaskInterface*)(&ti), user);
        }
    });
}

auto daxa_destroy_task(daxa_Task task) -> void
{
    delete task;
}

auto daxa_create_task_graph(daxa_TaskGraphInfo const* info, daxa_TaskGraph* out) -> void
{
    if(info == nullptr || out == nullptr)
    {
        return;
    }
    daxa::TaskGraphInfo graphInfo;
    std::memcpy((void*)&graphInfo, info, sizeof(daxa_TaskGraphInfo));
    if(info->pre_task_callback != nullptr)
    {
        graphInfo.pre_task_callback = [callback = info->pre_task_callback, user = info->user_data](daxa::TaskInterface ti)
        {
            callback((daxa_TaskInterface*)(&ti), user);
        };
    }

    if(info->post_task_callback != nullptr)
    {
        graphInfo.post_task_callback = [callback = info->post_task_callback, user = info->user_data](daxa::TaskInterface ti)
        {
            callback((daxa_TaskInterface*)(&ti), user);
        };
    }

    graphInfo.default_queue = std::bit_cast<daxa::Queue>(info->default_queue);
    graphInfo.name = info->name;

    (*out) = new daxa_TaskGraphImpl();
    (*out)->internal = {graphInfo};
}

auto daxa_destroy_task_graph(daxa_TaskGraph graph) -> void
{
    delete graph;
}

auto daxa_tg_use_persistent_buffer(daxa_TaskGraph graph, daxa_TaskBuffer buffer) -> void
{
    if(graph == nullptr || buffer == nullptr)
    {
        return;
    }
    graph->internal.use_persistent_buffer(buffer->internal);
}

auto daxa_tg_use_persistent_blas(daxa_TaskGraph graph, daxa_TaskBlas blas) -> void
{
    if(graph == nullptr || blas == nullptr)
    {
        return;
    }
    graph->internal.use_persistent_blas(blas->internal);
}

auto daxa_tg_use_persistent_tlas(daxa_TaskGraph graph, daxa_TaskTlas tlas) -> void
{
    if(graph == nullptr || tlas == nullptr)
    {
        return;
    }
    graph->internal.use_persistent_tlas(tlas->internal);
}

auto daxa_tg_use_persistent_image(daxa_TaskGraph graph, daxa_TaskImage image) -> void
{
    if(graph == nullptr || image == nullptr)
    {
        return;
    }
    graph->internal.use_persistent_image(image->internal);
}

auto daxa_tg_create_transient_buffer(daxa_TaskGraph graph, daxa_TaskTransientBufferInfo const* info) -> daxa_TaskBufferView
{
    if(graph == nullptr || info == nullptr)
    {
        return {};
    }
    return std::bit_cast<daxa_TaskBufferView>(graph->internal.create_transient_buffer(*(daxa::TaskTransientBufferInfo*)info));
}

auto daxa_tg_create_transient_tlas(daxa_TaskGraph graph, daxa_TaskTransientTlasInfo const* info) -> daxa_TaskTlasView
{
    if(graph == nullptr || info == nullptr)
    {
        return {};
    }
    return std::bit_cast<daxa_TaskTlasView>(graph->internal.create_transient_tlas(*(daxa::TaskTransientTlasInfo*)info));
}

auto daxa_tg_create_transient_image(daxa_TaskGraph graph, daxa_TaskTransientImageInfo const* info) -> daxa_TaskImageView
{
    if(graph == nullptr || info == nullptr)
    {
        return {};
    }
    return std::bit_cast<daxa_TaskImageView>(graph->internal.create_transient_image(*(daxa::TaskTransientImageInfo*)info));
}

auto daxa_tg_transient_buffer_info(daxa_TaskGraph graph, daxa_TaskBufferView transient) -> daxa_TaskTransientBufferInfo const*
{
    if(graph == nullptr)
    {
        return nullptr;
    }
    return (daxa_TaskTransientBufferInfo*)&graph->internal.transient_buffer_info(*(daxa::TaskBufferView*)(&transient));
}

auto daxa_tg_transient_tlas_info(daxa_TaskGraph graph, daxa_TaskTlasView transient) -> daxa_TaskTransientTlasInfo const*
{
    if(graph == nullptr)
    {
        return nullptr;
    }
    return (daxa_TaskTransientBufferInfo*)&graph->internal.transient_tlas_info(*(daxa::TaskTlasView*)(&transient));
}

auto daxa_tg_transient_image_info(daxa_TaskGraph graph, daxa_TaskImageView transient) -> daxa_TaskTransientImageInfo const*
{
    if(graph == nullptr)
    {
        return nullptr;
    }
    return (daxa_TaskTransientImageInfo*)&graph->internal.transient_image_info(*(daxa::TaskImageView*)(&transient));
}

auto daxa_tg_clear_buffer(daxa_TaskGraph graph, daxa_TaskBufferClearInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.clear_buffer(*(daxa::TaskBufferClearInfo*)info);
}

auto daxa_tg_clear_image(daxa_TaskGraph graph, daxa_TaskImageClearInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.clear_image(*(daxa::TaskImageClearInfo*)info);
}

auto daxa_tg_copy_buffer_to_buffer(daxa_TaskGraph graph, daxa_TaskBufferCopyInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.copy_buffer_to_buffer(*(daxa::TaskBufferCopyInfo*)info);
}

auto daxa_tg_copy_image_to_image(daxa_TaskGraph graph, daxa_TaskImageCopyInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.copy_image_to_image(*(daxa::TaskImageCopyInfo*)info);
}

auto daxa_tg_add_task(daxa_TaskGraph graph, daxa_Task task) -> void
{
    if(graph == nullptr || task == nullptr)
    {
        return;
    }
    graph->internal.add_task(*task->internal);
}

auto daxa_tg_conditional(daxa_TaskGraph graph, daxa_TaskGraphConditionalInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    daxa::TaskGraphConditionalInfo i{
        .condition_index=info->condition_index,
        .when_true=[when_true = info->when_true]()
        {
            when_true();
        },
        .when_false=[when_false = info->when_false]()
        {
            when_false();
        }
    };
    graph->internal.conditional(i);
}

auto daxa_tg_submit(daxa_TaskGraph graph, daxa_TaskSubmitInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.submit(*(daxa::TaskSubmitInfo*)info);
}

auto daxa_tg_present(daxa_TaskGraph graph, daxa_TaskPresentInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.present(*(daxa::TaskPresentInfo*)info);
}

auto daxa_tg_complete(daxa_TaskGraph graph, daxa_TaskCompleteInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.complete(*(daxa::TaskCompleteInfo*)info);
}

auto daxa_tg_execute(daxa_TaskGraph graph, daxa_TaskExecutionInfo const* info) -> void
{
    if(graph == nullptr || info == nullptr)
    {
        return;
    }
    graph->internal.execute(*(daxa::ExecutionInfo*)info);
}

#endif