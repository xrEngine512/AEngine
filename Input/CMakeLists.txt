file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

include_directories(include ../CommonSystem ../PlatformAbstraction/include)

add_library(ApproxInput SHARED ${SOURCE_FILES} ${HEADER_FILES})

set(COMMON_LINK_LIBRARIES
        ApproxPlatformAbstraction
        ApproxCommonSystem)

if(WIN32)
    target_link_libraries(ApproxInput ${COMMON_LINK_LIBRARIES})
elseif(UNIX)
    target_link_libraries(ApproxInput ${COMMON_LINK_LIBRARIES})
endif()
