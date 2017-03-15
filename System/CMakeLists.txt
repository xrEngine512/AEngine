file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

include_directories(
        ../CommonSystem
        ../RendererAbstract/include
        ../Input/include
        ../Sound/include
        ../PlatformAbstraction/include
        ../GlobalServices/include
)

set(EXCLUDE_FILES ./SynchronizedSlaveThread.cpp
                  ./SynchronizedMasterThread.cpp
                  ./SynchronizedThread.cpp
                  ./SyncEvent.cpp
                  ./main.cpp)

list(REMOVE_ITEM SOURCE_FILES ${EXCLUDE_FILES})

add_library(ApproxSystem SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ApproxSystem
        ApproxCommonSystem
        ApproxPlatformAbstraction
        ApproxRendererVulkan
        )
