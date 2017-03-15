file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

set(PLATFORM_DEPENDENT_HEADER_DIR)

if(WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
    set(PLATFORM_DEPENDENT_HEADER_DIR Windows)
elseif(UNIX)
    add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
    set(PLATFORM_DEPENDENT_HEADER_DIR Linux)
endif()

include_directories(
        ../CommonSystem
        ../RendererAbstract/include
        ../Sound/include
        ../GlobalServices/include
        ../Terminal/include
        ${PLATFORM_DEPENDENT_HEADER_DIR}
)

add_library(ApproxRendererVulkan SHARED ${SOURCE_FILES} ${HEADER_FILES})

set(COMMON_LINK_LIBRARIES
        ApproxCommonSystem
        ApproxRendererAbstract
        ApproxGlobalServices
        ApproxTerminal
        vulkan
)

if(WIN32)
    target_link_libraries(ApproxRendererVulkan ${COMMON_LINK_LIBRARIES})
elseif(UNIX)
    target_link_libraries(ApproxRendererVulkan ${COMMON_LINK_LIBRARIES})
endif()

