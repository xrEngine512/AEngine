#pragma once
#define DELETE_SYS_OBJECT(Object) \
if (Object)\
{\
    Object->shutdown();\
    delete Object;\
    Object = nullptr;\
}
#define DELETE_D3D11_OBJECT(Object)\
if(Object)\
{\
    Object->Release();\
    Object = nullptr;\
}
#define RESOURCES_DIR L"../../Core/Engine/Resources/"
#define TEXTURES_DIR L"../../Core/Resources/Textures/"