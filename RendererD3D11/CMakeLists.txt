file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

include_directories(include
        ../CommonSystem
        ../RendererAbstract/include
        ../PlatformAbstraction/include
        ../GlobalServices/include)

add_library(ApproxRendererD3D11 SHARED ${SOURCE_FILES} ${HEADER_FILES})

set(COMMON_LINK_LIBRARIES ApproxCommonSystem ApproxRendererAbstract)

target_link_libraries(ApproxRendererD3D11 ${COMMON_LINK_LIBRARIES})