#include <DirectXMath.h>
using namespace DirectX;
#include "VertexTypes.h"

PosColorVert::PosColorVert(PosTexVert& parent):position(parent.position){}
PosColorVert::PosColorVert(PosTexNormVert& parent): position(parent.position){}

PosTexVert::PosTexVert(PosColorVert& parent): position(parent.position){}
PosTexVert::PosTexVert(PosTexNormVert& parent): position(parent.position), texture(parent.texture){}

PosTexNormVert::PosTexNormVert(PosColorVert& parent) : position(parent.position){}
PosTexNormVert::PosTexNormVert(PosTexVert& parent) : position(parent.position), texture(parent.texture){}