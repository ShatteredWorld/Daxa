#pragma once
#include "../daxa.inl"

#if DAXA_SHADER
#define DAXA_DECL_TASK_USES_BEGIN(NAME, SLOT) \
    DAXA_DECL_UNIFORM_BUFFER(SLOT)            \
    NAME                                      \
    {
#define DAXA_TASK_USE_IMAGE(NAME, VIEW_TYPE, TASK_ACCESS) daxa_ImageViewId NAME;
#define DAXA_TASK_USE_BUFFER(NAME, TYPE, TASK_ACCESS) TYPE NAME;
#define DAXA_DECL_TASK_USES_END() \
    }                             \
    ;
#else
#include "task_graph_types.hpp"
#define DAXA_DECL_TASK_USES_BEGIN(TASK_NAME, SLOT)           \
    struct TASK_NAME                                         \
    {                                                        \
        static constexpr isize CONSTANT_BUFFER_SLOT = SLOT;   \
        static constexpr std::string_view NAME = #TASK_NAME; \
        std::string name = #TASK_NAME;                       \
        struct Uses                                          \
        {
#define DAXA_TASK_USE_BUFFER(NAME, TYPE, TASK_ACCESS) daxa::TaskBufferUse<daxa::TaskBufferAccess::TASK_ACCESS> NAME{};
#define DAXA_TASK_USE_IMAGE(NAME, VIEW_TYPE, TASK_ACCESS) daxa::TaskImageUse<daxa::TaskImageAccess::TASK_ACCESS, daxa::ImageViewType::VIEW_TYPE> NAME{};
#define DAXA_DECL_TASK_USES_END() \
    }                             \
    uses = {};                    \
    }                             \
    ;

#endif

#if __cplusplus
#define DAXA_USE_TASK_HEADER(x)                                          \
    static constexpr isize CONSTANT_BUFFER_SLOT = x::CONSTANT_BUFFER_SLOT; \
    static constexpr std::string_view NAME = x::NAME;                    \
    std::string name = std::string(x::NAME);                             \
    x::Uses uses = {};
#endif

// DAXA 3.0 Task-Head-Shader interface:
// #define DAXA_TASK_HEAD_BEGIN(...)
// #define DAXA_TH_IMAGE(...)
// #define DAXA_TH_IMAGE_ARRAY(..., SIZE)
// #define DAXA_TH_BUFFER_ID(...)
// #define DAXA_TH_BUFFER_ADDR(...)
// #define DAXA_TH_BUFFER_ID_ARRAY(NAME, TASK_ACCESS, SIZE)
// #define DAXA_TH_BUFFER_ADDR_ARRAY(NAME, TASK_ACCESS, SIZE)
// #define DAXA_TASK_HEAD_END