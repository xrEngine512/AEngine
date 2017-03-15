//
// Created by islam on 12.03.17.
//

#pragma once

#include "ErrorHandling.h"

#include <vulkan/vulkan.h>

#undef Bool
#undef CursorShape
#undef Expose
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef None
#undef Status
#undef Unsorted

#define NATIVE_SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME

Display* get_display();

VkSurfaceKHR getSurface(VkInstance instance, WindowHandle window) {
    VkXlibSurfaceCreateInfoKHR display_surface_info {
        .sType          = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .window         = window,
        .dpy            = get_display()
    };

    VkSurfaceKHR surface;
    HandleErrors(vkCreateXlibSurfaceKHR(instance, &display_surface_info, nullptr, &surface))
    return surface;
}
