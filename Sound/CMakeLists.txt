file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

include_directories(../CommonSystem include)

if(WIN32)
    include_directories(../DirectXTools/include)
elseif(UNIX)
    set(EXCLUDE_FILES ./DirectSoundSystem.cpp)
endif()

list(REMOVE_ITEM SOURCE_FILES ${EXCLUDE_FILES})

add_library(ApproxSound SHARED ${SOURCE_FILES} ${HEADER_FILES})
