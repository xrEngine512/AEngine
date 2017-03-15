//
// Created by islam on 26.02.17.
//

#include "RenderQueue.h"

#include <algorithm>

using namespace std;

void RenderQueue::add_object(IInternalRenderObject *object) {
    by_object.push_back(object);
}

void RenderQueue::remove_object(const IInternalRenderObject *object) {
    auto buffer_it = find(by_object.begin(), by_object.end(), object);
    if(buffer_it != by_object.end()){
        by_object.erase(buffer_it);
    }
}

void RenderQueue::render(MatInterface::MaterialInterface& mi) {
    for(auto object : by_object) {
        object->render(mi);
    }
}
