file(GLOB_RECURSE HEADER_FILES *.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

find_package(Qt5Widgets REQUIRED)

qt5_add_resources(EDITOR_RCC_SRC Resources/editor.qrc editor.qrc)

include_directories(
        ../GUI_Library
        ../ShaderLab
        ../RendererAbstract/include
        ../System
        ../CommonSystem
        ../Sound/include
        ../Input
        ../GlobalServices/include
)
add_executable(ApproxEditor ${SOURCE_FILES} ${HEADER_FILES} ${EDITOR_RCC_SRC})

file(COPY Resources/editor.qss DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Resources)

set(COMMON_LINK_LIBRARIES
        Qt5::Widgets
        boost_locale
        ApproxShaderLab
        ApproxGUILibrary
        ApproxSystem
        ApproxCommonSystem
        ApproxRendererVulkan
        ApproxInput
        ApproxSound)

if(WIN32)
    target_link_libraries(ApproxEditor ${COMMON_LINK_LIBRARIES})
elseif(UNIX)
    target_link_libraries(ApproxEditor ${COMMON_LINK_LIBRARIES}
            -lpthread)
endif()