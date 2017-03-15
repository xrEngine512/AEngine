//
// Created by islam on 22.02.17.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utility>


#define ALIAS(x,y) template <typename... Args>\
inline auto x(Args&&... args) -> decltype(y(std::forward<Args>(args)...)) {\
    return y(std::forward<Args>(args)...);\
}

#define ALIAS_DEFAULT(x,y,z) template <typename... Args>\
inline auto x(Args&&... args) -> decltype(y(z, std::forward<Args>(args)...)) {\
    return y(z, std::forward<Args>(args)...);\
}

namespace Math {
    const auto pi = glm::pi<float>();

    typedef glm::mat4x4 AMatrix;
    typedef glm::mat3x3 AMatrix3;
    typedef glm::vec4 AVector;
    typedef glm::vec3 AVector3;
    typedef glm::vec2 AVector2;
    typedef glm::vec3 APoint;
    typedef glm::quat AQuaternion;

    ALIAS(ANormalize, glm::normalize)
    ALIAS(ADotProduct, glm::dot)
    ALIAS(ACrossProduct, glm::cross)
    ALIAS(AFromEuler, glm::eulerAngleXYZ)
    ALIAS(ALookAt, glm::lookAt)
    ALIAS_DEFAULT(ATranslationMatrix, glm::translate, glm::mat4());
    ALIAS(AToMatrix, glm::mat4_cast)
    ALIAS(ATranspose, glm::transpose)
    ALIAS(APerspective, glm::perspective)
    ALIAS(AOrthogonal, glm::ortho)
}
