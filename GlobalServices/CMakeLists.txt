file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

include_directories(
        include
        ../CommonSystem
        ../Terminal/include
)

add_library(ApproxGlobalServices ${SOURCE_FILES} ${HEADER_FILES})

set(COMMON_LINK_LIBRARIES
        ApproxTerminal)

if(WIN32)
    target_link_libraries(ApproxGlobalServices ${COMMON_LINK_LIBRARIES})
elseif(UNIX)
    target_link_libraries(ApproxGlobalServices ${COMMON_LINK_LIBRARIES})
endif()
