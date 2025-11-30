#include "../impl_core.hpp"

#include <daxa/daxa.inl>

struct ImGuiVertex
{
    daxa_f32vec2 pos;
    daxa_f32vec2 uv;
    daxa_u32 color;
};

DAXA_DECL_BUFFER_PTR(ImGuiVertex)

struct Push
{
    daxa_f32vec2 scale;
    daxa_f32vec2 translate;
    daxa_u32 vbuffer_offset;
    daxa_u32 ibuffer_offset;
    daxa_BufferPtr(ImGuiVertex) vbuffer_ptr;
    daxa_BufferPtr(daxa_u32) ibuffer_ptr;
    daxa_ImageViewId texture0_id;
    daxa_SamplerId sampler0_id;
};

#if DAXA_BUILT_WITH_UTILS_IMGUI && !DAXA_COMPILE_IMGUI_SHADERS

#include "impl_imgui.hpp"

#include <cstring>
#include <utility>
#include <algorithm>
#include <iostream>

void set_imgui_style()
{
    ImVec4 * colors = ImGui::GetStyle().Colors;
    ImGuiStyle & style = ImGui::GetStyle();
    // clang-format off
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    style.WindowPadding                     = ImVec2(8.00f, 8.00f);
    style.FramePadding                      = ImVec2(5.00f, 2.00f);
    style.CellPadding                       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                     = 25;
    style.ScrollbarSize                     = 15;
    style.GrabMinSize                       = 10;
    style.WindowBorderSize                  = 1;
    style.ChildBorderSize                   = 1;
    style.PopupBorderSize                   = 1;
    style.FrameBorderSize                   = 1;
    style.TabBorderSize                     = 1;
    style.WindowRounding                    = 7;
    style.ChildRounding                     = 4;
    style.FrameRounding                     = 3;
    style.PopupRounding                     = 4;
    style.ScrollbarRounding                 = 9;
    style.GrabRounding                      = 3;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 4;
    // clang-format on
}

#include "impl_imgui_spv.hpp"

namespace daxa
{
    ImGuiRenderer::ImGuiRenderer(ImGuiRendererInfo const & info)
    {
        this->object = new ImplImGuiRenderer(info);
    }

    void ImGuiRenderer::record_commands(ImDrawData * draw_data, CommandRecorder & recorder, ImageId target_image, u32 size_x, u32 size_y)
    {
        auto & impl = *r_cast<ImplImGuiRenderer *>(this->object);
        impl.record_commands(draw_data, recorder, target_image, size_x, size_y);
    }

    auto ImGuiRenderer::create_texture_id(ImGuiImageContext const & context) -> ImTextureID
    {
        auto & impl = *r_cast<ImplImGuiRenderer *>(this->object);
        impl.image_sampler_pairs.push_back(context);
        return std::bit_cast<ImTextureID>(impl.image_sampler_pairs.size() - 1);
    }

#if DAXA_BUILT_WITH_UTILS_TASK_GRAPH
    void ImGuiRenderer::record_task(ImDrawData * /* draw_data */, TaskGraph & /* task_graph */, TaskImageView /* task_swapchain_image */, u32 /* size_x */, u32 /* size_y */)
    {
    }
#endif

    void ImplImGuiRenderer::recreate_vbuffer(usize vbuffer_new_size)
    {
        vbuffer = info.device.create_buffer({
            .size = static_cast<u32>(vbuffer_new_size),
            .name = std::string("dear ImGui vertex buffer"),
        });
    }
    void ImplImGuiRenderer::recreate_ibuffer(usize ibuffer_new_size)
    {
        ibuffer = info.device.create_buffer({
            .size = static_cast<u32>(ibuffer_new_size),
            .name = std::string("dear ImGui index buffer"),
        });
    }

    void ImplImGuiRenderer::record_commands(ImDrawData * draw_data, CommandRecorder & recorder, ImageId target_image, u32 size_x, u32 size_y)
    {
        ++frame_count;
        if ((draw_data != nullptr) && draw_data->TotalIdxCount > 0)
        {
            auto vbuffer_current_size = info.device.buffer_info(vbuffer).value().size;
            auto vbuffer_needed_size = static_cast<usize>(draw_data->TotalVtxCount) * sizeof(ImDrawVert);
            auto ibuffer_current_size = info.device.buffer_info(ibuffer).value().size;
            auto ibuffer_needed_size = static_cast<usize>(draw_data->TotalIdxCount) * sizeof(ImDrawIdx);

            if (vbuffer_needed_size > vbuffer_current_size)
            {
                auto vbuffer_new_size = vbuffer_needed_size + 4096;
                info.device.destroy_buffer(vbuffer);
                recreate_vbuffer(vbuffer_new_size);
            }
            if (ibuffer_needed_size > ibuffer_current_size)
            {
                auto ibuffer_new_size = ibuffer_needed_size + 4096;
                info.device.destroy_buffer(ibuffer);
                recreate_ibuffer(ibuffer_new_size);
            }

            constexpr usize IMGUI_RESOURCE_NAME_MAX_NUMBER = 8;

            auto staging_vbuffer = info.device.create_buffer({
                .size = static_cast<u32>(vbuffer_needed_size),
                .allocate_info = daxa::MemoryFlagBits::HOST_ACCESS_RANDOM,
                .name = std::string("dear ImGui vertex staging buffer ") + std::to_string(frame_count % IMGUI_RESOURCE_NAME_MAX_NUMBER),
            });
            auto * vtx_dst = info.device.buffer_host_address_as<ImDrawVert>(staging_vbuffer).value();
            for (i32 n = 0; n < draw_data->CmdListsCount; n++)
            {
                ImDrawList const * draws = draw_data->CmdLists[n];
                std::memcpy(vtx_dst, draws->VtxBuffer.Data, static_cast<usize>(draws->VtxBuffer.Size) * sizeof(ImDrawVert));
                vtx_dst += draws->VtxBuffer.Size;
            }
            recorder.destroy_buffer_deferred(staging_vbuffer);
            auto staging_ibuffer = info.device.create_buffer({
                .size = static_cast<u32>(ibuffer_needed_size),
                .allocate_info = daxa::MemoryFlagBits::HOST_ACCESS_RANDOM,
                .name = std::string("dear ImGui index staging buffer ") + std::to_string(frame_count % IMGUI_RESOURCE_NAME_MAX_NUMBER),
            });
            auto * idx_dst = info.device.buffer_host_address_as<ImDrawIdx>(staging_ibuffer).value();
            for (i32 n = 0; n < draw_data->CmdListsCount; n++)
            {
                ImDrawList const * draws = draw_data->CmdLists[n];
                std::memcpy(idx_dst, draws->IdxBuffer.Data, static_cast<usize>(draws->IdxBuffer.Size) * sizeof(ImDrawIdx));
                idx_dst += draws->IdxBuffer.Size;
            }
            recorder.destroy_buffer_deferred(staging_ibuffer);
            recorder.pipeline_barrier({
                .src_access = daxa::AccessConsts::HOST_WRITE,
                .dst_access = daxa::AccessConsts::TRANSFER_READ,
            });
            recorder.copy_buffer_to_buffer({
                .src_buffer = staging_ibuffer,
                .dst_buffer = ibuffer,
                .size = ibuffer_needed_size,
            });
            recorder.copy_buffer_to_buffer({
                .src_buffer = staging_vbuffer,
                .dst_buffer = vbuffer,
                .size = vbuffer_needed_size,
            });
            recorder.pipeline_barrier({
                .src_access = daxa::AccessConsts::TRANSFER_WRITE,
                .dst_access = daxa::AccessConsts::VERTEX_SHADER_READ | daxa::AccessConsts::INDEX_INPUT_READ,
            });

            auto render_recorder = std::move(recorder).begin_renderpass({
                .color_attachments = std::array{RenderAttachmentInfo{.image_view = target_image.default_view(), .load_op = AttachmentLoadOp::LOAD}},
                .render_area = {.x = 0, .y = 0, .width = size_x, .height = size_y},
            });

            render_recorder.set_pipeline(raster_pipeline);

            render_recorder.set_index_buffer({
                .id = ibuffer,
                .offset = 0,
                .index_type = IndexType::uint16,
            });

            auto push = Push{};
            push.scale = {2.0f / draw_data->DisplaySize.x, 2.0f / draw_data->DisplaySize.y};
            push.translate = {-1.0f - draw_data->DisplayPos.x * push.scale.x, -1.0f - draw_data->DisplayPos.y * push.scale.y};
            ImVec2 const clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
            ImVec2 const clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)
            i32 global_vtx_offset = 0;
            i32 global_idx_offset = 0;
            push.vbuffer_ptr = this->info.device.device_address(vbuffer).value();
            push.ibuffer_ptr = this->info.device.device_address(ibuffer).value();

#if IMGUI_VERSION_NUM >= 19200
            for(ImTextureData* tex : *draw_data->Textures)
            {
                if(tex->Status == ImTextureStatus_OK) continue;

                /*struct ImGuiImageContext
                {
                    ImageViewId image_view_id;
                    SamplerId sampler_id;
                };
                */
                //image_sampler_pairs.push_back(context);
                //return std::bit_cast<ImTextureID>(impl.image_sampler_pairs.size() - 1);

                if (tex->Status == ImTextureStatus_WantCreate)
                {
                    IM_ASSERT(tex->TexID == ImTextureID_Invalid && tex->BackendUserData == nullptr);
                    IM_ASSERT(tex->Format == ImTextureFormat_RGBA32);

                    auto image = font_sheet = this->info.device.create_image({
                        .size = {static_cast<u32>(tex->Width), static_cast<u32>(tex->Height), 1},
                        .usage = ImageUsageFlagBits::TRANSFER_DST | ImageUsageFlagBits::SHADER_SAMPLED,
                    });

                    // Create the Image View:
                    /*{
                        VkImageViewCreateInfo info = {};
                        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                        info.image = backend_tex->Image;
                        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
                        info.format = VK_FORMAT_R8G8B8A8_UNORM;
                        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                        info.subresourceRange.levelCount = 1;
                        info.subresourceRange.layerCount = 1;
                        err = vkCreateImageView(v->Device, &info, v->Allocator, &backend_tex->ImageView);
                        check_vk_result(err);
                    }*/

                    // Store identifiers
                    image_sampler_pairs.push_back({
                        .image_view_id = image.default_view(),
                        .sampler_id = font_sampler,
                    });
                    tex->SetTexID(std::bit_cast<ImTextureID>(image_sampler_pairs.size() - 1));
                }

                if (tex->Status == ImTextureStatus_WantCreate || tex->Status == ImTextureStatus_WantUpdates)
                {
                    auto const image_context = this->image_sampler_pairs.at(std::bit_cast<usize>(tex->GetTexID()));
                    auto image_id = info.device.image_view_info(image_context.image_view_id).value().image;

                    // Update full texture or selected blocks. We only ever write to textures regions which have never been used before!
                    // This backend choose to use tex->UpdateRect but you can use tex->Updates[] to upload individual regions.
                    // We could use the smaller rect on _WantCreate but using the full rect allows us to clear the texture.
                    const int upload_x = (tex->Status == ImTextureStatus_WantCreate) ? 0 : tex->UpdateRect.x;
                    const int upload_y = (tex->Status == ImTextureStatus_WantCreate) ? 0 : tex->UpdateRect.y;
                    const int upload_w = (tex->Status == ImTextureStatus_WantCreate) ? tex->Width : tex->UpdateRect.w;
                    const int upload_h = (tex->Status == ImTextureStatus_WantCreate) ? tex->Height : tex->UpdateRect.h;

                    usize const upload_size = static_cast<usize>(upload_w) * static_cast<usize>(upload_h) * 4 * sizeof(u8);
                    auto texture_staging_buffer = this->info.device.create_buffer({
                        .size = static_cast<u32>(upload_size),
                        .allocate_info = daxa::MemoryFlagBits::HOST_ACCESS_RANDOM,
                    });

                    /*
                    {
                        VkBufferCreateInfo buffer_info = {};
                        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                        buffer_info.size = upload_size;
                        buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
                        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
                        err = vkCreateBuffer(v->Device, &buffer_info, v->Allocator, &upload_buffer);
                        check_vk_result(err);
                        VkMemoryRequirements req;
                        vkGetBufferMemoryRequirements(v->Device, upload_buffer, &req);
                        bd->BufferMemoryAlignment = (bd->BufferMemoryAlignment > req.alignment) ? bd->BufferMemoryAlignment : req.alignment;
                        VkMemoryAllocateInfo alloc_info = {};
                        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                        alloc_info.allocationSize = IM_MAX(v->MinAllocationSize, req.size);
                        alloc_info.memoryTypeIndex = ImGui_ImplVulkan_MemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, req.memoryTypeBits);
                        err = vkAllocateMemory(v->Device, &alloc_info, v->Allocator, &upload_buffer_memory);
                        check_vk_result(err);
                        err = vkBindBufferMemory(v->Device, upload_buffer, upload_buffer_memory, 0);
                        check_vk_result(err);
                    }

                    // Upload to Buffer:
                    {
                        char* map = nullptr;
                        err = vkMapMemory(v->Device, upload_buffer_memory, 0, upload_size, 0, (void**)(&map));
                        check_vk_result(err);
                        for (int y = 0; y < upload_h; y++)
                            memcpy(map + upload_pitch * y, tex->GetPixelsAt(upload_x, upload_y + y), (size_t)upload_pitch);
                        VkMappedMemoryRange range[1] = {};
                        range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
                        range[0].memory = upload_buffer_memory;
                        range[0].size = upload_size;
                        err = vkFlushMappedMemoryRanges(v->Device, 1, range);
                        check_vk_result(err);
                        vkUnmapMemory(v->Device, upload_buffer_memory);
                    }

                    // Start command buffer
                    {
                        err = vkResetCommandPool(v->Device, bd->TexCommandPool, 0);
                        check_vk_result(err);
                        VkCommandBufferBeginInfo begin_info = {};
                        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                        err = vkBeginCommandBuffer(bd->TexCommandBuffer, &begin_info);
                        check_vk_result(err);
                    }

                    // Copy to Image:
                    {
                        VkBufferMemoryBarrier upload_barrier[1] = {};
                        upload_barrier[0].sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
                        upload_barrier[0].srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
                        upload_barrier[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                        upload_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        upload_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        upload_barrier[0].buffer = upload_buffer;
                        upload_barrier[0].offset = 0;
                        upload_barrier[0].size = upload_size;

                        VkImageMemoryBarrier copy_barrier[1] = {};
                        copy_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                        copy_barrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                        copy_barrier[0].oldLayout = (tex->Status == ImTextureStatus_WantCreate) ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        copy_barrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                        copy_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        copy_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        copy_barrier[0].image = backend_tex->Image;
                        copy_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                        copy_barrier[0].subresourceRange.levelCount = 1;
                        copy_barrier[0].subresourceRange.layerCount = 1;
                        vkCmdPipelineBarrier(bd->TexCommandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 1, upload_barrier, 1, copy_barrier);

                        VkBufferImageCopy region = {};
                        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                        region.imageSubresource.layerCount = 1;
                        region.imageExtent.width = upload_w;
                        region.imageExtent.height = upload_h;
                        region.imageExtent.depth = 1;
                        region.imageOffset.x = upload_x;
                        region.imageOffset.y = upload_y;
                        vkCmdCopyBufferToImage(bd->TexCommandBuffer, upload_buffer, backend_tex->Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

                        VkImageMemoryBarrier use_barrier[1] = {};
                        use_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                        use_barrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                        use_barrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                        use_barrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                        use_barrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        use_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        use_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        use_barrier[0].image = backend_tex->Image;
                        use_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                        use_barrier[0].subresourceRange.levelCount = 1;
                        use_barrier[0].subresourceRange.layerCount = 1;
                        vkCmdPipelineBarrier(bd->TexCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, use_barrier);
                    }

                    // End command buffer
                    {
                        VkSubmitInfo end_info = {};
                        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                        end_info.commandBufferCount = 1;
                        end_info.pCommandBuffers = &bd->TexCommandBuffer;
                        err = vkEndCommandBuffer(bd->TexCommandBuffer);
                        check_vk_result(err);
                        err = vkQueueSubmit(v->Queue, 1, &end_info, VK_NULL_HANDLE);
                        check_vk_result(err);
                    }

                    err = vkQueueWaitIdle(v->Queue); // FIXME-OPT: Suboptimal!
                    check_vk_result(err);
                    vkDestroyBuffer(v->Device, upload_buffer, v->Allocator);
                    vkFreeMemory(v->Device, upload_buffer_memory, v->Allocator);*/

                    u8* staging_buffer_data = this->info.device.buffer_host_address_as<u8>(texture_staging_buffer).value();
                    for (int y = 0; y < upload_h; y++)
                    {
                        std::memcpy(staging_buffer_data + sizeof(u8) * y, tex->GetPixelsAt(upload_x, upload_y + y), (size_t)sizeof(u8));
                    }
                    std::memcpy(staging_buffer_data, tex->GetPixels(), upload_size);
                    auto recorder = this->info.device.create_command_recorder({.name = "dear ImGui Tex Upload"});
                    recorder.pipeline_barrier_image_transition({
                        .src_access = daxa::AccessConsts::HOST_WRITE,
                        .dst_access = daxa::AccessConsts::TRANSFER_READ_WRITE,
                        .dst_layout = daxa::ImageLayout::GENERAL,
                        .image_slice = {
                            .base_mip_level = 0,
                            .level_count = 1,
                            .base_array_layer = 0,
                            .layer_count = 1,
                        },
                        .image_id = image_id,
                    });
                    recorder.copy_buffer_to_image({
                        .buffer = texture_staging_buffer,
                        .image = image_id,
                        .image_slice = {
                            .mip_level = 0,
                            .base_array_layer = 0,
                            .layer_count = 1,
                        },
                        .image_offset = {upload_x, upload_y, 0},
                        .image_extent = {static_cast<u32>(upload_w), static_cast<u32>(upload_h), 1},
                    });
                    recorder.pipeline_barrier_image_transition({
                        .src_access = daxa::AccessConsts::TRANSFER_WRITE,
                        .dst_access = daxa::AccessConsts::FRAGMENT_SHADER_READ,
                        .src_layout = daxa::ImageLayout::GENERAL,
                        .dst_layout = daxa::ImageLayout::GENERAL,
                        .image_slice = {
                            .base_mip_level = 0,
                            .level_count = 1,
                            .base_array_layer = 0,
                            .layer_count = 1,
                        },
                        .image_id = image_id,
                    });
                    auto executable_commands = recorder.complete_current_commands();
                    this->info.device.submit_commands({
                        .command_lists = std::array{executable_commands},
                    });
                    this->info.device.destroy_buffer(texture_staging_buffer);

                    tex->SetStatus(ImTextureStatus_OK);
                }

                if (tex->Status == ImTextureStatus_WantDestroy && tex->UnusedFrames >= (int)3)
                {
                    auto const image_context = this->image_sampler_pairs.at(std::bit_cast<usize>(tex->GetTexID()));
                    auto image_id = info.device.image_view_info(image_context.image_view_id).value().image;
                    info.device.destroy_image(image_id);
                    this->image_sampler_pairs.erase(this->image_sampler_pairs.begin() + std::bit_cast<usize>(tex->GetTexID()));
                }
            }
#endif

            for (i32 n = 0; n < draw_data->CmdListsCount; n++)
            {
                ImDrawList const * draws = draw_data->CmdLists[n];
                for (i32 cmd_i = 0; cmd_i < draws->CmdBuffer.Size; cmd_i++)
                {
                    ImDrawCmd const * pcmd = &draws->CmdBuffer[cmd_i];

                    // Project scissor/clipping rectangles into framebuffer space
                    ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                    ImVec2 const clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

                    // Clamp to viewport as vkCmdSetScissor() won't accept values that are off bounds
                    clip_min.x = std::clamp(clip_min.x, 0.0f, static_cast<f32>(size_x));
                    clip_min.y = std::clamp(clip_min.y, 0.0f, static_cast<f32>(size_y));
                    if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    {
                        continue;
                    }

                    // Apply scissor/clipping rectangle
                    Rect2D scissor;
                    scissor.x = static_cast<i32>(clip_min.x);
                    scissor.y = static_cast<i32>(clip_min.y);
                    scissor.width = static_cast<u32>(clip_max.x - clip_min.x);
                    scissor.height = static_cast<u32>(clip_max.y - clip_min.y);
                    render_recorder.set_scissor(scissor);

                    // Draw
#if IMGUI_VERSION_NUM < 19200
                    auto texId = pcmd->TextureId;
#else
                    auto texId = pcmd->GetTexID();
#endif
                    auto const image_context = this->image_sampler_pairs.at(std::bit_cast<usize>(texId));
                    push.texture0_id = image_context.image_view_id;
                    push.sampler0_id = image_context.sampler_id;

                    push.vbuffer_offset = pcmd->VtxOffset + static_cast<u32>(global_vtx_offset);
                    push.ibuffer_offset = pcmd->IdxOffset + static_cast<u32>(global_idx_offset);

                    render_recorder.push_constant(push);
                    render_recorder.draw_indexed({
                        .index_count = pcmd->ElemCount,
                        .first_index = pcmd->IdxOffset + static_cast<u32>(global_idx_offset),
                        .vertex_offset = static_cast<i32>(pcmd->VtxOffset) + global_vtx_offset,
                    });
                }
                global_idx_offset += draws->IdxBuffer.Size;
                global_vtx_offset += draws->VtxBuffer.Size;
            }

            recorder = std::move(render_recorder).end_renderpass();
        }
#if IMGUI_VERSION_NUM < 19200
        this->image_sampler_pairs.resize(1);
#endif
    }

    ImplImGuiRenderer::ImplImGuiRenderer(ImGuiRendererInfo a_info)
        : info{std::move(a_info)},
          raster_pipeline{
              [this]()
              {
                  auto create_info = daxa::RasterPipelineInfo{};
                  create_info.vertex_shader_info = daxa::ShaderInfo{.byte_code = imgui_vert_spv.data(), .byte_code_size = static_cast<u32>(imgui_vert_spv.size())};
                  // TODO(msakmary) Possibly add more UNORM swapchain formats or a bool flag that lets the user tell us if the target format is UNORM
                  if (info.format == daxa::Format::R8G8B8A8_UNORM || info.format == daxa::Format::B8G8R8A8_UNORM)
                  {
                      create_info.fragment_shader_info = daxa::ShaderInfo{
                          .byte_code = imgui_gamma_frag_spv.data(),
                          .byte_code_size = static_cast<u32>(imgui_gamma_frag_spv.size()),
                      };
                  }
                  else
                  {
                      create_info.fragment_shader_info = daxa::ShaderInfo{
                          .byte_code = imgui_frag_spv.data(),
                          .byte_code_size = static_cast<u32>(imgui_frag_spv.size()),
                      };
                  }
                  create_info.color_attachments = std::array{daxa::RenderAttachment{
                      .format = info.format,
                      .blend = daxa::BlendInfo{
                          .src_color_blend_factor = BlendFactor::SRC_ALPHA,
                          .dst_color_blend_factor = BlendFactor::ONE_MINUS_SRC_ALPHA,
                          .src_alpha_blend_factor = BlendFactor::ONE,
                          .dst_alpha_blend_factor = BlendFactor::ONE_MINUS_SRC_ALPHA,
                      },
                  }};
                  create_info.raster = {};
                  create_info.push_constant_size = sizeof(Push);
                  create_info.name = "ImGui Draw Pipeline";
                  // return daxa::RasterPipeline{};
                  return this->info.device.create_raster_pipeline(create_info);
              }()}
    {
        if (this->info.context != nullptr)
        {
            ImGui::SetCurrentContext(info.context);
        }
        if (this->info.use_custom_config)
        {
            set_imgui_style();
        }
        recreate_vbuffer(4096);
        recreate_ibuffer(4096);

        ImGuiIO & io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
        u8 * pixels = nullptr;
        i32 width = 0;
        i32 height = 0;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        usize const upload_size = static_cast<usize>(width) * static_cast<usize>(height) * 4 * sizeof(u8);
        font_sheet = this->info.device.create_image({
            .size = {static_cast<u32>(width), static_cast<u32>(height), 1},
            .usage = ImageUsageFlagBits::TRANSFER_DST | ImageUsageFlagBits::SHADER_SAMPLED,
            .name = "dear ImGui font sheet",
        });

        auto texture_staging_buffer = this->info.device.create_buffer({
            .size = static_cast<u32>(upload_size),
            .allocate_info = daxa::MemoryFlagBits::HOST_ACCESS_RANDOM,
        });

        u8 * staging_buffer_data = this->info.device.buffer_host_address_as<u8>(texture_staging_buffer).value();
        std::memcpy(staging_buffer_data, pixels, upload_size);

        auto recorder = this->info.device.create_command_recorder({.name = "dear ImGui Font Sheet Upload"});
        recorder.pipeline_image_barrier({
            .src_access = daxa::AccessConsts::HOST_WRITE,
            .dst_access = daxa::AccessConsts::TRANSFER_READ_WRITE,
            .layout_operation = daxa::ImageLayoutOperation::TO_GENERAL,
            .image_id = font_sheet,
        });
        recorder.copy_buffer_to_image({
            .buffer = texture_staging_buffer,
            .image = font_sheet,
            .image_slice = {
                .mip_level = 0,
                .base_array_layer = 0,
                .layer_count = 1,
            },
            .image_offset = {0, 0, 0},
            .image_extent = {static_cast<u32>(width), static_cast<u32>(height), 1},
        });
        recorder.pipeline_barrier({
            .src_access = daxa::AccessConsts::TRANSFER_WRITE,
            .dst_access = daxa::AccessConsts::FRAGMENT_SHADER_READ,
        });
        auto executable_commands = recorder.complete_current_commands();
        this->info.device.submit_commands({
            .command_lists = std::array{executable_commands},
        });
        this->info.device.destroy_buffer(texture_staging_buffer);
        this->font_sampler = this->info.device.create_sampler({.name = "ImGui Font Sampler"});
        this->image_sampler_pairs.push_back(ImGuiImageContext{
            .image_view_id = font_sheet.default_view(),
            .sampler_id = this->font_sampler,
        });
        io.Fonts->SetTexID({});
    }

    ImplImGuiRenderer::~ImplImGuiRenderer()
    {
        this->info.device.destroy_buffer(this->vbuffer);
        this->info.device.destroy_buffer(this->ibuffer);
        this->info.device.destroy_image(this->font_sheet);
        this->info.device.destroy_sampler(this->font_sampler);

#if IMGUI_VERSION_NUM >= 19200
        for (auto const &[image_view_id, sampler_id] : this->image_sampler_pairs)
        {
            if(info.device.is_image_view_id_valid(image_view_id))
            {
                this->info.device.destroy_image(this->info.device.image_view_info(image_view_id).value().image);
            }
            if(sampler_id != this->font_sampler && info.device.is_sampler_id_valid(sampler_id))
            {
                this->info.device.destroy_sampler(sampler_id);
            }
        }
#endif
    }

    void ImplImGuiRenderer::zero_ref_callback(ImplHandle const * handle)
    {
        auto const * self = r_cast<ImplImGuiRenderer const *>(handle);
        delete self;
    }

    auto ImGuiRenderer::inc_refcnt(ImplHandle const * object) -> u64
    {
        return object->inc_refcnt();
    }

    auto ImGuiRenderer::dec_refcnt(ImplHandle const * object) -> u64
    {
        return object->dec_refcnt(
            ImplImGuiRenderer::zero_ref_callback,
            nullptr);
    }
} // namespace daxa

#elif DAXA_SHADER

DAXA_DECL_PUSH_CONSTANT(Push, push)

#if DAXA_SHADER_STAGE == DAXA_SHADER_STAGE_VERTEX

layout(location = 0) out struct
{
    daxa_f32vec4 Color;
    daxa_f32vec2 UV;
} Out;

void main()
{
    // uint vert_index = deref(push.ibuffer_ptr[gl_VertexIndex + push.ibuffer_offset]);
    ImGuiVertex vert = deref(push.vbuffer_ptr[gl_VertexIndex]);

    daxa_f32vec2 aPos = vert.pos;
    daxa_f32vec2 aUV = vert.uv;
    daxa_u32 aColor = vert.color;

    Out.Color.r = ((aColor >> 0x00) & 0xff) * 1.0 / 255.0;
    Out.Color.g = ((aColor >> 0x08) & 0xff) * 1.0 / 255.0;
    Out.Color.b = ((aColor >> 0x10) & 0xff) * 1.0 / 255.0;
    Out.Color.a = ((aColor >> 0x18) & 0xff) * 1.0 / 255.0;
    Out.UV = aUV;
    gl_Position = vec4(aPos * push.scale + push.translate, 0, 1);
}

#elif DAXA_SHADER_STAGE == DAXA_SHADER_STAGE_FRAGMENT

layout(location = 0) out daxa_f32vec4 fColor;
layout(location = 0) in struct
{
    daxa_f32vec4 Color;
    daxa_f32vec2 UV;
} In;

daxa_f32vec4 srgb_to_linear(daxa_f32vec4 srgb)
{
    daxa_f32vec3 color_srgb = srgb.rgb;
    daxa_f32vec3 selector = clamp(ceil(color_srgb - 0.04045), 0.0, 1.0); // 0 if under value, 1 if over
    daxa_f32vec3 under = color_srgb / 12.92;
    daxa_f32vec3 over = pow((color_srgb + 0.055) / 1.055, daxa_f32vec3(2.4, 2.4, 2.4));
    daxa_f32vec3 result = mix(under, over, selector);
    return daxa_f32vec4(result, srgb.a);
}

daxa_f32vec4 linear_to_srgb(daxa_f32vec4 linear)
{
    daxa_f32vec3 color_linear = linear.rgb;
    daxa_f32vec3 selector = clamp(ceil(color_linear - 0.0031308), 0.0, 1.0); // 0 if under value, 1 if over
    daxa_f32vec3 under = 12.92 * color_linear;
    daxa_f32vec3 over = (1.055) * pow(color_linear, daxa_f32vec3(1.0 / 2.4)) - 0.055;
    daxa_f32vec3 result = mix(under, over, selector);
    return daxa_f32vec4(result, linear.a);
}

void main()
{
    fColor = srgb_to_linear(In.Color) * texture(daxa_sampler2D(push.texture0_id, push.sampler0_id), In.UV.st);
#if defined(GAMMA_CORRECTION)
    fColor = linear_to_srgb(fColor);
#endif
}

#endif
#endif

#if DAXA_COMPILE_IMGUI_SHADERS

#include <daxa/daxa.hpp>
#include <daxa/utils/pipeline_manager.hpp>
#include <fstream>
#include <format>
#include <iostream>

auto main() -> int
{
    daxa::Instance daxa_ctx = daxa::create_instance({});
    daxa::Device device = daxa_ctx.create_device_2(daxa_ctx.choose_device({}, daxa::DeviceInfo2{}));

    daxa::PipelineManager pipeline_manager = daxa::PipelineManager(daxa::PipelineManagerInfo2{
        .device = device,
        .root_paths = {
            DAXA_SHADER_INCLUDE_DIR,
            "tests/0_common/shaders",
        },
        .write_out_spirv = "./",
        .default_language = daxa::ShaderLanguage::GLSL,
        .default_enable_debug_info = false,
        .name = "pipeline_manager",
    });

    daxa::RasterPipelineCompileInfo2 compile_info{
        .vertex_shader_info = daxa::ShaderCompileInfo2{.source = daxa::ShaderFile{"src/utils/impl_imgui.cpp"}},
        .fragment_shader_info = daxa::ShaderCompileInfo2{.source = daxa::ShaderFile{"src/utils/impl_imgui.cpp"}},
        .color_attachments = {{.format = daxa::Format::R16G16B16A16_SFLOAT}},
        .raster = {},
        .push_constant_size = sizeof(Push),
        .name = "imgui_pipeline",
    };
    // NO GAMMA CORRECTION
    auto result = pipeline_manager.add_raster_pipeline2(compile_info);
    std::cout << result.to_string();

    auto vert_file = std::ifstream{"./imgui_pipeline.vert.main.spv", std::ios::binary};
    auto vert_size = std::filesystem::file_size("./imgui_pipeline.vert.main.spv");
    auto vert_bytes = std::vector<uint32_t>(vert_size / sizeof(uint32_t));
    vert_file.read(reinterpret_cast<char *>(vert_bytes.data()), static_cast<std::streamsize>(vert_size));
    vert_file.close();
    std::filesystem::remove("./imgui_pipeline.vert.spv");

    auto frag_file = std::ifstream{"./imgui_pipeline.frag.main.spv", std::ios::binary};
    auto frag_size = std::filesystem::file_size("./imgui_pipeline.frag.main.spv");
    auto frag_bytes = std::vector<uint32_t>(frag_size / sizeof(uint32_t));
    frag_file.read(reinterpret_cast<char *>(frag_bytes.data()), static_cast<std::streamsize>(frag_size));
    frag_file.close();
    std::filesystem::remove("./imgui_pipeline.frag.main.spv");

    // WITH GAMMA CORRECTION
    compile_info.fragment_shader_info.value().defines = {{"GAMMA_CORRECTION", "TRUE"}};
    result = pipeline_manager.add_raster_pipeline2(compile_info);
    std::cout << result.to_string();

    // vert is unchanged
    std::filesystem::remove("./imgui_pipeline.vert.main.spv");
    auto gamma_frag_file = std::ifstream{"./imgui_pipeline.frag.main.spv", std::ios::binary};
    auto gamma_frag_size = std::filesystem::file_size("./imgui_pipeline.frag.main.spv");
    auto gamma_frag_bytes = std::vector<uint32_t>(gamma_frag_size / sizeof(uint32_t));
    gamma_frag_file.read(reinterpret_cast<char *>(gamma_frag_bytes.data()), static_cast<std::streamsize>(gamma_frag_size));
    gamma_frag_file.close();
    std::filesystem::remove("./imgui_pipeline.frag.main.spv");

    auto out_file = std::ofstream{"./src/utils/impl_imgui_spv.hpp", std::ofstream::trunc};
    out_file << "#pragma once\n#include <array>\n\n";
    out_file << std::format("static constexpr auto imgui_vert_spv = std::array<uint32_t, {}>{{\n    // clang-format off\n   ", vert_size / sizeof(uint32_t));

    size_t iter = 0;
    for (auto const & u : vert_bytes)
    {
        out_file << std::format(" {:#010x},", u);
        if ((iter % 8) == 7)
        {
            out_file << "\n   ";
        }
        ++iter;
    }

    out_file << "\n    // clang-format on\n};\n";

    out_file << std::format("static constexpr auto imgui_frag_spv = std::array<uint32_t, {}>{{\n    // clang-format off\n   ", frag_size / sizeof(uint32_t));
    iter = 0;
    for (auto const & u : frag_bytes)
    {
        out_file << std::format(" {:#010x},", u);
        if ((iter % 8) == 7)
        {
            out_file << "\n   ";
        }
        ++iter;
    }

    out_file << "\n    // clang-format on\n};\n";

    out_file << std::format("static constexpr auto imgui_gamma_frag_spv = std::array<uint32_t, {}>{{\n    // clang-format off\n   ", gamma_frag_size / sizeof(uint32_t));
    iter = 0;
    for (auto const & u : gamma_frag_bytes)
    {
        out_file << std::format(" {:#010x},", u);
        if ((iter % 8) == 7)
        {
            out_file << "\n   ";
        }
        ++iter;
    }

    out_file << "\n    // clang-format on\n};\n";
}

#endif
