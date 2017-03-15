//
// Created by islam on 23.02.17.
//

#pragma once

#import <memory>

interface IMesh {
    virtual void Render() = 0;
};

typedef std::shared_ptr<IMesh> IMeshPtr;
typedef std::weak_ptr<IMesh> IMeshRef;
