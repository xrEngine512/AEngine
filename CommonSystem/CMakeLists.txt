file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

find_package(Qt5Widgets REQUIRED)

include_directories(../GlobalServices/include)

add_library(ApproxCommonSystem SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ApproxCommonSystem Qt5::Widgets)
