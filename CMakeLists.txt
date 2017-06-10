cmake_minimum_required(VERSION 3.5)

# Cross-platform 3D visualization engine with D3D11 and Vulkan support
# (D3D11 isn't really needed, but it was in the first place, so let it be)
# Some about interfaces:
# IExternal... interfaces meant to be used ouside of project (open model, change texture, that kind of stuff)
# IInternal... interfaces are for (unexpected!) internal usage inside of project, they handle system stuff as initialization, cleanup,
# note that some of them may not be meant to be included outside of their module (for instance, IInternalMaterial),
# while others may assume that (for instance IInternalRenderer) (maybe i should separate them)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(ApproxEngine)

add_definitions(-Dinterface=struct)

if(WIN32)
    add_definitions(-DAPPROX_API=extern "C" __declspec(dllexport))
    add_definitions(-DWindowHandle=HWND)
    add_definitions(-Dsimdcall=__vectorcall)
    add_subdirectory(RendererD3D11)
elseif(UNIX)
    add_definitions(-DAPPROX_API=)
    add_definitions(-DWindowHandle=unsigned\ long)
    add_definitions(-Dsimdcall=)
    set(CMAKE_CXX_FLAGS -fPIC)
endif()

add_subdirectory(CommonSystem)
add_subdirectory(PlatformAbstraction)
add_subdirectory(GlobalServices)
add_subdirectory(Input)
add_subdirectory(Sound)
add_subdirectory(RendererAbstract)
add_subdirectory(RendererVulkan)
add_subdirectory(GUI_Library)
add_subdirectory(System)
add_subdirectory(Terminal)
add_subdirectory(external_local/glslang)
add_subdirectory(ShaderLab)
add_subdirectory(Editor)
