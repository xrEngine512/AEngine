//
// Created by islam on 10.04.17.
//

#pragma once

#include <msgpack.hpp>

#include <glslang/MachineIndependent/reflection.h>


class ObjectReflection: public glslang::TObjectReflection {
public:
    MSGPACK_DEFINE(name, offset, glDefineType, size, index);
};

class ReflectionAccessor: glslang::TReflection {
public:
    auto get_uniforms() const {
        auto ptr = (const ObjectReflection*)indexToUniform.data();
        return vector<ObjectReflection>(ptr, ptr + indexToUniform.size());
    }
    auto get_uniform_blocks() const {
        auto ptr = (const ObjectReflection*)indexToUniformBlock.data();
        return vector<ObjectReflection>(ptr, ptr + indexToUniformBlock.size());
    }
    auto get_attributes() const {
        auto ptr = (const ObjectReflection*)indexToAttribute.data();
        return vector<ObjectReflection>(ptr, ptr + indexToAttribute.size());
    }
};

class Reflection {
    vector<ObjectReflection> uniforms, uniformBlocks, attributes;
public:
    Reflection(const glslang::TReflection* reflection) {
        auto accessed_reflection = (const ReflectionAccessor*)reflection;
        uniforms        = accessed_reflection->get_uniforms();
        uniformBlocks   = accessed_reflection->get_uniform_blocks();
        attributes      = accessed_reflection->get_attributes();
    }
    MSGPACK_DEFINE(uniforms, uniformBlocks, attributes);
};

// Huge thanks to glslang developers for using their own TString instead of standard std::string
namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(v1) {
        namespace adaptor {
            template <>
            struct convert<glslang::TString>: convert<std::string> {
                msgpack::object const& operator()(msgpack::object const& o, glslang::TString& v) const {
                    auto std_string = std::string(v.c_str());
                    return convert<std::string>::operator()(o, std_string);
                }
            };

            template <>
            struct pack<glslang::TString>: pack<std::string> {
                template <typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glslang::TString& v) const {
                    auto std_string = std::string(v.c_str());
                    return pack<std::string>::operator()(o, std_string);
                }
            };

            template <>
            struct object<glslang::TString>: object<std::string> {
                void operator()(msgpack::object& o, const glslang::TString& v) const {
                    auto std_string = std::string(v.c_str());
                    object<std::string>::operator()(o, std_string);
                }
            };

            template <>
            struct object_with_zone<glslang::TString>: object_with_zone<std::string> {
                void operator()(msgpack::object::with_zone& o, const glslang::TString& v) const {
                    auto std_string = std::string(v.c_str());
                    object_with_zone<std::string>::operator()(o, std_string);
                }
            };

        } // namespace adaptor
    } // MSGPACK_API_VERSION_NAMESPACE(v1)
} // namespace msgpack
