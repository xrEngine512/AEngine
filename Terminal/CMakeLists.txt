file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

find_package(Qt5Widgets REQUIRED)

include_directories(
        include
        ../CommonSystem
        ../external/rlutil
)

file(GLOB_RECURSE EXTERNAL_HEADER_FILES ../external/rlutil/*.h)

add_library(ApproxTerminal SHARED ${SOURCE_FILES} ${HEADER_FILES} ${EXTERNAL_HEADER_FILES})

set(COMMON_LINK_LIBRARIES
        Qt5::Widgets
        )

if(WIN32)
    target_link_libraries(ApproxTerminal ${COMMON_LINK_LIBRARIES})
elseif(UNIX)
    target_link_libraries(ApproxTerminal ${COMMON_LINK_LIBRARIES})
endif()

