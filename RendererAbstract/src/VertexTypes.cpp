#include "VertexTypes.h"

PosColorVert::PosColorVert(PosTexVert& parent):position(parent.position){}
PosColorVert::PosColorVert(PosTexNormVert& parent): position(parent.position){}

PosTexVert::PosTexVert(PosColorVert& parent): position(parent.position){}
PosTexVert::PosTexVert(PosTexNormVert& parent): position(parent.position), texture(parent.texture){}

PosTexNormVert::PosTexNormVert(PosColorVert& parent) : position(parent.position){}
PosTexNormVert::PosTexNormVert(PosTexVert& parent) : position(parent.position), texture(parent.texture){}

DefaultVertex::DefaultVertex(const DefaultVertex &vertex):
    position(vertex.position),
    texture(vertex.texture),
    normal(vertex.normal)
{}

bool DefaultVertex::operator==(const DefaultVertex &lhr) {
    return this->position == lhr.position;
}

bool DefaultVertex::operator!=(const DefaultVertex &lhr) {
    return this->position != lhr.position;
}

int DefaultVertex::GetSize() {
    return sizeof(PosTexNormVert);
}

VertexTypes DefaultVertex::GetType() {
    return VT_DEFAULT;
}

DefaultVertex::DefaultVertex(float x, float y, float z, float w): position(x, y, z, w) {

}

DefaultVertex::DefaultVertex(const Math::AVector &position, const Math::AVector2 &texture):
    position(position), texture(texture) {

}
