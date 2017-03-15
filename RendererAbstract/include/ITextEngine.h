#pragma once
//
// Created by islam on 23.02.17.
//

#include <ApproxMath.h>

#include <experimental/string_view>
#include <memory>


/**
 * @brief: interface to access text engine that responsible for text rendering
 *              should be implemented in the end renderer
 */
class ITextEngine {
public:
    virtual void initialize() = 0;
    virtual void set_font(const std::experimental::string_view&) = 0;
    virtual void draw_string(const std::experimental::string_view&, const Math::AVector2& pos) = 0;
    virtual ~ITextEngine(){};
};

typedef std::shared_ptr<ITextEngine> ITextEnginePtr;
typedef std::weak_ptr<ITextEngine> ITextEngineRef;
