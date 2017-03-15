file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

find_package(Qt5Widgets REQUIRED)

include_directories(../PlatformAbstraction/include)

add_library(ApproxGUILibrary SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ApproxGUILibrary Qt5::Widgets)
