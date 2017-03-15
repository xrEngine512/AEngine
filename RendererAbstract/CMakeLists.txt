AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

file(GLOB_RECURSE HEADER_FILES *.h)

include_directories(include
        ../CommonSystem
        ../Sound/include
        ../Input/include
        ../PlatformAbstraction/include
        ../GlobalServices/include
        ../ShaderLab)

add_library(ApproxRendererAbstract STATIC ${SOURCE_FILES} ${HEADER_FILES})

set(COMMON_LINK_LIBRARIES ApproxCommonSystem)

target_link_libraries(ApproxRendererAbstract ${COMMON_LINK_LIBRARIES})