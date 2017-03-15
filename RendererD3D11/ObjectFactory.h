#pragma once
//
// Created by islam on 25.02.17.
//

#include <IObjectFactory.h>



class ObjectFactory: IObjectFactory {
public:
    IGeometryBufferPtr createGeometryBuffer(std::vector<DefaultVertex>&& vertices, std::vector<index_t>&& indices) override;
};



