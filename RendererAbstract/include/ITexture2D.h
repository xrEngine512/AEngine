#pragma once
//
// Created by islam on 23.02.17.
//

#include <string>
#include <memory>

/**
 *@brief: interface to 2D texture, should be implemented at the end renderer
 */
interface ITexture2D {
    virtual ~ITexture2D(void) {};
};

typedef std::shared_ptr<ITexture2D> ITexture2DPtr;
typedef std::weak_ptr<ITexture2D> ITexture2DRef;
