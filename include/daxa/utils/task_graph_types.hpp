#pragma once

// Disable msvc warning on alignment padding.
#if defined(_MSC_VER)
#pragma warning(disable : 4324)
#endif

#if !DAXA_BUILT_WITH_UTILS_TASK_GRAPH
#error "[package management error] You must build Daxa with the DAXA_ENABLE_UTILS_TASK_GRAPH CMake option enabled, or request the utils-task-graph feature in vcpkg"
#endif

#include <array>
#include <string_view>
#include <cstring>
#include <type_traits>
#include <span>
#include <algorithm>

#include <daxa/core.hpp>
#include <daxa/device.hpp>
#include <daxa/utils/mem.hpp>

namespace daxa
{
    enum struct TaskBufferAccess
    {
        NONE,
        READ,
        WRITE,
        READ_WRITE,
        READ_WRITE_CONCURRENT,
        GRAPHICS_SHADER_READ,
        GRAPHICS_SHADER_WRITE,
        GRAPHICS_SHADER_READ_WRITE,
        GRAPHICS_SHADER_READ_WRITE_CONCURRENT,
        GPS_READ = GRAPHICS_SHADER_READ,
        GPS_WRITE = GRAPHICS_SHADER_WRITE,
        GPS_READ_WRITE = GRAPHICS_SHADER_READ_WRITE,
        GPS_READ_WRITE_CONCURRENT = GRAPHICS_SHADER_READ_WRITE_CONCURRENT,
        COMPUTE_SHADER_READ,
        COMPUTE_SHADER_WRITE,
        COMPUTE_SHADER_READ_WRITE,
        COMPUTE_SHADER_READ_WRITE_CONCURRENT,
        CS_READ = COMPUTE_SHADER_READ,
        CS_WRITE = COMPUTE_SHADER_WRITE,
        CS_READ_WRITE = COMPUTE_SHADER_READ_WRITE,
        CS_READ_WRITE_CONCURRENT = COMPUTE_SHADER_READ_WRITE_CONCURRENT,
        RAY_TRACING_SHADER_READ,
        RAY_TRACING_SHADER_WRITE,
        RAY_TRACING_SHADER_READ_WRITE,
        RAY_TRACING_SHADER_READ_WRITE_CONCURRENT,
        RS_READ = RAY_TRACING_SHADER_READ,
        RS_WRITE = RAY_TRACING_SHADER_WRITE,
        RS_READ_WRITE = RAY_TRACING_SHADER_READ_WRITE,
        RS_READ_WRITE_CONCURRENT = RAY_TRACING_SHADER_READ_WRITE_CONCURRENT,
        TASK_SHADER_READ,
        TASK_SHADER_WRITE,
        TASK_SHADER_READ_WRITE,
        TASK_SHADER_READ_WRITE_CONCURRENT,
        TS_READ = TASK_SHADER_READ,
        TS_WRITE = TASK_SHADER_WRITE,
        TS_READ_WRITE = TASK_SHADER_READ_WRITE,
        TS_READ_WRITE_CONCURRENT = TASK_SHADER_READ_WRITE_CONCURRENT,
        MESH_SHADER_READ,
        MESH_SHADER_WRITE,
        MESH_SHADER_READ_WRITE,
        MESH_SHADER_READ_WRITE_CONCURRENT,
        MS_READ = MESH_SHADER_READ,
        MS_WRITE = MESH_SHADER_WRITE,
        MS_READ_WRITE = MESH_SHADER_READ_WRITE,
        MS_READ_WRITE_CONCURRENT = MESH_SHADER_READ_WRITE_CONCURRENT,
        VERTEX_SHADER_READ,
        VERTEX_SHADER_WRITE,
        VERTEX_SHADER_READ_WRITE,
        VERTEX_SHADER_READ_WRITE_CONCURRENT,
        VS_READ = VERTEX_SHADER_READ,
        VS_WRITE = VERTEX_SHADER_WRITE,
        VS_READ_WRITE = VERTEX_SHADER_READ_WRITE,
        VS_READ_WRITE_CONCURRENT = VERTEX_SHADER_READ_WRITE_CONCURRENT,
        TESSELLATION_CONTROL_SHADER_READ,
        TESSELLATION_CONTROL_SHADER_WRITE,
        TESSELLATION_CONTROL_SHADER_READ_WRITE,
        TESSELLATION_CONTROL_SHADER_READ_WRITE_CONCURRENT,
        TCS_READ = TESSELLATION_CONTROL_SHADER_READ,
        TCS_WRITE = TESSELLATION_CONTROL_SHADER_WRITE,
        TCS_READ_WRITE = TESSELLATION_CONTROL_SHADER_READ_WRITE,
        TCS_READ_WRITE_CONCURRENT = TESSELLATION_CONTROL_SHADER_READ_WRITE_CONCURRENT,
        TESSELLATION_EVALUATION_SHADER_READ,
        TESSELLATION_EVALUATION_SHADER_WRITE,
        TESSELLATION_EVALUATION_SHADER_READ_WRITE,
        TESSELLATION_EVALUATION_SHADER_READ_WRITE_CONCURRENT,
        TES_READ = TESSELLATION_EVALUATION_SHADER_READ,
        TES_WRITE = TESSELLATION_EVALUATION_SHADER_WRITE,
        TES_READ_WRITE = TESSELLATION_EVALUATION_SHADER_READ_WRITE,
        TES_READ_WRITE_CONCURRENT = TESSELLATION_EVALUATION_SHADER_READ_WRITE_CONCURRENT,
        GEOMETRY_SHADER_READ,
        GEOMETRY_SHADER_WRITE,
        GEOMETRY_SHADER_READ_WRITE,
        GEOMETRY_SHADER_READ_WRITE_CONCURRENT,
        GS_READ = GEOMETRY_SHADER_READ,
        GS_WRITE = GEOMETRY_SHADER_WRITE,
        GS_READ_WRITE = GEOMETRY_SHADER_READ_WRITE,
        GS_READ_WRITE_CONCURRENT = GEOMETRY_SHADER_READ_WRITE_CONCURRENT,
        FRAGMENT_SHADER_READ,
        FRAGMENT_SHADER_WRITE,
        FRAGMENT_SHADER_READ_WRITE,
        FRAGMENT_SHADER_READ_WRITE_CONCURRENT,
        FS_READ = FRAGMENT_SHADER_READ,
        FS_WRITE = FRAGMENT_SHADER_WRITE,
        FS_READ_WRITE = FRAGMENT_SHADER_READ_WRITE,
        FS_READ_WRITE_CONCURRENT = FRAGMENT_SHADER_READ_WRITE_CONCURRENT,
        INDEX_READ,
        DRAW_INDIRECT_INFO_READ,
        TRANSFER_READ,
        TRANSFER_WRITE,
        TRANSFER_READ_WRITE,
        HOST_TRANSFER_READ,
        HOST_TRANSFER_WRITE,
        HOST_TRANSFER_READ_WRITE,
        ACCELERATION_STRUCTURE_BUILD_READ,
        ACCELERATION_STRUCTURE_BUILD_WRITE,
        ACCELERATION_STRUCTURE_BUILD_READ_WRITE,
        IDX_READ = INDEX_READ,
        DII_READ = DRAW_INDIRECT_INFO_READ,
        T_READ = TRANSFER_READ,
        T_WRITE = TRANSFER_WRITE,
        T_READ_WRITE = TRANSFER_READ_WRITE,
        HT_READ = HOST_TRANSFER_READ,
        HT_WRITE = HOST_TRANSFER_WRITE,
        HT_READ_WRITE = HOST_TRANSFER_READ_WRITE,
        ASB_READ,
        ASB_WRITE,
        ASB_READ_WRITE,
        MAX_ENUM = 0x7fffffff,
    };

    [[nodiscard]] DAXA_EXPORT_CXX auto to_string(TaskBufferAccess const & usage) -> std::string_view;

    enum struct TaskBlasAccess
    {
        NONE = static_cast<u32>(TaskBufferAccess::NONE),
        READ = static_cast<u32>(TaskBufferAccess::READ),
        WRITE = static_cast<u32>(TaskBufferAccess::WRITE),
        READ_WRITE = static_cast<u32>(TaskBufferAccess::READ_WRITE),
        TRANSFER_READ = static_cast<u32>(TaskBufferAccess::TRANSFER_READ),
        TRANSFER_WRITE = static_cast<u32>(TaskBufferAccess::TRANSFER_WRITE),
        HOST_TRANSFER_READ = static_cast<u32>(TaskBufferAccess::HOST_TRANSFER_READ),
        HOST_TRANSFER_WRITE = static_cast<u32>(TaskBufferAccess::HOST_TRANSFER_WRITE),
        BUILD_READ = static_cast<u32>(TaskBufferAccess::ACCELERATION_STRUCTURE_BUILD_READ),
        BUILD_WRITE = static_cast<u32>(TaskBufferAccess::ACCELERATION_STRUCTURE_BUILD_WRITE),
        BUILD_READ_WRITE = static_cast<u32>(TaskBufferAccess::ACCELERATION_STRUCTURE_BUILD_READ_WRITE),
        MAX_ENUM = 0x7fffffff,
    };

    [[nodiscard]] DAXA_EXPORT_CXX auto to_string(TaskBlasAccess const & usage) -> std::string_view;

    enum struct TaskTlasAccess
    {
        NONE = static_cast<u32>(TaskBufferAccess::NONE),
        READ = static_cast<u32>(TaskBufferAccess::READ),
        WRITE = static_cast<u32>(TaskBufferAccess::WRITE),
        READ_WRITE = static_cast<u32>(TaskBufferAccess::READ_WRITE),
        TRANSFER_READ = static_cast<u32>(TaskBufferAccess::TRANSFER_READ),
        TRANSFER_WRITE = static_cast<u32>(TaskBufferAccess::TRANSFER_WRITE),
        HOST_TRANSFER_READ = static_cast<u32>(TaskBufferAccess::HOST_TRANSFER_READ),
        HOST_TRANSFER_WRITE = static_cast<u32>(TaskBufferAccess::HOST_TRANSFER_WRITE),
        BUILD_READ = static_cast<u32>(TaskBufferAccess::ACCELERATION_STRUCTURE_BUILD_READ),
        BUILD_WRITE = static_cast<u32>(TaskBufferAccess::ACCELERATION_STRUCTURE_BUILD_WRITE),
        BUILD_READ_WRITE = static_cast<u32>(TaskBufferAccess::ACCELERATION_STRUCTURE_BUILD_READ_WRITE),
        GRAPHICS_SHADER_READ = static_cast<u32>(TaskBufferAccess::GRAPHICS_SHADER_READ),
        COMPUTE_SHADER_READ = static_cast<u32>(TaskBufferAccess::COMPUTE_SHADER_READ),
        RAY_TRACING_SHADER_READ = static_cast<u32>(TaskBufferAccess::RAY_TRACING_SHADER_READ),
        TASK_SHADER_READ = static_cast<u32>(TaskBufferAccess::TASK_SHADER_READ),
        MESH_SHADER_READ = static_cast<u32>(TaskBufferAccess::MESH_SHADER_READ),
        VERTEX_SHADER_READ = static_cast<u32>(TaskBufferAccess::VERTEX_SHADER_READ),
        TESSELLATION_CONTROL_SHADER_READ = static_cast<u32>(TaskBufferAccess::TESSELLATION_CONTROL_SHADER_READ),
        TESSELLATION_EVALUATION_SHADER_READ = static_cast<u32>(TaskBufferAccess::TESSELLATION_EVALUATION_SHADER_READ),
        GEOMETRY_SHADER_READ = static_cast<u32>(TaskBufferAccess::GEOMETRY_SHADER_READ),
        FRAGMENT_SHADER_READ = static_cast<u32>(TaskBufferAccess::FRAGMENT_SHADER_READ),
        MAX_ENUM = 0x7fffffff,
    };

    [[nodiscard]] DAXA_EXPORT_CXX auto to_string(TaskTlasAccess const & usage) -> std::string_view;

    enum struct TaskImageAccess
    {
        NONE,
        SHADER_SAMPLED,
        SHADER_STORAGE_WRITE_ONLY,
        SHADER_STORAGE_READ_ONLY,
        SHADER_STORAGE_READ_WRITE,
        SHADER_STORAGE_READ_WRITE_CONCURRENT,
        SAMPLED = SHADER_SAMPLED,
        WRITE = SHADER_STORAGE_WRITE_ONLY,
        READ = SHADER_STORAGE_READ_ONLY,
        READ_WRITE = SHADER_STORAGE_READ_WRITE,
        READ_WRITE_CONCURRENT = SHADER_STORAGE_READ_WRITE_CONCURRENT,
        GRAPHICS_SHADER_SAMPLED,
        GRAPHICS_SHADER_STORAGE_WRITE_ONLY,
        GRAPHICS_SHADER_STORAGE_READ_ONLY,
        GRAPHICS_SHADER_STORAGE_READ_WRITE,
        GRAPHICS_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        GPS_SAMPLED = GRAPHICS_SHADER_SAMPLED,
        GPS_WRITE = GRAPHICS_SHADER_STORAGE_WRITE_ONLY,
        GPS_READ = GRAPHICS_SHADER_STORAGE_READ_ONLY,
        GPS_READ_WRITE = GRAPHICS_SHADER_STORAGE_READ_WRITE,
        GPS_READ_WRITE_CONCURRENT = GRAPHICS_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        COMPUTE_SHADER_SAMPLED,
        COMPUTE_SHADER_STORAGE_WRITE_ONLY,
        COMPUTE_SHADER_STORAGE_READ_ONLY,
        COMPUTE_SHADER_STORAGE_READ_WRITE,
        COMPUTE_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        CS_SAMPLED = COMPUTE_SHADER_SAMPLED,
        CS_WRITE = COMPUTE_SHADER_STORAGE_WRITE_ONLY,
        CS_READ = COMPUTE_SHADER_STORAGE_READ_ONLY,
        CS_READ_WRITE = COMPUTE_SHADER_STORAGE_READ_WRITE,
        CS_READ_WRITE_CONCURRENT = COMPUTE_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        RAY_TRACING_SHADER_SAMPLED,
        RAY_TRACING_SHADER_STORAGE_WRITE_ONLY,
        RAY_TRACING_SHADER_STORAGE_READ_ONLY,
        RAY_TRACING_SHADER_STORAGE_READ_WRITE,
        RAY_TRACING_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        RS_SAMPLED = RAY_TRACING_SHADER_SAMPLED,
        RS_WRITE = RAY_TRACING_SHADER_STORAGE_WRITE_ONLY,
        RS_READ = RAY_TRACING_SHADER_STORAGE_READ_ONLY,
        RS_READ_WRITE = RAY_TRACING_SHADER_STORAGE_READ_WRITE,
        RS_READ_WRITE_CONCURRENT = RAY_TRACING_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        TASK_SHADER_SAMPLED,
        TASK_SHADER_STORAGE_WRITE_ONLY,
        TASK_SHADER_STORAGE_READ_ONLY,
        TASK_SHADER_STORAGE_READ_WRITE,
        TASK_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        TS_SAMPLED = TASK_SHADER_SAMPLED,
        TS_WRITE = TASK_SHADER_STORAGE_WRITE_ONLY,
        TS_READ = TASK_SHADER_STORAGE_READ_ONLY,
        TS_READ_WRITE = TASK_SHADER_STORAGE_READ_WRITE,
        TS_READ_WRITE_CONCURRENT = TASK_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        MESH_SHADER_SAMPLED,
        MESH_SHADER_STORAGE_WRITE_ONLY,
        MESH_SHADER_STORAGE_READ_ONLY,
        MESH_SHADER_STORAGE_READ_WRITE,
        MESH_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        MS_SAMPLED = MESH_SHADER_SAMPLED,
        MS_WRITE = MESH_SHADER_STORAGE_WRITE_ONLY,
        MS_READ = MESH_SHADER_STORAGE_READ_ONLY,
        MS_READ_WRITE = MESH_SHADER_STORAGE_READ_WRITE,
        MS_READ_WRITE_CONCURRENT = MESH_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        VERTEX_SHADER_SAMPLED,
        VERTEX_SHADER_STORAGE_WRITE_ONLY,
        VERTEX_SHADER_STORAGE_READ_ONLY,
        VERTEX_SHADER_STORAGE_READ_WRITE,
        VERTEX_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        VS_SAMPLED = VERTEX_SHADER_SAMPLED,
        VS_WRITE = VERTEX_SHADER_STORAGE_WRITE_ONLY,
        VS_READ = VERTEX_SHADER_STORAGE_READ_ONLY,
        VS_READ_WRITE = VERTEX_SHADER_STORAGE_READ_WRITE,
        VS_READ_WRITE_CONCURRENT = VERTEX_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        TESSELLATION_CONTROL_SHADER_SAMPLED,
        TESSELLATION_CONTROL_SHADER_STORAGE_WRITE_ONLY,
        TESSELLATION_CONTROL_SHADER_STORAGE_READ_ONLY,
        TESSELLATION_CONTROL_SHADER_STORAGE_READ_WRITE,
        TESSELLATION_CONTROL_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        TCS_SAMPLED = TESSELLATION_CONTROL_SHADER_SAMPLED,
        TCS_WRITE = TESSELLATION_CONTROL_SHADER_STORAGE_WRITE_ONLY,
        TCS_READ = TESSELLATION_CONTROL_SHADER_STORAGE_READ_ONLY,
        TCS_READ_WRITE = TESSELLATION_CONTROL_SHADER_STORAGE_READ_WRITE,
        TCS_READ_WRITE_CONCURRENT = TESSELLATION_CONTROL_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        TESSELLATION_EVALUATION_SHADER_SAMPLED,
        TESSELLATION_EVALUATION_SHADER_STORAGE_WRITE_ONLY,
        TESSELLATION_EVALUATION_SHADER_STORAGE_READ_ONLY,
        TESSELLATION_EVALUATION_SHADER_STORAGE_READ_WRITE,
        TESSELLATION_EVALUATION_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        TES_SAMPLED = TESSELLATION_EVALUATION_SHADER_SAMPLED,
        TES_WRITE = TESSELLATION_EVALUATION_SHADER_STORAGE_WRITE_ONLY,
        TES_READ = TESSELLATION_EVALUATION_SHADER_STORAGE_READ_ONLY,
        TES_READ_WRITE = TESSELLATION_EVALUATION_SHADER_STORAGE_READ_WRITE,
        TES_READ_WRITE_CONCURRENT = TESSELLATION_EVALUATION_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        GEOMETRY_SHADER_SAMPLED,
        GEOMETRY_SHADER_STORAGE_WRITE_ONLY,
        GEOMETRY_SHADER_STORAGE_READ_ONLY,
        GEOMETRY_SHADER_STORAGE_READ_WRITE,
        GEOMETRY_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        GS_SAMPLED = GEOMETRY_SHADER_SAMPLED,
        GS_WRITE = GEOMETRY_SHADER_STORAGE_WRITE_ONLY,
        GS_READ = GEOMETRY_SHADER_STORAGE_READ_ONLY,
        GS_READ_WRITE = GEOMETRY_SHADER_STORAGE_READ_WRITE,
        GS_READ_WRITE_CONCURRENT = GEOMETRY_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        FRAGMENT_SHADER_SAMPLED,
        FRAGMENT_SHADER_STORAGE_WRITE_ONLY,
        FRAGMENT_SHADER_STORAGE_READ_ONLY,
        FRAGMENT_SHADER_STORAGE_READ_WRITE,
        FRAGMENT_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        FS_SAMPLED = FRAGMENT_SHADER_SAMPLED,
        FS_WRITE = FRAGMENT_SHADER_STORAGE_WRITE_ONLY,
        FS_READ = FRAGMENT_SHADER_STORAGE_READ_ONLY,
        FS_READ_WRITE = FRAGMENT_SHADER_STORAGE_READ_WRITE,
        FS_READ_WRITE_CONCURRENT = FRAGMENT_SHADER_STORAGE_READ_WRITE_CONCURRENT,
        TRANSFER_READ,
        TRANSFER_WRITE,
        T_READ = TRANSFER_READ,
        T_WRITE = TRANSFER_WRITE,
        COLOR_ATTACHMENT,
        DEPTH_ATTACHMENT,
        STENCIL_ATTACHMENT,
        DEPTH_STENCIL_ATTACHMENT,
        DEPTH_ATTACHMENT_READ,
        STENCIL_ATTACHMENT_READ,
        DEPTH_STENCIL_ATTACHMENT_READ,
        CA = COLOR_ATTACHMENT,
        DA = DEPTH_ATTACHMENT,
        SA = STENCIL_ATTACHMENT,
        DSA = DEPTH_STENCIL_ATTACHMENT,
        DA_READ = DEPTH_ATTACHMENT_READ,
        SA_READ = STENCIL_ATTACHMENT_READ,
        DSA_READ = DEPTH_STENCIL_ATTACHMENT_READ,
        RESOLVE_WRITE,
        PRESENT,
        MAX_ENUM = 0x7fffffff,
    };

    [[nodiscard]] DAXA_EXPORT_CXX auto to_string(TaskImageAccess const & usage) -> std::string_view;

    using TaskResourceIndex = u32;

    struct DAXA_EXPORT_CXX TaskGPUResourceView
    {
        TaskResourceIndex task_graph_index = {};
        TaskResourceIndex index = {};

        auto is_empty() const -> bool;
        auto is_persistent() const -> bool;
        auto is_null() const -> bool;

        auto operator<=>(TaskGPUResourceView const & other) const = default;
    };

    [[nodiscard]] DAXA_EXPORT_CXX auto to_string(TaskGPUResourceView const & id) -> std::string;

    struct TaskBufferView : public TaskGPUResourceView
    {
        using ID_T = BufferId;
    };

    struct TaskBlasView : public TaskGPUResourceView
    {
        using ID_T = BlasId;
    };

    struct TaskTlasView : public TaskGPUResourceView
    {
        using ID_T = TlasId;
    };

    struct TaskAttachmentInfo;

    struct TaskImageView : public TaskGPUResourceView
    {
        daxa::ImageMipArraySlice slice = {};
        auto view(daxa::ImageMipArraySlice const & new_slice) const -> TaskImageView
        {
            auto ret = *this;
            ret.slice = new_slice;
            return ret;
        }
        auto mips(u32 base_mip_level, u32 mip_count = 1) const -> TaskImageView
        {
            return view({.base_mip_level = base_mip_level, .level_count = mip_count});
        }
        auto layers(u32 base_array_layer, u32 layer_count = 1) const -> TaskImageView
        {
            return view({.base_array_layer = base_array_layer, .layer_count = layer_count});
        }
        auto operator<=>(TaskGPUResourceView const & other) const = delete;
        auto operator<=>(TaskImageView const & other) const = default;
    };

    static constexpr inline TaskBufferView NullTaskBuffer = []()
    {
        TaskBufferView ret = {};
        ret.task_graph_index = std::numeric_limits<u32>::max();
        ret.index = std::numeric_limits<u32>::max();
        return ret;
    }();

    static constexpr inline TaskBlasView NullTaskBlas = {NullTaskBuffer};

    static constexpr inline TaskTlasView NullTaskTlas = {NullTaskBuffer};

    static constexpr inline TaskImageView NullTaskImage = []()
    {
        TaskImageView ret = {};
        ret.task_graph_index = std::numeric_limits<u32>::max();
        ret.index = std::numeric_limits<u32>::max();
        return ret;
    }();

    struct ImageSliceState
    {
        Access latest_access = {};
        ImageLayout latest_layout = {};
        ImageMipArraySlice slice = {};
    };

    enum struct TaskHeadImageArrayType : u8
    {
        RUNTIME_IMAGES,
        MIP_LEVELS,
    };

    enum struct TaskAttachmentType
    {
        UNDEFINED,
        BUFFER,
        BLAS,
        TLAS,
        IMAGE,
        MAX_ENUM = 0x7fffffff,
    };

    struct TaskBufferAttachmentIndex
    {
        u32 value;
    };

    struct TaskBlasAttachmentIndex
    {
        u32 value;
    };

    struct TaskTlasAttachmentIndex
    {
        u32 value;
    };

    struct TaskImageAttachmentIndex
    {
        u32 value;
    };

    template <typename T>
    concept TaskBufferIndexOrView = std::is_same_v<T, TaskBufferAttachmentIndex> || std::is_same_v<T, TaskBufferView>;
    template <typename T>
    concept TaskBlasIndexOrView = std::is_same_v<T, TaskBlasAttachmentIndex> || std::is_same_v<T, TaskBlasView>;
    template <typename T>
    concept TaskTlasIndexOrView = std::is_same_v<T, TaskTlasAttachmentIndex> || std::is_same_v<T, TaskTlasView>;
    template <typename T>
    concept TaskImageIndexOrView = std::is_same_v<T, TaskImageAttachmentIndex> || std::is_same_v<T, TaskImageView>;
    template <typename T>
    concept TaskIndexOrView = TaskBufferIndexOrView<T> || TaskBlasIndexOrView<T> || TaskTlasIndexOrView<T> || TaskImageIndexOrView<T>;
    template <typename T>
    concept TaskBufferBlasOrTlasIndexOrView = TaskBufferIndexOrView<T> || TaskBlasIndexOrView<T> || TaskTlasIndexOrView<T>;

    struct UndefinedAttachment
    {
    };

    struct TaskBufferAttachment
    {
        using INDEX_TYPE = TaskBufferAttachmentIndex;
        char const * name = {};
        TaskBufferAccess task_access = {};
        Access access = {};
        u8 shader_array_size = {};
        bool shader_as_address = {};
    };

    struct TaskBlasAttachment
    {
        using INDEX_TYPE = TaskBlasAttachmentIndex;
        char const * name = {};
        TaskBlasAccess task_access = {};
        Access access = {};
    };

    struct TaskTlasAttachment
    {
        using INDEX_TYPE = TaskTlasAttachmentIndex;
        char const * name = {};
        TaskTlasAccess task_access = {};
        Access access = {};
        bool shader_as_address = {};
    };

    struct TaskImageAttachment
    {
        using INDEX_TYPE = TaskImageAttachmentIndex;
        char const * name = {};
        TaskImageAccess task_access = {};
        Access access = {};
        ImageViewType view_type = ImageViewType::MAX_ENUM;
        u8 shader_array_size = {};
        bool shader_as_index = {};
        TaskHeadImageArrayType shader_array_type = {};
    };

    struct TaskBufferInlineAttachment
    {
        char const * name = {};
        TaskBufferAccess access = {};
        u8 shader_array_size = {};
        bool shader_as_address = {};
        TaskBufferView view = {};
    };

    struct TaskBlasInlineAttachment
    {
        char const * name = {};
        TaskBlasAccess access = {};
        TaskBlasView view = {};
    };

    struct TaskTlasInlineAttachment
    {
        char const * name = {};
        TaskTlasAccess access = {};
        TaskTlasView view = {};
    };

    struct TaskImageInlineAttachment
    {
        char const * name = {};
        TaskImageAccess access = {};
        ImageViewType view_type = ImageViewType::MAX_ENUM;
        u8 shader_array_size = {};
        TaskHeadImageArrayType shader_array_type = {};
        TaskImageView view = {};
    };

    template <typename T>
    concept IsTaskResourceAttachment =
        std::is_same_v<T, TaskBufferAttachment> ||
        std::is_same_v<T, TaskBlasAttachment> ||
        std::is_same_v<T, TaskTlasAttachment> ||
        std::is_same_v<T, TaskImageAttachment>;

    struct TaskAttachment
    {
        TaskAttachmentType type = TaskAttachmentType::UNDEFINED;
        union Value
        {
            UndefinedAttachment undefined;
            TaskBufferAttachment buffer;
            TaskBlasAttachment blas;
            TaskTlasAttachment tlas;
            TaskImageAttachment image;
        } value = {.undefined = {}};

        constexpr TaskAttachment() = default;

        constexpr TaskAttachment(TaskBufferAttachment const & buffer)
            : type{TaskAttachmentType::BUFFER}, value{.buffer = buffer}
        {
        }

        constexpr TaskAttachment(TaskBlasAttachment const & blas)
            : type{TaskAttachmentType::BLAS}, value{.blas = blas}
        {
        }

        constexpr TaskAttachment(TaskTlasAttachment const & tlas)
            : type{TaskAttachmentType::TLAS}, value{.tlas = tlas}
        {
        }

        constexpr TaskAttachment(TaskImageAttachment const & image)
            : type{TaskAttachmentType::IMAGE}, value{.image = image}
        {
        }

        constexpr auto name() const -> char const *
        {
            switch (type)
            {
            case TaskAttachmentType::BUFFER: return value.buffer.name;
            case TaskAttachmentType::BLAS: return value.blas.name;
            case TaskAttachmentType::TLAS: return value.tlas.name;
            case TaskAttachmentType::IMAGE: return value.image.name;
            default: return "undefined";
            }
        }

        constexpr auto shader_array_size() const -> u32
        {
            switch (type)
            {
            case TaskAttachmentType::BUFFER: return value.buffer.shader_array_size * 8;
            case TaskAttachmentType::BLAS: return 0;
            case TaskAttachmentType::TLAS: return 8;
            case TaskAttachmentType::IMAGE: return value.image.shader_array_size * (value.image.shader_as_index ? 4 : 8);
            default: return 0;
            }
        }

        constexpr auto shader_element_align() const -> u32
        {
            switch (type)
            {
            case TaskAttachmentType::BUFFER: return 8;
            case TaskAttachmentType::BLAS: return 8;
            case TaskAttachmentType::TLAS: return 8;
            case TaskAttachmentType::IMAGE: return value.image.shader_as_index ? 4 : 8;
            default: return 0;
            }
        }
    };

    struct UndefinedAttachmentRuntimeData
    {
    };

    struct TaskBufferAttachmentInfo : TaskBufferAttachment
    {
        TaskBufferView view = {};
        TaskBufferView translated_view = {};
        std::span<BufferId const> ids = {};
    };

    struct TaskBlasAttachmentInfo : TaskBlasAttachment
    {
        TaskBlasView view = {};
        TaskBlasView translated_view = {};
        std::span<BlasId const> ids = {};
    };

    struct TaskTlasAttachmentInfo : TaskTlasAttachment
    {
        TaskTlasView view = {};
        TaskTlasView translated_view = {};
        std::span<TlasId const> ids = {};
    };

    struct TaskImageAttachmentInfo : TaskImageAttachment
    {
        TaskImageView view = {};
        TaskImageView translated_view = {};
        ImageLayout layout = {};
        std::span<ImageId const> ids = {};
        std::span<ImageViewId const> view_ids = {};
    };

    struct TaskAttachmentInfo
    {
        TaskAttachmentType type = TaskAttachmentType::UNDEFINED;
        union Value
        {
            UndefinedAttachment undefined;
            TaskBufferAttachmentInfo buffer;
            TaskBlasAttachmentInfo blas;
            TaskTlasAttachmentInfo tlas;
            TaskImageAttachmentInfo image;
        } value = {.undefined = {}};

        constexpr TaskAttachmentInfo() = default;

        constexpr TaskAttachmentInfo(TaskBufferAttachmentInfo const & buffer)
            : type{TaskAttachmentType::BUFFER}, value{.buffer = buffer}
        {
        }

        constexpr TaskAttachmentInfo(TaskBlasAttachmentInfo const & blas)
            : type{TaskAttachmentType::BLAS}, value{.blas = blas}
        {
        }

        constexpr TaskAttachmentInfo(TaskTlasAttachmentInfo const & tlas)
            : type{TaskAttachmentType::TLAS}, value{.tlas = tlas}
        {
        }

        constexpr TaskAttachmentInfo(TaskImageAttachmentInfo const & image)
            : type{TaskAttachmentType::IMAGE}, value{.image = image}
        {
        }

        constexpr auto name() const -> char const *
        {
            switch (type)
            {
            case TaskAttachmentType::BUFFER: return value.buffer.name;
            case TaskAttachmentType::BLAS: return value.blas.name;
            case TaskAttachmentType::TLAS: return value.tlas.name;
            case TaskAttachmentType::IMAGE: return value.image.name;
            default: return "undefined";
            }
        }

        constexpr auto shader_array_size() const -> u32
        {
            switch (type)
            {
            case TaskAttachmentType::BUFFER: return value.buffer.shader_array_size * 8;
            case TaskAttachmentType::BLAS: return 0;
            case TaskAttachmentType::TLAS: return 8;
            case TaskAttachmentType::IMAGE: return value.image.shader_array_size * (value.image.shader_as_index ? 4 : 8);
            default: return 0;
            }
        }

        constexpr auto shader_element_align() const -> u32
        {
            switch (type)
            {
            case TaskAttachmentType::BUFFER: return 8;
            case TaskAttachmentType::BLAS: return 8;
            case TaskAttachmentType::TLAS: return 8;
            case TaskAttachmentType::IMAGE: return value.image.shader_as_index ? 4 : 8;
            default: return 0;
            }
        }
    };

    using TaskAttachmentInfoVariant = Variant<
        TaskBufferAttachmentInfo,
        TaskBlasAttachmentInfo,
        TaskTlasAttachmentInfo,
        TaskImageAttachmentInfo>;

    struct DAXA_EXPORT_CXX TaskInterface
    {
        Device & device;
        CommandRecorder & recorder;
        std::span<TaskAttachmentInfo const> attachment_infos = {};
        // optional:
        TransferMemoryPool * allocator = {};
        std::span<std::byte const> attachment_shader_blob = {};
        std::string_view task_name = {};
        usize task_index = {};

#if !DAXA_REMOVE_DEPRECATED
        [[deprecated("Use AttachmentBlob(std::span<std::byte const>) constructor instead, API:3.0")]] void assign_attachment_shader_blob(std::span<std::byte> arr) const
        {
            std::memcpy(
                arr.data(),
                attachment_shader_blob.data(),
                attachment_shader_blob.size());
        }
#endif

        auto get(TaskBufferAttachmentIndex index) const -> TaskBufferAttachmentInfo const &;
        auto get(TaskBufferView view) const -> TaskBufferAttachmentInfo const &;
        auto get(TaskBlasAttachmentIndex index) const -> TaskBlasAttachmentInfo const &;
        auto get(TaskBlasView view) const -> TaskBlasAttachmentInfo const &;
        auto get(TaskTlasAttachmentIndex index) const -> TaskTlasAttachmentInfo const &;
        auto get(TaskTlasView view) const -> TaskTlasAttachmentInfo const &;
        auto get(TaskImageAttachmentIndex index) const -> TaskImageAttachmentInfo const &;
        auto get(TaskImageView view) const -> TaskImageAttachmentInfo const &;
        auto get(usize index) const -> TaskAttachmentInfo const &;

        auto info(TaskIndexOrView auto tresource, u32 array_index = 0) const
        {
            return this->device.info(this->get(tresource).ids[array_index]);
        }
        auto image_view_info(TaskImageIndexOrView auto timage, u32 array_index = 0) const -> Optional<ImageViewInfo>
        {
            return this->device.image_view_info(this->get(timage).view_ids[array_index]);
        }
        auto device_address(TaskBufferBlasOrTlasIndexOrView auto tresource, u32 array_index = 0) const -> Optional<DeviceAddress>
        {
            return this->device.device_address(this->get(tresource).ids[array_index]);
        }
        auto buffer_host_address(TaskBufferIndexOrView auto tbuffer, u32 array_index = 0) const -> Optional<std::byte *>
        {
            return this->device.buffer_host_address(this->get(tbuffer).ids[array_index]);
        }
        auto id(TaskIndexOrView auto tresource, u32 index = 0)
        {
            return this->get(tresource).ids[index];
        }
        auto view(TaskImageIndexOrView auto timg, u32 index = 0)
        {
            return this->get(timg).view_ids[index];
        }
    };

    using TaskViewVariant = Variant<
        std::pair<daxa::TaskBufferAttachmentIndex, daxa::TaskBufferView>,
        std::pair<daxa::TaskBlasAttachmentIndex, daxa::TaskBlasView>,
        std::pair<daxa::TaskTlasAttachmentIndex, daxa::TaskTlasView>,
        std::pair<daxa::TaskImageAttachmentIndex, daxa::TaskImageView>>;

    inline namespace detail
    {
        struct AsbSizeAlignment
        {
            u32 size = {};
            u32 alignment = {};
        };
        constexpr auto get_asb_size_and_alignment(auto const & attachment_array) -> AsbSizeAlignment
        {
            AsbSizeAlignment size_align = {};
            auto align_up = [](auto value, auto align) -> u32
            {
                if (value == 0 || align == 0)
                    return 0;
                return (value + align - 1u) / align * align;
            };
            for (auto const & attachment_decl : attachment_array)
            {
                if (attachment_decl.shader_array_size() == 0 || attachment_decl.shader_element_align() == 0)
                    continue;
                size_align.size = align_up(size_align.size, attachment_decl.shader_element_align());
                size_align.alignment = std::max(size_align.alignment, attachment_decl.shader_element_align());
                size_align.size += attachment_decl.shader_array_size();
            }
            size_align.size = align_up(size_align.size, size_align.alignment);
            return size_align;
        }
    } // namespace detail

    struct ITask
    {
        constexpr virtual ~ITask() {}
        /// TODO(pahrens): optimize:
        constexpr virtual auto attachment_shader_blob_size() const -> u32
        {
            return detail::get_asb_size_and_alignment(attachments()).size;
        };
        constexpr virtual auto attachments() -> std::span<TaskAttachmentInfo> = 0;
        constexpr virtual auto attachments() const -> std::span<TaskAttachmentInfo const> = 0;
        constexpr virtual std::string_view name() const = 0;
        virtual void callback(TaskInterface){};
    };

    template <usize N>
    struct StringLiteral
    {
        constexpr StringLiteral(char const (&str)[N])
        {
            std::ranges::copy_n(str, N - 1, value);
        }
        char value[N - 1];
        usize SIZE = N - 1;
    };

    // Used for simpler concept template constraint in add_task.
    struct IPartialTask
    {
    };

    template <usize ATTACHMENT_COUNT>
    struct AttachmentViews
    {
        AttachmentViews(std::array<daxa::TaskViewVariant, ATTACHMENT_COUNT> const & index_view_pairs)
        {
            for (TaskViewVariant const & vari : index_view_pairs)
            {
                if (auto * buffer_pair = get_if<std::pair<daxa::TaskBufferAttachmentIndex, daxa::TaskBufferView>>(&vari))
                {
                    views[buffer_pair->first.value] = buffer_pair->second;
                }
                else if (auto * blas_pair = get_if<std::pair<daxa::TaskBlasAttachmentIndex, daxa::TaskBlasView>>(&vari))
                {
                    views[blas_pair->first.value] = blas_pair->second;
                }
                else if (auto * tlas_pair = get_if<std::pair<daxa::TaskTlasAttachmentIndex, daxa::TaskTlasView>>(&vari))
                {
                    views[tlas_pair->first.value] = tlas_pair->second;
                }
                else
                {
                    auto const & img_pair = get<std::pair<daxa::TaskImageAttachmentIndex, daxa::TaskImageView>>(vari);
                    views[img_pair.first.value] = img_pair.second;
                }
            }
        }
        AttachmentViews() = default;
        std::array<Variant<
                       daxa::TaskBufferView,
                       daxa::TaskBlasView,
                       daxa::TaskTlasView,
                       daxa::TaskImageView>,
                   ATTACHMENT_COUNT>
            views = {};
    };

    template <usize ATTACHMENT_COUNT, StringLiteral NAME>
    struct PartialTask : IPartialTask
    {
        /// NOTE: Used to add attachments and declare named constant indices to the added attachment.
        template <IsTaskResourceAttachment IndexT>
        static auto add_attachment(IndexT const & attach) -> IndexT::INDEX_TYPE
        {
            declared_attachments.at(cur_attach_index) = attach;
            return {cur_attach_index++};
        }
        static auto name() -> std::string_view { return std::string_view{NAME.value, NAME.SIZE}; }
        static auto attachments() -> std::span<TaskAttachment const>
        {
            return declared_attachments;
        }
        static constexpr inline usize ATTACH_COUNT = ATTACHMENT_COUNT;
        static inline std::array<TaskAttachment, ATTACHMENT_COUNT> declared_attachments = {};
        using AttachmentViews = daxa::AttachmentViews<ATTACHMENT_COUNT>;

      private:
        static inline u32 cur_attach_index = 0;
    };

    /*
    ⠀⠀⢀⣀⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠾⠛⠛⠷⣦⡀⠀⠀⠀⠀⠀⠀
    ⢠⣶⠛⠋⠉⡙⢷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⠐⡡⢂⠢⠈⠻⣦⡀⠀⠀⠀⠀
    ⣾⠃⠠⡀⠥⡐⡙⣧⣰⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡹⡜⢄⠣⢤⣩⣦⣸⣧⠀⠀⠀⠀
    ⣿⡀⢢⠑⠢⣵⡿⠛⠉⠉⠉⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣜⢬⣿⠛⠉⠉⠉⠻⣧⡀⠀⠀
    ⣹⣇⠢⣉⣾⡏⠀⠠⠀⢆⠡⣘⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠟⠋⢉⠛⢷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⡆⠱⡈⠔⠠⠄⠈⢷⡄⠀
    ⠀⢿⣦⢡⣿⠀⠌⡐⠩⡄⢊⢵⣇⣠⣀⣀⡀⠀⠀⠀⠀⣼⠟⢁⢀⠂⠆⡌⢢⢿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⡀⠀⠀⠀⠀⣾⣅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡏⢷⡣⠜⣈⠆⡡⠂⠌⣷⠀
    ⠀⠀⢹⡞⣧⠈⡆⢡⠃⣼⣾⡟⠛⠉⠉⠉⠛⣷⡄⠀⢸⡏⠐⢨⡄⡍⠒⣬⢡⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⡟⠁⠀⠀⠀⠀⠑⢻⣶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣴⣾⡖⣶⣦⠀⠀⠀⠀⠀⠀⠀⢸⡏⡜⣷⢱⢨⡆⢱⠈⡆⣿⠀
    ⠀⠀⠀⠽⣇⠎⡰⣩⡼⡟⠁⠄⡀⠠⠀⠀⠀⠈⢿⡄⡿⢄⢃⠖⡰⣉⠖⣡⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣷⡿⠁⣀⣠⣀⣤⣤⣤⣼⣿⣷⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⢯⠋⡀⢀⠀⠉⢿⣆⠀⠀⠀⠀⠀⣸⢗⢡⣿⣂⣖⣨⡱⢊⡔⣿⠀
    ⠀⠀⠀⠀⣯⠒⠥⡾⢇⠰⡉⠔⡠⠃⡌⢐⠡⠀⣼⡟⡓⢌⢒⢪⠑⣌⡾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣿⣿⣿⣻⠭⠿⠛⠒⠓⠚⠛⠛⠿⣿⣿⣿⣿⣳⡶⢦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡚⠤⣁⠢⠐⣀⠀⣿⡀⠀⠀⠀⢈⡟⣸⢟⠉⠁⠀⠉⠙⢷⣴⠇⠀
    ⠀⠀⠀⠀⢿⣩⢲⣟⢌⡒⡱⢊⠴⢡⢘⣄⣢⣽⠞⡑⢌⠂⢎⠤⢋⡞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⣿⣿⣿⠿⠋⡀⢀⠠⠀⠄⠠⠂⠄⠄⡠⠀⠄⡈⠉⠛⠛⠛⡙⠺⣭⡗⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣰⠂⡅⢣⠐⡠⢽⡇⠀⠀⢀⡾⡅⣯⢄⠊⠤⢁⠂⠄⠀⠙⣧⡀
    ⠀⠀⠀⠀⠺⣇⢾⢭⢢⠱⣡⠋⣔⣷⠋⡍⠰⢀⠊⠰⢈⠜⡠⢊⣽⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡶⣿⡿⠛⠛⡉⡁⢄⠂⡔⢠⠂⡅⢊⢡⠘⡐⢌⣠⡑⠢⢐⠡⢊⠔⡡⢂⠅⣂⠙⡳⣎⡟⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣯⠰⢃⡜⢠⢺⡇⠀⢰⡾⠅⠃⢿⣜⠌⡒⢄⢊⡐⡁⢂⠘⣧
    ⠀⠀⠀⠀⠀⢻⣺⡇⢎⡱⢄⡓⣾⠄⢣⠈⠅⡂⠡⠑⡈⢢⠑⢢⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡾⣫⠗⡅⣢⣥⣧⢽⠶⠟⣶⢶⣿⠆⡜⡐⠦⠱⢌⠢⡜⢏⣿⠛⣛⠳⢾⣤⡣⡜⣠⠓⡤⢩⢳⡎⡝⡷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡟⡰⢈⠆⡹⣇⣰⠿⡀⢌⠒⠤⠙⢷⣼⡠⢆⡔⢡⠂⠔⣻
    ⠀⠀⠀⠀⠀⠐⢻⣏⠦⣑⢊⠔⣿⠈⢆⡑⠂⡌⢠⠑⡈⠤⡉⢼⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⢻⢣⠞⣣⢵⣾⡿⢋⠃⢆⠬⣹⠗⡬⡑⢎⠴⣉⠎⣕⢪⡑⢎⠲⡸⢯⣅⡚⠤⡘⡙⠿⣶⣍⡒⠧⢎⠼⣑⢣⢏⢷⣆⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡿⡐⠥⠚⡄⡙⠓⠤⡑⢌⡘⠤⡉⣼⢌⣷⠢⠜⢢⠉⢆⣿
    ⠀⠀⠀⠀⠀⠀⠐⣯⣚⠤⡋⡜⢫⠩⢄⠢⡑⡠⢃⠰⡁⢆⠱⣈⡧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⠏⣎⢣⣾⣿⡋⢍⡰⢌⡚⣌⣾⢋⠳⡰⣉⢎⠲⣡⠚⡤⠣⡜⣌⢣⠱⣩⠙⠷⣧⠵⡨⠜⡨⠻⣿⣇⠮⣑⢎⢣⠞⣬⡙⣯⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⢻⡌⡱⢃⡜⣨⡕⢢⠑⣢⠘⢤⣹⢏⡜⣠⢣⠙⢦⡙⢦⠇
    ⠀⠀⠀⠀⠀⠀⠀⠽⣎⠖⡱⢌⠥⢊⠖⠓⠒⠿⣮⡔⡡⢎⠰⢂⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⡛⣆⡛⣴⣿⡟⠰⣌⠲⡌⢶⢞⡋⢦⡉⠖⣑⣢⣮⣵⣶⣷⣶⣷⣶⣶⣥⣧⣢⡙⢢⡑⢎⡡⡙⠴⣛⠛⡦⢓⢬⠚⣌⡓⢦⡹⢜⡻⣆⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⢜⡢⢱⣡⡿⠛⠛⢒⠳⢤⢋⠴⣛⠣⡔⢢⢎⡙⢦⣱⠟⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⢻⣝⡰⣉⠖⣡⠚⣈⠁⠄⠈⢻⣶⡨⢡⢃⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⢳⠍⣦⠱⣊⠏⡽⣉⢆⠳⢌⠣⢆⡙⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣼⡠⢃⠝⡢⢅⠫⡔⡍⢦⠹⢤⡙⢦⠱⣋⡜⡻⣆⠀⠀⠀⠀⠀⠀⠀⠈⢻⣜⠲⣱⣿⠀⠂⡍⠰⣈⠦⡉⢖⡡⢓⡌⢣⠎⣜⣶⠏⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠠⢻⣖⡡⠞⣄⠓⡄⠣⢐⠀⠀⢻⣿⣥⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⣍⢧⢫⠔⡫⠴⣉⠖⡡⢎⡱⢊⡱⣼⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣎⡑⢎⡱⡘⡜⢢⠝⣢⡙⣌⢳⡑⢮⠱⣹⣆⠀⠀⠀⠀⠀⠀⠀⠈⢿⡱⣿⣿⠀⢃⠌⡱⢠⢒⡉⢦⡑⢣⡜⢣⣾⠞⠁⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⣼⠱⣌⠓⡬⠑⡌⠠⠁⢸⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⢑⠎⡖⣩⢎⡱⢣⠜⡬⡑⢎⠔⣣⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣮⡢⡑⡜⢌⡱⢪⠔⡱⢊⢦⠹⣌⠏⣄⢻⡆⠀⠀⠀⠀⠀⠀⠀⠁⠙⢿⣿⡌⡐⢌⠰⠡⢎⠜⣢⠙⣦⡽⠟⠁⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣦⣝⡰⢩⢌⠱⣈⣾⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣎⠹⡬⣑⠎⣔⠣⣍⠒⡭⢌⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡕⡘⠦⣡⢃⢎⡱⣉⢦⢋⡜⡎⢥⠊⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣔⣈⠒⣍⣢⣽⡴⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠚⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⡿⠐⣌⢓⠲⣉⠞⡤⢓⠬⡑⢆⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣎⢒⡡⠎⢦⠱⡌⠦⡍⠖⣭⠒⡌⢸⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠋⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢼⡇⢢⠙⡜⢦⢋⡴⢡⠚⡤⢓⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡌⢣⠔⡌⢦⠱⢢⠕⡲⣉⠖⣁⠚⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⠁⢢⠹⣌⠳⣌⠲⣡⢋⠴⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡐⢎⡜⢢⡙⢆⢫⠱⣌⠳⣀⠂⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣿⠐⢂⠳⣌⠳⣌⠳⣄⢋⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡰⢌⠣⡜⣌⠣⡝⢤⠳⢄⠂⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⢣⡙⣔⠣⡜⠲⡌⢦⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⢊⠵⡘⢤⡓⢬⢣⡙⠢⠌⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⠈⠴⡱⢌⡳⢌⠳⡘⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣎⠲⣉⠦⡙⣆⢣⠚⡅⠊⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⡈⡱⣘⢣⠜⡬⢣⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡰⡡⢎⡱⡌⡖⣍⠒⡡⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⡇⡒⣍⠮⡜⢆⢣⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⡋⠍⡠⠄⡠⢀⠂⡍⢙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⡑⢮⠰⡱⢜⡢⠍⢤⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣺⣇⣱⢎⢲⣉⠮⢼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢟⡋⠔⣡⠘⠤⡑⢨⠐⡁⢎⠠⢃⠌⡐⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡜⣌⢣⠕⣎⡱⣩⢘⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠩⣷⡐⣏⠦⣃⠞⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⡑⠢⠜⡰⢠⢉⠒⡌⢄⠣⡘⠄⠣⢌⠢⡑⢌⠢⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⠔⣣⢚⡴⣑⡃⢾⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣷⠸⣜⡰⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠋⡴⢉⠜⢢⠑⠢⢌⠒⡌⢢⠑⠤⣉⠲⡈⢆⠱⢌⠢⢡⠃⡽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢜⢢⠣⢖⡱⢌⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣷⢆⡇⣻⣿⣿⣿⣿⣿⣿⣿⣿⡿⣁⠳⢨⠜⡨⢆⣉⠣⣊⠜⣈⠆⣙⡐⢢⠡⣑⢊⠒⡌⢣⢑⡊⡔⣊⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡎⡖⣹⢊⡵⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⢼⡘⣽⣿⣿⣿⣿⣿⣿⣿⢏⠦⣡⢋⠲⢌⡑⠦⢌⡱⡐⠎⡤⠩⢔⠨⡅⢃⠲⢌⡱⢌⡱⢢⠱⡘⢤⣉⠻⣿⣿⣿⣿⣿⣿⣿⣿⡗⣍⠖⣯⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⢺⡱⣚⣿⣿⣿⣿⣿⠟⡕⢎⠲⡡⢎⡱⢊⡜⡘⢆⠲⢡⠓⣌⠓⣌⠓⣌⢃⠳⣈⠲⢌⡒⣡⢣⡙⠦⣌⠣⡍⢿⣿⣿⣿⣿⣿⣿⡹⡰⣋⢿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡟⡠⢇⠼⣻⠿⣟⢣⠟⡸⢜⢣⠣⡜⡠⢇⡸⢣⠜⢣⠇⡛⣄⢛⡀⢟⡀⠟⡤⢃⠻⡄⢣⢄⢣⡘⢇⡄⢧⠛⣤⢘⡿⣿⣿⣿⢟⡣⢣⠇⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠚⣧⡑⡌⠒⡡⠚⢤⣳⡾⣱⠪⣅⠳⢬⠱⣊⠴⣃⠮⡑⢎⡱⢌⠦⣙⢢⡙⡜⡰⣉⠖⣩⠲⡌⢦⡙⠦⡜⢢⠛⡤⢓⡜⣆⠳⡜⢪⡑⢣⠋⣾⢁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣷⣄⣁⣠⣽⡟⢧⡱⢆⡳⣌⢓⡎⡱⣌⠳⣌⢲⣉⠖⡱⢊⠖⣡⢒⡱⣌⡱⡘⣜⢢⢓⡜⣢⡙⣜⡘⣣⢝⡸⣛⢾⣌⡳⢈⠥⠘⠠⢡⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠛⠋⠉⠛⢧⡝⣎⠵⣌⠧⡜⡱⣌⠳⣌⠶⡌⢞⡡⢏⠼⣡⢎⡱⢢⠵⡱⣌⢎⠦⣱⢡⠞⣤⠛⡴⢪⠵⣩⢞⣼⠿⢶⣤⣥⣤⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠳⣽⡘⢮⡱⢳⣌⠳⣜⢢⡝⣢⢝⡸⢲⢡⠞⣰⠣⡞⡱⣌⢎⢞⡰⢣⠞⣔⡫⣜⢣⣿⠶⠋⠀⠀⠀⠈⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⠯⣧⣎⠳⣬⢓⡬⡱⢎⡵⣋⡬⣛⠴⣋⠶⡱⢎⡞⡬⢳⡍⣞⣦⠷⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⡙⠓⠻⠶⠽⢮⣶⣥⣷⣭⣾⣥⣯⡵⠯⠼⠗⠛⠋⣉⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    */

    /// ======================================= BRACE FOR IMPACT =======================================
    /// ============================== HEAVY TEMPLATE METAPROGRAMMING AHEAD ============================
    /// =========================================DO NOT PANIC ==========================================

    struct TaskAttachmentViewWrapperRaw
    {
        TaskAttachmentType type = TaskAttachmentType::UNDEFINED;
        union Value
        {
            u32 undefined;
            TaskBufferView buffer;
            TaskBlasView blas;
            TaskTlasView tlas;
            TaskImageView image;
        } value = {.undefined = {}};
    };

    template <typename T>
    struct TaskAttachmentViewWrapper
    {
        TaskAttachmentViewWrapperRaw _value = {};

        TaskAttachmentViewWrapper() = default;
        TaskAttachmentViewWrapper(TaskBufferView const & view)
            requires(std::is_same_v<T, TaskBufferView>)
            : _value{TaskAttachmentType::BUFFER, {.buffer = view}}
        {
        }
        TaskAttachmentViewWrapper(TaskBlasView const & view)
            requires(std::is_same_v<T, TaskBlasView>)
            : _value{TaskAttachmentType::BLAS, {.blas = view}}
        {
        }
        TaskAttachmentViewWrapper(TaskTlasView const & view)
            requires(std::is_same_v<T, TaskTlasView>)
            : _value{TaskAttachmentType::TLAS, {.tlas = view}}
        {
        }
        TaskAttachmentViewWrapper(TaskImageView const & view)
            requires(std::is_same_v<T, TaskImageView>)
            : _value{TaskAttachmentType::IMAGE, {.image = view}}
        {
        }
    };

    // Used to specialize the TaskHeadStruct.
    // Specialized struct will contain an array of all declared attachments,
    // As well as a named field for each declared attachment with the index of that declaration as a value.
    template <daxa::usize ATTACHMENT_COUNT>
    struct TaskHeadStructSpecializeAttachmentDecls
    {
        struct DeclaredAttachments
        {
            std::array<daxa::TaskAttachment, ATTACHMENT_COUNT> value = {};
            u32 count = {};
        };
        using InternalT = DeclaredAttachments;
        using TaskBufferT = daxa::TaskBufferAttachmentIndex const;
        using TaskBlasT = daxa::TaskBlasAttachmentIndex const;
        using TaskTlasT = daxa::TaskTlasAttachmentIndex const;
        using TaskImageT = daxa::TaskImageAttachmentIndex const;
        constexpr static usize DECL_ATTACHMENTS = true;
        template <typename TaskResourceT>
        static constexpr auto process_attachment_decl(DeclaredAttachments & _internal, auto const & initalizer)
        {
            _internal.value.at(_internal.count) = initalizer;
            return _internal.count++;
        }
    };

    // Used to specialize the TaskHeadStruct.
    // Specialized struct will contain a named field for each declared attachment typed as a view for that attachment.
    template <daxa::usize ATTACHMENT_COUNT>
    struct TaskHeadStructSpecializeAttachmentViews
    {
        using InternalT = u32;
        using TaskBufferT = daxa::TaskAttachmentViewWrapper<TaskBufferView>;
        using TaskBlasT = daxa::TaskAttachmentViewWrapper<TaskBlasView>;
        using TaskTlasT = daxa::TaskAttachmentViewWrapper<TaskTlasView>;
        using TaskImageT = daxa::TaskAttachmentViewWrapper<TaskImageView>;
        constexpr static usize DECL_ATTACHMENTS = false;
        template <typename TaskResourceT>
        static constexpr auto process_attachment_decl(InternalT &, auto const & initalizer)
        {
            return TaskResourceT{};
        }

        static auto convert(auto const & type) -> daxa::AttachmentViews<ATTACHMENT_COUNT>
        {
            // Binary compatible with TaskHeadStruct for Views type.
            struct Extractor
            {
                u32 dummy = {};
                std::array<TaskAttachmentViewWrapperRaw, ATTACHMENT_COUNT> initializers = {};
            };
            auto views = std::bit_cast<Extractor>(type);
            auto ret = daxa::AttachmentViews<ATTACHMENT_COUNT>{};
            for (daxa::u32 i = 0; i < ATTACHMENT_COUNT; ++i)
            {
                switch (views.initializers[i].type)
                {
                case TaskAttachmentType::BUFFER:
                    ret.views[i] = views.initializers[i].value.buffer;
                    break;
                case TaskAttachmentType::BLAS:
                    ret.views[i] = views.initializers[i].value.blas;
                    break;
                case TaskAttachmentType::TLAS:
                    ret.views[i] = views.initializers[i].value.tlas;
                    break;
                case TaskAttachmentType::IMAGE:
                    ret.views[i] = views.initializers[i].value.image;
                    break;
                default:
                    DAXA_DBG_ASSERT_TRUE_M(false, "Invalid attachment type!");
                }
            }
            return ret;
        }
    };

#define DAXA_DECL_TASK_HEAD_BEGIN(HEAD_NAME)                    \
    namespace HEAD_NAME                                         \
    {                                                           \
        static inline constexpr char NAME[] = #HEAD_NAME;       \
        template <typename TDecl, daxa::usize ATTACHMENT_COUNT> \
        struct TaskHeadStruct                                   \
        {                                                       \
            TDecl::InternalT _internal = {};                    \
            operator daxa::AttachmentViews<ATTACHMENT_COUNT>()  \
                requires(!TDecl::DECL_ATTACHMENTS)              \
            {                                                   \
                return TDecl::convert(*this);                   \
            }

#define _DAXA_HELPER_TH_BUFFER(NAME, TASK_ACCESS, ...)              \
    TDecl::TaskBufferT NAME =                                       \
        {TDecl::process_attachment_decl<TDecl::TaskBufferT>(        \
            _internal,                                              \
            daxa::TaskBufferAttachment{                             \
                .name = #NAME,                                      \
                .task_access = daxa::TaskBufferAccess::TASK_ACCESS, \
                __VA_ARGS__})};

#define _DAXA_HELPER_TH_BLAS(NAME, TASK_ACCESS)                   \
    TDecl::TaskBlasT const NAME =                                 \
        {TDecl::process_attachment_decl<TDecl::TaskBlasT>(        \
            _internal,                                            \
            daxa::TaskBlasAttachment{                             \
                .name = #NAME,                                    \
                .task_access = daxa::TaskBlasAccess::TASK_ACCESS, \
            })};

#define _DAXA_HELPER_TH_TLAS(NAME, TASK_ACCESS, ...)              \
    TDecl::TaskTlasT const NAME =                                 \
        {TDecl::process_attachment_decl<TDecl::TaskTlasT>(        \
            _internal,                                            \
            daxa::TaskTlasAttachment{                             \
                .name = #NAME,                                    \
                .task_access = daxa::TaskTlasAccess::TASK_ACCESS, \
                __VA_ARGS__})};

#define _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, ...)              \
    TDecl::TaskImageT const NAME =                                 \
        {TDecl::process_attachment_decl<TDecl::TaskImageT>(        \
            _internal,                                             \
            daxa::TaskImageAttachment{                             \
                .name = #NAME,                                     \
                .task_access = daxa::TaskImageAccess::TASK_ACCESS, \
                __VA_ARGS__})};

#define DAXA_DECL_TASK_HEAD_END                                                                                                                \
    }                                                                                                                                          \
    ;                                                                                                                                          \
    static inline constexpr auto ATTACHMENT_COUNT = TaskHeadStruct<daxa::TaskHeadStructSpecializeAttachmentDecls<256>, 256>{}._internal.count; \
    using ATTACHMENTS_T = TaskHeadStruct<daxa::TaskHeadStructSpecializeAttachmentDecls<ATTACHMENT_COUNT>, ATTACHMENT_COUNT>;                   \
    using VIEWS_T = TaskHeadStruct<daxa::TaskHeadStructSpecializeAttachmentViews<ATTACHMENT_COUNT>, ATTACHMENT_COUNT>;                         \
    static inline constexpr auto ATTACHMENTS = ATTACHMENTS_T{};                                                                                \
    static inline constexpr auto const & AT = ATTACHMENTS;                                                                                     \
    struct alignas(daxa::detail::get_asb_size_and_alignment(AT._internal.value).alignment) AttachmentShaderBlob                                \
    {                                                                                                                                          \
        std::array<std::byte, daxa::detail::get_asb_size_and_alignment(AT._internal.value).size> value = {};                                   \
        AttachmentShaderBlob() = default;                                                                                                      \
        AttachmentShaderBlob(std::span<std::byte const> data) { *this = data; }                                                                \
        auto operator=(std::span<std::byte const> data) -> AttachmentShaderBlob &                                                              \
        {                                                                                                                                      \
            DAXA_DBG_ASSERT_TRUE_M(this->value.size() == data.size(), "Blob size missmatch!");                                                 \
            for (daxa::u32 i = 0; i < data.size(); ++i)                                                                                        \
                this->value[i] = data[i];                                                                                                      \
            return *this;                                                                                                                      \
        }                                                                                                                                      \
    };                                                                                                                                         \
    struct Task : public daxa::IPartialTask                                                                                                    \
    {                                                                                                                                          \
        using AttachmentViews = daxa::AttachmentViews<ATTACHMENT_COUNT>;                                                                       \
        using Views = VIEWS_T;                                                                                                                 \
        static constexpr ATTACHMENTS_T const & AT = ATTACHMENTS;                                                                               \
        static constexpr daxa::usize ATTACH_COUNT = ATTACHMENT_COUNT;                                                                          \
        static auto name() -> std::string_view { return std::string_view{NAME}; }                                                              \
        static auto attachments() -> std::span<daxa::TaskAttachment const>                                                                     \
        {                                                                                                                                      \
            return AT._internal.value;                                                                                                         \
        }                                                                                                                                      \
    };                                                                                                                                         \
    }                                                                                                                                          \
    ;

#define DAXA_TH_BLOB(HEAD_NAME, field_name) HEAD_NAME::AttachmentShaderBlob field_name;

#define DAXA_TH_IMAGE(TASK_ACCESS, VIEW_TYPE, NAME) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = daxa::ImageViewType::VIEW_TYPE, .shader_array_size = 0)

#define DAXA_TH_IMAGE_ID(TASK_ACCESS, VIEW_TYPE, NAME) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = daxa::ImageViewType::VIEW_TYPE, .shader_array_size = 1)
#define DAXA_TH_IMAGE_ID_ARRAY(TASK_ACCESS, VIEW_TYPE, NAME, SIZE) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = daxa::ImageViewType::VIEW_TYPE, .shader_array_size = SIZE)
#define DAXA_TH_IMAGE_ID_MIP_ARRAY(TASK_ACCESS, VIEW_TYPE, NAME, SIZE) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = daxa::ImageViewType::VIEW_TYPE, .shader_array_size = SIZE, .shader_array_type = daxa::TaskHeadImageArrayType::MIP_LEVELS)

#define DAXA_TH_IMAGE_INDEX(TASK_ACCESS, VIEW_TYPE, NAME) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = daxa::ImageViewType::VIEW_TYPE, .shader_array_size = 1, .shader_as_index = true)
#define DAXA_TH_IMAGE_INDEX_ARRAY(TASK_ACCESS, VIEW_TYPE, NAME, SIZE) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = daxa::ImageViewType::VIEW_TYPE, .shader_array_size = SIZE, .shader_as_index = true)
#define DAXA_TH_IMAGE_INDEX_MIP_ARRAY(TASK_ACCESS, VIEW_TYPE, NAME, SIZE) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = daxa::ImageViewType::VIEW_TYPE, .shader_array_size = SIZE, .shader_array_type = daxa::TaskHeadImageArrayType::MIP_LEVELS, .shader_as_index = true)

#define DAXA_TH_IMAGE_TYPED(TASK_ACCESS, VIEW_TYPE, NAME) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = VIEW_TYPE::IMAGE_VIEW_TYPE, .shader_array_size = 1, .shader_as_index = VIEW_TYPE::SHADER_INDEX32)
#define DAXA_TH_IMAGE_TYPED_ARRAY(TASK_ACCESS, VIEW_TYPE, NAME, SIZE) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = VIEW_TYPE::IMAGE_VIEW_TYPE, .shader_array_size = SIZE, .shader_as_index = VIEW_TYPE::SHADER_INDEX32)
#define DAXA_TH_IMAGE_TYPED_MIP_ARRAY(TASK_ACCESS, VIEW_TYPE, NAME, SIZE) _DAXA_HELPER_TH_IMAGE(NAME, TASK_ACCESS, .view_type = VIEW_TYPE::IMAGE_VIEW_TYPE, .shader_array_size = SIZE, .shader_as_index = VIEW_TYPE::SHADER_INDEX32, .shader_array_type = daxa::TaskHeadImageArrayType::MIP_LEVELS)

#define DAXA_TH_BUFFER(TASK_ACCESS, NAME) _DAXA_HELPER_TH_BUFFER(NAME, TASK_ACCESS, .shader_array_size = 0)
#define DAXA_TH_BUFFER_ID(TASK_ACCESS, NAME) _DAXA_HELPER_TH_BUFFER(NAME, TASK_ACCESS, .shader_array_size = 1, .shader_as_address = false)
#define DAXA_TH_BUFFER_PTR(TASK_ACCESS, PTR_TYPE, NAME) _DAXA_HELPER_TH_BUFFER(NAME, TASK_ACCESS, .shader_array_size = 1, .shader_as_address = true)
#define DAXA_TH_BUFFER_ID_ARRAY(TASK_ACCESS, NAME, SIZE) _DAXA_HELPER_TH_BUFFER(NAME, TASK_ACCESS, .shader_array_size = SIZE, .shader_as_address = false)
#define DAXA_TH_BUFFER_PTR_ARRAY(TASK_ACCESS, PTR_TYPE, NAME, SIZE) _DAXA_HELPER_TH_BUFFER(NAME, TASK_ACCESS, .shader_array_size = SIZE, .shader_as_address = false)
#define DAXA_TH_BLAS(TASK_ACCESS, NAME) _DAXA_HELPER_TH_BLAS(NAME, TASK_ACCESS)
#define DAXA_TH_TLAS(TASK_ACCESS, NAME) _DAXA_HELPER_TH_TLAS(NAME, TASK_ACCESS, .shader_array_size = 0)
#define DAXA_TH_TLAS_PTR(TASK_ACCESS, NAME) _DAXA_HELPER_TH_TLAS(NAME, TASK_ACCESS, .shader_as_address = true)
#define DAXA_TH_TLAS_ID(TASK_ACCESS, NAME) _DAXA_HELPER_TH_TLAS(NAME, TASK_ACCESS, .shader_as_address = false)

    template <typename BufFn, typename ImgFn>
    constexpr void for_each(std::span<TaskAttachmentInfo> attachments, BufFn && buf_fn, ImgFn && img_fn)
    {
        for (u32 index = 0; index < attachments.size(); ++index)
        {
            switch (attachments[index].type)
            {
            case TaskAttachmentType::BUFFER: buf_fn(index, attachments[index].value.buffer); break;
            case TaskAttachmentType::BLAS: buf_fn(index, attachments[index].value.blas); break;
            case TaskAttachmentType::TLAS: buf_fn(index, attachments[index].value.tlas); break;
            case TaskAttachmentType::IMAGE: img_fn(index, attachments[index].value.image); break;
            default: break;
            }
        }
    }

    template <typename BufFn, typename ImgFn>
    constexpr void for_each(std::span<TaskAttachmentInfo const> attachments, BufFn && buf_fn, ImgFn && img_fn)
    {
        for (u32 index = 0; index < attachments.size(); ++index)
        {
            switch (attachments[index].type)
            {
            case TaskAttachmentType::BUFFER: buf_fn(index, attachments[index].value.buffer); break;
            case TaskAttachmentType::BLAS: buf_fn(index, attachments[index].value.blas); break;
            case TaskAttachmentType::TLAS: buf_fn(index, attachments[index].value.tlas); break;
            case TaskAttachmentType::IMAGE: img_fn(index, attachments[index].value.image); break;
            default: break;
            }
        }
    }

    struct TrackedBuffers
    {
        std::span<BufferId const> buffers = {};
        Access latest_access = {};
    };

    struct TaskBufferInfo
    {
        TrackedBuffers initial_buffers = {};
        std::string name = {};
    };

    struct ImplPersistentTaskBufferBlasTlas;
    struct DAXA_EXPORT_CXX TaskBuffer : ManagedPtr<TaskBuffer, ImplPersistentTaskBufferBlasTlas *>
    {
        TaskBuffer() = default;
        TaskBuffer(TaskBufferInfo const & info);
        TaskBuffer(daxa::Device & device, BufferInfo const & info);

        operator TaskBufferView() const;

        auto view() const -> TaskBufferView;
        /// THREADSAFETY:
        /// * reference MUST NOT be read after the object is destroyed.
        /// @return reference to info of object.
        auto info() const -> TaskBufferInfo const &;
        auto get_state() const -> TrackedBuffers;
        auto is_owning() const -> bool;

        void set_buffers(TrackedBuffers const & buffers);
        void swap_buffers(TaskBuffer & other);

      protected:
        template <typename T, typename H_T>
        friend struct ManagedPtr;
        static auto inc_refcnt(ImplHandle const * object) -> u64;
        static auto dec_refcnt(ImplHandle const * object) -> u64;
    };

    struct TrackedBlas
    {
        std::span<BlasId const> blas = {};
        Access latest_access = {};
    };

    struct TaskBlasInfo
    {
        TrackedBlas initial_blas = {};
        std::string name = {};
    };

    struct DAXA_EXPORT_CXX TaskBlas : ManagedPtr<TaskBlas, ImplPersistentTaskBufferBlasTlas *>
    {
        TaskBlas() = default;
        TaskBlas(TaskBlasInfo const & info);

        operator TaskBlasView() const;

        auto view() const -> TaskBlasView;
        /// THREADSAFETY:
        /// * reference MUST NOT be read after the object is destroyed.
        /// @return reference to info of object.
        auto info() const -> TaskBlasInfo const &;
        auto get_state() const -> TrackedBlas;

        void set_blas(TrackedBlas const & blas);
        void swap_blas(TaskBlas & other);

      protected:
        template <typename T, typename H_T>
        friend struct ManagedPtr;
        static auto inc_refcnt(ImplHandle const * object) -> u64;
        static auto dec_refcnt(ImplHandle const * object) -> u64;
    };

    struct TrackedTlas
    {
        std::span<TlasId const> tlas = {};
        Access latest_access = {};
    };

    struct TaskTlasInfo
    {
        TrackedTlas initial_tlas = {};
        std::string name = {};
    };

    struct DAXA_EXPORT_CXX TaskTlas : ManagedPtr<TaskTlas, ImplPersistentTaskBufferBlasTlas *>
    {
        TaskTlas() = default;
        TaskTlas(TaskTlasInfo const & info);

        operator TaskTlasView() const;

        auto view() const -> TaskTlasView;
        /// THREADSAFETY:
        /// * reference MUST NOT be read after the object is destroyed.
        /// @return reference to info of object.
        auto info() const -> TaskTlasInfo const &;
        auto get_state() const -> TrackedTlas;

        void set_tlas(TrackedTlas const & tlas);
        void swap_tlas(TaskTlas & other);

      protected:
        template <typename T, typename H_T>
        friend struct ManagedPtr;
        static auto inc_refcnt(ImplHandle const * object) -> u64;
        static auto dec_refcnt(ImplHandle const * object) -> u64;
    };

    struct TrackedImages
    {
        std::span<ImageId const> images = {};
        // optional:
        std::span<ImageSliceState const> latest_slice_states = {};
    };

    struct TaskImageInfo
    {
        TrackedImages initial_images = {};
        bool swapchain_image = {};
        std::string name = {};
    };

    struct ImplPersistentTaskImage;
    struct DAXA_EXPORT_CXX TaskImage : ManagedPtr<TaskImage, ImplPersistentTaskImage *>
    {
        TaskImage() = default;
        // TaskImage(TaskImage const & ti) = default;
        TaskImage(TaskImageInfo const & info);

        operator TaskImageView() const;

        auto view() const -> TaskImageView;
        /// THREADSAFETY:
        /// * reference MUST NOT be read after the object is destroyed.
        /// @return reference to info of object.
        auto info() const -> TaskImageInfo const &;
        auto get_state() const -> TrackedImages;

        void set_images(TrackedImages const & images);
        void swap_images(TaskImage & other);

      protected:
        template <typename T, typename H_T>
        friend struct ManagedPtr;
        static auto inc_refcnt(ImplHandle const * object) -> u64;
        static auto dec_refcnt(ImplHandle const * object) -> u64;
    };

    inline auto attachment_view(TaskBufferAttachmentIndex index, TaskBufferView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskBufferAttachmentIndex, daxa::TaskBufferView>(index, view);
    }

    inline auto attachment_view(TaskBlasAttachmentIndex index, TaskBlasView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskBlasAttachmentIndex, daxa::TaskBlasView>(index, view);
    }

    inline auto attachment_view(TaskTlasAttachmentIndex index, TaskTlasView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskTlasAttachmentIndex, daxa::TaskTlasView>(index, view);
    }

    inline auto attachment_view(TaskImageAttachmentIndex index, TaskImageView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskImageAttachmentIndex, daxa::TaskImageView>(index, view);
    }

    inline auto operator|(TaskBufferAttachmentIndex index, TaskBufferView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskBufferAttachmentIndex, daxa::TaskBufferView>(index, view);
    }

    inline auto operator|(TaskBlasAttachmentIndex index, TaskBlasView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskBlasAttachmentIndex, daxa::TaskBlasView>(index, view);
    }

    inline auto operator|(TaskTlasAttachmentIndex index, TaskTlasView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskTlasAttachmentIndex, daxa::TaskTlasView>(index, view);
    }

    inline auto operator|(TaskImageAttachmentIndex index, TaskImageView view) -> TaskViewVariant
    {
        return std::pair<daxa::TaskImageAttachmentIndex, daxa::TaskImageView>(index, view);
    }

    inline auto inl_attachment(TaskBufferAccess access, TaskBufferView view) -> TaskAttachmentInfo
    {
        TaskBufferAttachmentInfo buf = {};
        buf.name = "inline attachment";
        buf.task_access = access;
        buf.shader_array_size = 0;
        buf.shader_as_address = false;
        buf.view = view;
        TaskAttachmentInfo info = {};
        info.type = daxa::TaskAttachmentType::BUFFER;
        info.value.buffer = buf;
        return info;
    }

    inline auto inl_attachment(TaskBlasAccess access, TaskBlasView view) -> TaskAttachmentInfo
    {
        TaskBlasAttachmentInfo blas = {};
        blas.name = "inline attachment";
        blas.task_access = access;
        blas.view = view;
        TaskAttachmentInfo info = {};
        info.type = daxa::TaskAttachmentType::BLAS;
        info.value.blas = blas;
        return info;
    }

    inline auto inl_attachment(TaskTlasAccess access, TaskTlasView view) -> TaskAttachmentInfo
    {
        TaskTlasAttachmentInfo tlas = {};
        tlas.name = "inline attachment";
        tlas.task_access = access;
        tlas.view = view;
        tlas.shader_as_address = false;
        TaskAttachmentInfo info = {};
        info.type = daxa::TaskAttachmentType::TLAS;
        info.value.tlas = tlas;
        return info;
    }

    inline auto inl_attachment(TaskImageAccess access, TaskImageView view) -> TaskAttachmentInfo
    {
        TaskImageAttachmentInfo img = {};
        img.name = "inline attachment";
        img.task_access = access;
        img.view_type = daxa::ImageViewType::MAX_ENUM;
        img.shader_array_size = 0;
        img.view = view;
        TaskAttachmentInfo info = {};
        info.value.image = img;
        info.type = daxa::TaskAttachmentType::IMAGE;
        return info;
    }

    inline auto inl_attachment(TaskImageAccess access, ImageViewType view_type, TaskImageView view) -> TaskAttachmentInfo
    {
        TaskImageAttachmentInfo img = {};
        img.name = "inline attachment";
        img.task_access = access;
        img.view_type = view_type;
        img.shader_array_size = 0;
        img.view = view;
        TaskAttachmentInfo info = {};
        info.type = daxa::TaskAttachmentType::IMAGE;
        info.value.image = img;
        return info;
    }
} // namespace daxa
