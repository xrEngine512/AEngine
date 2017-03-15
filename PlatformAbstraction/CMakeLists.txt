file(GLOB_RECURSE HEADER_FILES *.h)

if(WIN32)
    AUX_SOURCE_DIRECTORY(Windows SOURCE_FILES)
elseif(UNIX)
    AUX_SOURCE_DIRECTORY(Linux SOURCE_FILES)
endif()

include_directories(include ../CommonSystem)

add_library(ApproxPlatformAbstraction SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ApproxPlatformAbstraction ApproxCommonSystem)
