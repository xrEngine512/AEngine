//
// Created by islam on 11.03.17.
//

#include "VulkanAPI.h"
#include "ErrorHandling.h"

#include <Logger.h>
#include <Terminal.h>
#include <PlatformAbstraction.h>
#include <Utility.h>

using namespace std;
using namespace Math;

const VkAllocationCallbacks* allocation_callback = nullptr;

PFN_vkCreateDebugReportCallbackEXT      pfnVkCreateDebugReportCallbackEXT       = nullptr;
PFN_vkDestroyDebugReportCallbackEXT     pfnVkDestroyDebugReportCallbackEXT      = nullptr;

inline string compose_message(const char* layer, const char* message) {
    return string("Vulkan::") + layer + "::" + message;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT type,               //  type of object that caused call of callback
    uint64_t object,             //  pointer to object that caused call of callback
    size_t location,
    int32_t message_code,
    const char *layer_prefix,
    const char *message,
    void *user_data
) {
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        Logger::instance().debug(compose_message(layer_prefix, message));
    else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        Logger::instance().warning(compose_message(layer_prefix, message));
    else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        Logger::instance().error(compose_message(layer_prefix, message));
    else
        Logger::instance().log(compose_message(layer_prefix, message));

    return (VkBool32)false;       // not stopping propagation
}


VulkanAPI::VulkanAPI() {

}

bool VulkanAPI::begin_scene(const AVector &color) {

    HandleErrors(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, nullptr, swapchain_image_fence, &active_swapchain_image_id))
    HandleErrors(vkWaitForFences(device, 1, &swapchain_image_fence, VK_TRUE, UINT64_MAX))
    HandleErrors(vkResetFences(device, 1, &swapchain_image_fence))
    HandleErrors(vkQueueWaitIdle(queue))

    VkCommandBufferBeginInfo command_buffer_begin_info {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags              = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    /* begin recording to command buffer */
    HandleErrors(vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info))

    /* fill command buffer here */
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    const uint32_t clear_value_count = 2;
    VkClearValue clear_values[clear_value_count] {
        {
            .depthStencil = {
                .depth      = 0.0f,
                .stencil    = 0
            },
        },
        {
            .color = convert(color)
        }
    };

    VkRenderPassBeginInfo render_pass_begin_info {
        .sType              = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass         = render_pass,
        .framebuffer        = framebuffers[active_swapchain_image_id],
        .renderArea         = {
            .extent = {
                .width  = surface_width,
                .height = surface_height
            },
            .offset = {}
        },
        .clearValueCount    = clear_value_count,
        .pClearValues       = clear_values
    };

    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    return true;
}

bool VulkanAPI::end_scene() {

    vkCmdEndRenderPass(command_buffer);

    /* end of recording, state of command buffer now if executable */
    HandleErrors(vkEndCommandBuffer(command_buffer))

    VkSubmitInfo submit_info {
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount     = 1,
        .pCommandBuffers        = &command_buffer
    };

    /* submit compiled command buffer to GPU */
    HandleErrors(vkQueueSubmit(queue, 1, &submit_info, queue_submit_fence))

    /* wait for GPU to process command buffer submission */
    HandleErrors(vkWaitForFences(device, 1, &queue_submit_fence, VK_TRUE, UINT64_MAX))
    HandleErrors(vkResetFences(device, 1, &queue_submit_fence))

    VkResult present_result = VK_RESULT_MAX_ENUM;

    VkPresentInfoKHR present_info {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores    = nullptr,
        .swapchainCount     = 1,
        .pSwapchains        = &swapchain,
        .pImageIndices      = &active_swapchain_image_id,
        .pResults           = &present_result
    };

    HandleErrors(vkQueuePresentKHR(queue, &present_info))
    HandleErrors(present_result)
    return true;
}

bool VulkanAPI::set_zbuffer_enabled(bool enabled) {
    return false;
}

bool VulkanAPI::set_alpha_blending_enabled(bool enabled) {
    return false;
}

std::vector<VkQueueFamilyProperties> VulkanAPI::enumerate_gpu_queue_families(VkPhysicalDevice gpu) {
    uint32_t family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);

    vector<VkQueueFamilyProperties> families(family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, families.data());

    return families;
}

uint32_t VulkanAPI::find_queue_family(const std::vector<VkQueueFamilyProperties> &families, VkQueueFlags flags) {
    uint32_t family_index = 0, best_family_index = 0;
    bool found = false;
    for (auto family: families) {
        if (family.queueFlags & flags)
            found = true;
        if (family.queueFlags == flags)
            return best_family_index;

        if (!found)
            ++family_index;
        ++best_family_index;
    }
    if (found)
        return family_index;

    throw approx_exception("Couldn't find graphics family queue in given list", "VulkanAPI");
}

VkClearColorValue VulkanAPI::convert(const Math::AVector &color) {
    // Apparently, component order is always RGBA, no matter if format is, for instance, R8G8B8A8_UNORM or B8G8R8A8_UNORM. Unexpected.
    switch (surface_format.format) {
        case VK_FORMAT_R8G8B8A8_UNORM:
            return VkClearColorValue {
                .float32 = {
                    color.r, color.g, color.b, color.a
                }
            };
        case VK_FORMAT_R8G8B8A8_SNORM:
            return VkClearColorValue {
                .float32 = {
                    color.r, color.g, color.b, color.a
                }
            };
        case VK_FORMAT_R8G8B8A8_USCALED:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_R8G8B8A8_SSCALED:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_R8G8B8A8_UINT:
            return VkClearColorValue {
                .uint32 = {
                    static_cast<uint32_t>(255 * color.r),
                    static_cast<uint32_t>(255 * color.g),
                    static_cast<uint32_t>(255 * color.b),
                    static_cast<uint32_t>(255 * color.a)
                }
            };
        case VK_FORMAT_R8G8B8A8_SINT:
            return VkClearColorValue {
                .int32 = {
                    static_cast<int32_t>(255 * color.r),
                    static_cast<int32_t>(255 * color.g),
                    static_cast<int32_t>(255 * color.b),
                    static_cast<int32_t>(255 * color.a)
                }
            };
        case VK_FORMAT_R8G8B8A8_SRGB:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_B8G8R8A8_UNORM:
            return VkClearColorValue {
                .float32 = {
                    color.r, color.g, color.b, color.a
                }
            };
        case VK_FORMAT_B8G8R8A8_SNORM:
            return VkClearColorValue {
                .float32 = {
                    color.r, color.g, color.b, color.a
                }
            };
        case VK_FORMAT_B8G8R8A8_USCALED:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_B8G8R8A8_SSCALED:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_B8G8R8A8_UINT:
            return VkClearColorValue {
                .uint32 = {
                    static_cast<uint32_t>(255 * color.r),
                    static_cast<uint32_t>(255 * color.g),
                    static_cast<uint32_t>(255 * color.b),
                    static_cast<uint32_t>(255 * color.a)
                }
            };
        case VK_FORMAT_B8G8R8A8_SINT:
            return VkClearColorValue {
                .int32 = {
                    static_cast<int32_t>(255 * color.r),
                    static_cast<int32_t>(255 * color.g),
                    static_cast<int32_t>(255 * color.b),
                    static_cast<int32_t>(255 * color.a)
                }
            };
        case VK_FORMAT_B8G8R8A8_SRGB:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return VkClearColorValue {
                .float32 = {}
            };
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return VkClearColorValue {
                .float32 = {}
            };
        default:
            return VkClearColorValue {};
    }
}

uint32_t VulkanAPI::find_memory_type_index(const VkMemoryRequirements& memory_requirements,
                                           const VkMemoryPropertyFlags required_properties) const {
    for (uint32_t i = 0; i < gpu_memory_properties.memoryTypeCount; ++i) {
        if (memory_requirements.memoryTypeBits & (1 << i)) {
            if (gpu_memory_properties.memoryTypes[i].propertyFlags & required_properties) {
                return i;
            }
        }
    }
    throw approx_exception("Couldn't find suitable memory type", "VulkanAPI");
}

void VulkanAPI::initialize_instance(const char *application_name) {
    VkApplicationInfo application_info {
        .sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .apiVersion             = VK_MAKE_VERSION(1, 0, 2),
        .applicationVersion     = VK_MAKE_VERSION(0, 0, 1),
        .engineVersion          = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName            = "ApproxEngine",
        .pApplicationName       = application_name
    };

    VkInstanceCreateInfo instance_info {
        .sType                  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo       = &application_info,
        .enabledLayerCount      = (uint32_t)instance_layers.size(),
        .ppEnabledLayerNames    = instance_layers.data(),
        .enabledExtensionCount  = (uint32_t)instance_extensions.size(),
        .ppEnabledExtensionNames= instance_extensions.data(),
        .pNext                  = &debug_report_callback_info
    };

    HandleErrors(vkCreateInstance(&instance_info, allocation_callback, &instance))
}

void VulkanAPI::destroy_instance() {
    if (instance)
        vkDestroyInstance(instance, allocation_callback);
    instance = nullptr;
}

vector<VkPhysicalDevice> VulkanAPI::enumerate_gpus() {
    uint32_t gpu_count = 0;
    HandleErrors(vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr))

    vector<VkPhysicalDevice> gpus(gpu_count);
    HandleErrors(vkEnumeratePhysicalDevices(instance, &gpu_count, gpus.data()))

    return gpus;
}

std::vector<VkLayerProperties> VulkanAPI::enumerate_instance_layers() {
    terminal().set_color(TerminalColors::Gray);
    terminal() << "instance layers" << terminal_endl;

    uint32_t layer_count = 0;
    HandleErrors(vkEnumerateInstanceLayerProperties(&layer_count, nullptr))

    vector<VkLayerProperties> layers(layer_count);
    HandleErrors(vkEnumerateInstanceLayerProperties(&layer_count, layers.data()))

    for (auto layer : layers) {
        terminal() << "   " << layer.layerName << terminal_tab << terminal_tab << " | " << layer.description << terminal_endl;
    }

    terminal() << terminal_endl;

    return layers;
}

std::vector<VkLayerProperties> VulkanAPI::enumerate_device_layers() {
    terminal().set_color(TerminalColors::Gray);
    terminal() << "device layers" << terminal_endl;

    uint32_t layer_count = 0;
    HandleErrors(vkEnumerateDeviceLayerProperties(gpu, &layer_count, nullptr))

    vector<VkLayerProperties> layers(layer_count);
    HandleErrors(vkEnumerateDeviceLayerProperties(gpu, &layer_count, layers.data()))

    for (auto layer : layers) {
        terminal() << "   " << layer.layerName << terminal_tab << terminal_tab << " | " << layer.description << terminal_endl;
    }

    terminal() << terminal_endl;

    return layers;
}

void VulkanAPI::initialize_device() {
    gpu = enumerate_gpus()[0];     //TODO: choose GPU more wisely

    vkGetPhysicalDeviceProperties(gpu, &gpu_properties);
    vkGetPhysicalDeviceMemoryProperties(gpu, &gpu_memory_properties);

    terminal().set_color(TerminalColors::Gray);
    terminal() << "GPU description: " << terminal_endl <<
                  "Device name: " << gpu_properties.deviceName << terminal_endl <<
                  "Driver version: " << gpu_properties.driverVersion << terminal_endl;

    auto gpu_queue_families = enumerate_gpu_queue_families(gpu);
    graphics_queue_family_index = find_queue_family(gpu_queue_families, VK_QUEUE_GRAPHICS_BIT);
    transfer_queue_family_index = find_queue_family(gpu_queue_families, VK_QUEUE_TRANSFER_BIT);

    float queue_priorities[] {1.0f};

    vector<VkDeviceQueueCreateInfo> device_queue_infos;
    if (graphics_queue_family_index != transfer_queue_family_index)
        device_queue_infos = {
            VkDeviceQueueCreateInfo {
                .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex   = graphics_queue_family_index,
                .queueCount         = 1,
                .pQueuePriorities   = queue_priorities
            },
            VkDeviceQueueCreateInfo {
                .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex   = transfer_queue_family_index,
                .queueCount         = 1,
                .pQueuePriorities   = queue_priorities
            }
        };
    else
        device_queue_infos = {
            VkDeviceQueueCreateInfo {
                .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex   = graphics_queue_family_index,
                .queueCount         = min(2u, gpu_queue_families[graphics_queue_family_index].queueCount),
                .pQueuePriorities   = queue_priorities
            }
        };

    VkDeviceCreateInfo device_info {
        .sType                  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount   = (uint32_t)device_queue_infos.size(),
        .pQueueCreateInfos      = device_queue_infos.data(),
        .enabledLayerCount      = (uint32_t)device_layers.size(),
        .ppEnabledLayerNames    = device_layers.data(),
        .enabledExtensionCount  = (uint32_t)device_extensions.size(),
        .ppEnabledExtensionNames= device_extensions.data()
    };
    HandleErrors(vkCreateDevice(gpu, &device_info, allocation_callback, &device))

    vkGetDeviceQueue(device, graphics_queue_family_index, 0, &queue);

    /** if from same family - try use another queue **/
    if (graphics_queue_family_index != transfer_queue_family_index)
        vkGetDeviceQueue(device, transfer_queue_family_index, 0, &transfer_queue);
    else {
        uint32_t queue_index = min(1u, gpu_queue_families[transfer_queue_family_index].queueCount - 1);
        vkGetDeviceQueue(device, transfer_queue_family_index, queue_index, &transfer_queue);
    }
}

void VulkanAPI::destroy_device() {
    if (device)
        vkDestroyDevice(device, allocation_callback);
    device = nullptr;
}

void VulkanAPI::initialize(int screen_width, int screen_height, bool vSync, WindowHandle window, bool full_screen,
                           const char *application_name) {
    assert(!has_state(State::INITIALIZED));

    try {
        setup_debug_layers();
        setup_extensions();
        setup_viewport(screen_width, screen_height);
        initialize_instance(application_name);
        activate_debug();
        initialize_device();
        initialize_synchronization();
        enumerate_instance_layers();
        enumerate_device_layers();
        initialize_command_pools();
        initialize_command_buffers();
        initialize_descriptor_pool();
        initialize_pipeline_cache();
        initialize_surface(window);
        initialize_swapchain();
        initialize_swapchain_images();
        initialize_depth_stencil();
        initialize_render_pass();
        initialize_framebuffers();
    }
    catch (approx_exception& exception) {
        approx_exception("Couldn't initialize API", "VulkanAPI").becauseOf(exception)();
        shutdown();
    }


    add_state(State::INITIALIZED);
}

void VulkanAPI::shutdown() {
    destroy_framebuffers();
    destroy_render_pass();
    destroy_depth_stencil();
    destroy_swapchain_images();
    destroy_swapchain();
    destroy_surface();
    destroy_pipeline_cache();
    destroy_descriptor_pool();
    destroy_command_pools();
    destroy_synchronization();
    destroy_device();
    deactivate_debug();
    destroy_instance();

    pfnVkCreateDebugReportCallbackEXT       = nullptr;
    pfnVkDestroyDebugReportCallbackEXT      = nullptr;
}

VulkanAPI::~VulkanAPI() {
    shutdown();
}

void VulkanAPI::setup_debug_layers() {
    instance_layers = {
        "VK_LAYER_LUNARG_standard_validation"
    };
    device_layers = {
        "VK_LAYER_LUNARG_standard_validation"
    };

    debug_report_callback_info = {
        .sType          = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
        .pfnCallback    = debug_callback,
        .flags          = VK_DEBUG_REPORT_FLAG_BITS_MAX_ENUM_EXT
    };
}

void VulkanAPI::setup_extensions() {
    instance_extensions = {
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,
        NATIVE_SURFACE_EXTENSION_NAME
    };

    device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

void VulkanAPI::activate_debug() {
    if (!pfnVkCreateDebugReportCallbackEXT)
        pfnVkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

    if (!pfnVkDestroyDebugReportCallbackEXT)
        pfnVkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

    if (!pfnVkCreateDebugReportCallbackEXT || !pfnVkDestroyDebugReportCallbackEXT)
        throw approx_exception("vkGetInstanceProcAddr returned null", "VulkanAPI");

    pfnVkCreateDebugReportCallbackEXT(instance, &debug_report_callback_info, allocation_callback, &debug_report);
}

void VulkanAPI::deactivate_debug() {
    if (pfnVkDestroyDebugReportCallbackEXT)
        pfnVkDestroyDebugReportCallbackEXT(instance, debug_report, allocation_callback);
}

void VulkanAPI::initialize_command_pools() {
    // we will be resetting command buffers every frame
    VkCommandPoolCreateInfo command_pool_info {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .queueFamilyIndex   = graphics_queue_family_index,
        .flags              = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
    };
    HandleErrors(vkCreateCommandPool(device, &command_pool_info, allocation_callback, &command_pool))

    command_pool_info.queueFamilyIndex = transfer_queue_family_index;
    HandleErrors(vkCreateCommandPool(device, &command_pool_info, allocation_callback, &transfer_command_pool))
}

void VulkanAPI::destroy_command_pools() {
    if (command_pool) {
        vkDestroyCommandPool(device, command_pool, allocation_callback);
        command_pool = nullptr;
    }

    if (transfer_command_pool) {
        vkDestroyCommandPool(device, transfer_command_pool, allocation_callback);
        transfer_command_pool = nullptr;
    }
}


void VulkanAPI::initialize_command_buffers() {
    VkCommandBufferAllocateInfo command_buffer_info {
         .sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
         .commandPool           = command_pool,
         .commandBufferCount    = 1,
         .level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY
    };
    HandleErrors(vkAllocateCommandBuffers(device, &command_buffer_info, &command_buffer))

    command_buffer_info.commandPool = transfer_command_pool;
    HandleErrors(vkAllocateCommandBuffers(device, &command_buffer_info, &transfer_command_buffer))
}

void VulkanAPI::initialize_descriptor_pool() {
    // We need to tell the API the number of max. requested descriptors per type
    VkDescriptorPoolSize typeCounts[] {
        // Descriptor type uniform buffer and only one descriptor of this type
        {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
        },
    };
    // For additional types you need to add new entries in the type count list
    // E.g. for two combined image samplers :
    // typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // typeCounts[1].descriptorCount = 2;

    // Create the global descriptor pool
    // All descriptors used in this example are allocated from this pool
    VkDescriptorPoolCreateInfo descriptorPoolInfo {
        .sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext          = nullptr,
        .poolSizeCount  = static_size(typeCounts),
        .pPoolSizes     = typeCounts,
        // Set the max. number of descriptor sets that can be requested from this pool (requesting beyond this limit will result in an error)
        .maxSets        = 10,
    };

    HandleErrors(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptor_pool));
}

void VulkanAPI::destroy_descriptor_pool() {
    if (descriptor_pool) {
        vkDestroyDescriptorPool(device, descriptor_pool, allocation_callback);
        descriptor_pool = nullptr;
    }
}

void VulkanAPI::initialize_pipeline_cache() {
    VkPipelineCacheCreateInfo info {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO
    };
    HandleErrors(vkCreatePipelineCache(device, &info, nullptr, &pipeline_cache))
}

void VulkanAPI::destroy_pipeline_cache() {
    if (pipeline_cache) {
        vkDestroyPipelineCache(device, pipeline_cache, allocation_callback);
        pipeline_cache = nullptr;
    }
}

void VulkanAPI::setup_viewport(int width, int height) {
    viewport = {
        .x          = 0,
        .y          = 0,
        .width      = static_cast<float>(width),
        .height     = static_cast<float>(height),
        .maxDepth   = 1.0f,
        .minDepth   = 0.0f
    };
}

void VulkanAPI::initialize_synchronization() {
    VkFenceCreateInfo fence_info {
        .sType  = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
    };
    HandleErrors(vkCreateFence(device, &fence_info, allocation_callback, &queue_submit_fence))
    HandleErrors(vkCreateFence(device, &fence_info, allocation_callback, &transfer_queue_submit_fence))
    HandleErrors(vkCreateFence(device, &fence_info, allocation_callback, &swapchain_image_fence))
}

void VulkanAPI::destroy_synchronization() {
    destroy_fence(swapchain_image_fence);
    destroy_fence(transfer_queue_submit_fence);
    destroy_fence(queue_submit_fence);
}

void VulkanAPI::destroy_fence(VkFence & fence) {
    if (fence)
        vkDestroyFence(device, fence, allocation_callback);
    fence = nullptr;
}

void VulkanAPI::initialize_semaphore() {
    VkSemaphoreCreateInfo semaphore_info {
        .sType      = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    HandleErrors(vkCreateSemaphore(device, &semaphore_info, allocation_callback, &semaphore));
}

void VulkanAPI::destroy_semaphore() {
    if (semaphore)
        vkDestroySemaphore(device, semaphore, allocation_callback);
    semaphore = nullptr;
}

void VulkanAPI::initialize_surface(WindowHandle window) {
    surface = getSurface(instance, window);

    VkBool32 wsi_supported = VK_FALSE;
    HandleErrors(vkGetPhysicalDeviceSurfaceSupportKHR(gpu, graphics_queue_family_index, surface, &wsi_supported))
    if (!wsi_supported)
        throw approx_exception("WSI Extension is not supported", "VulkanAPI");
    HandleErrors(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &surface_capabilities))
    if(surface_capabilities.currentExtent.width < UINT32_MAX) {
        surface_width  = surface_capabilities.currentExtent.width;
        surface_height = surface_capabilities.currentExtent.height;
    }
    {
        uint32_t format_count = 0;
        HandleErrors(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr))
        if (!format_count) {
            throw approx_exception("No surface formats", "VulkanAPI");
        }
        vector<VkSurfaceFormatKHR> formats(format_count);
        HandleErrors(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, formats.data()))
        if (formats[0].format == VK_FORMAT_UNDEFINED) {     // Choose whatever we want
            surface_format = {
                .format         = VK_FORMAT_B8G8R8A8_UNORM,
                .colorSpace     = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            };
        } else {
            surface_format = formats[0];
        }
    }
}

void VulkanAPI::destroy_surface() {
    if (surface)
        vkDestroySurfaceKHR(instance, surface, allocation_callback);
    surface = nullptr;
}


void VulkanAPI::initialize_swapchain() {
    swapchain_images_count = min(swapchain_images_count, surface_capabilities.maxImageCount);
    swapchain_images_count = max(swapchain_images_count, surface_capabilities.minImageCount);

    if (!swapchain_images_count)
        throw approx_exception("Zero swapchain image count", "VulkanAPI");

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    {
        uint32_t present_mode_count = 0;
        HandleErrors(vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, nullptr))
        vector<VkPresentModeKHR> present_modes(present_mode_count);
        HandleErrors(vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, present_modes.data()))
        for (auto mode : present_modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                present_mode = mode;
                break;
            }
        }
    }
    VkSwapchainCreateInfoKHR swapchain_info {
        .sType                      = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface                    = surface,
        .minImageCount              = swapchain_images_count,
        .imageFormat                = surface_format.format,
        .imageColorSpace            = surface_format.colorSpace,
        .imageExtent.width          = surface_width,
        .imageExtent.height         = surface_height,
        .imageArrayLayers           = 1,
        .imageUsage                 = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode           = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount      = 0,
        .pQueueFamilyIndices        = nullptr,
        .preTransform               = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha             = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode                = present_mode,
        .clipped                    = VK_TRUE,
        .oldSwapchain               = nullptr
    };
    HandleErrors(vkCreateSwapchainKHR(device, &swapchain_info, allocation_callback, &swapchain))

    HandleErrors(vkGetSwapchainImagesKHR(device, swapchain, &swapchain_images_count, nullptr))
}

void VulkanAPI::destroy_swapchain() {
    if (swapchain)
        vkDestroySwapchainKHR(device, swapchain , allocation_callback);
    swapchain = nullptr;
}

void VulkanAPI::initialize_swapchain_images() {
    swapchain_images.resize(swapchain_images_count);
    swapchain_image_views.resize(swapchain_images_count);

    HandleErrors(vkGetSwapchainImagesKHR(device, swapchain, &swapchain_images_count, swapchain_images.data()))
    uint32_t i = 0;
    for (auto& view : swapchain_image_views) {
        VkImageViewCreateInfo image_view_info {
            .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image              = swapchain_images[i++],
            .viewType           = VK_IMAGE_VIEW_TYPE_2D,
            .format             = surface_format.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1
            }
        };
        HandleErrors(vkCreateImageView(device, &image_view_info, nullptr, &view))
    }
}

void VulkanAPI::destroy_swapchain_images() {
    for (auto view : swapchain_image_views) {
        vkDestroyImageView(device, view, allocation_callback);
    }
    swapchain_image_views.clear();
}

void VulkanAPI::initialize_depth_stencil() {
    {
        vector<VkFormat> try_formats {
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT,
            VK_FORMAT_D16_UNORM_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D16_UNORM
        };

        for (auto format : try_formats) {
            VkFormatProperties format_properties = {};
            vkGetPhysicalDeviceFormatProperties(gpu, format, &format_properties);
            if (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
                depth_stencil_format = format;
                break;
            }
        }

        if (depth_stencil_format == VK_FORMAT_UNDEFINED)
            throw approx_exception("No applicable depth stencil format available");

        switch (depth_stencil_format) {
            case VK_FORMAT_D32_SFLOAT_S8_UINT:
            case VK_FORMAT_D24_UNORM_S8_UINT:
            case VK_FORMAT_D16_UNORM_S8_UINT:
            case VK_FORMAT_S8_UINT:
                stencil_available = true;
                break;
            default:
                stencil_available = false;
                break;
        }
    }

    VkImageCreateInfo image_info {
        .sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .flags                  = 0,
        .imageType              = VK_IMAGE_TYPE_2D,
        .format                 = depth_stencil_format,
        .extent = {
            .width              = surface_width,
            .height             = surface_height,
            .depth              = 1
        },
        .mipLevels              = 1,
        .arrayLayers            = 1,
        .samples                = VK_SAMPLE_COUNT_1_BIT,
        .tiling                 = VK_IMAGE_TILING_OPTIMAL,
        .usage                  = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode            = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount  = VK_QUEUE_FAMILY_IGNORED,
        .pQueueFamilyIndices    = nullptr,
        .initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED
    };

    HandleErrors(vkCreateImage(device, &image_info, allocation_callback, &depth_stencil_image))

    VkMemoryRequirements image_memory_requirements {};
    vkGetImageMemoryRequirements(device, depth_stencil_image, &image_memory_requirements);

    uint32_t memory_index = find_memory_type_index(image_memory_requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (memory_index == UINT32_MAX)
        throw approx_exception("No applicable memory index found", "RendererVulkan");

    VkMemoryAllocateInfo memory_allocate_info {
        .sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize     = image_memory_requirements.size,
        .memoryTypeIndex    = memory_index
    };

    HandleErrors(vkAllocateMemory(device, &memory_allocate_info, allocation_callback, &depth_stencil_memory))
    HandleErrors(vkBindImageMemory(device, depth_stencil_image, depth_stencil_memory, 0))

    VkImageViewCreateInfo image_view_info {
        .sType              = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image              = depth_stencil_image,
        .viewType           = VK_IMAGE_VIEW_TYPE_2D,
        .format             = depth_stencil_format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .subresourceRange = {
            .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT | (stencil_available ? VK_IMAGE_ASPECT_STENCIL_BIT : (uint32_t)0),
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1
        }

    };

    HandleErrors(vkCreateImageView(device, &image_view_info,allocation_callback, &depth_stencil_image_view))
}

void VulkanAPI::destroy_depth_stencil() {
    if (depth_stencil_image_view)
        vkDestroyImageView(device, depth_stencil_image_view, allocation_callback);
    if (depth_stencil_memory)
        vkFreeMemory(device, depth_stencil_memory, allocation_callback);
    if (depth_stencil_image)
        vkDestroyImage(device, depth_stencil_image, allocation_callback);

    depth_stencil_image_view    = nullptr;
    depth_stencil_memory        = nullptr;
    depth_stencil_image         = nullptr;
}

void VulkanAPI::initialize_render_pass() {
    const uint32_t attachments_count = 2;
    VkAttachmentDescription attachments[attachments_count] {
        {
            .format             = depth_stencil_format,
            .flags              = 0,
            .samples            = VK_SAMPLE_COUNT_1_BIT,
            .loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp            = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp      = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp     = VK_ATTACHMENT_STORE_OP_STORE,
            .initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout        = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        },
        {
            .format             = surface_format.format,
            .flags              = 0,
            .samples            = VK_SAMPLE_COUNT_1_BIT,
            .loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp            = VK_ATTACHMENT_STORE_OP_STORE,
            .initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        }
    };

    VkAttachmentReference depth_stencil_attachment {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    const uint32_t color_attachment_count = 1;
    VkAttachmentReference color_attachments[color_attachment_count] {
        {
            .attachment = 1,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        }
    };

    const uint32_t subpasses_count = 1;
    VkSubpassDescription subpasses[subpasses_count] {
        {
            .pipelineBindPoint          = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount       = 0,
            .pInputAttachments          = nullptr,
            .pColorAttachments          = color_attachments,
            .colorAttachmentCount       = color_attachment_count,
            .pDepthStencilAttachment    = &depth_stencil_attachment,
        }
    };


    VkRenderPassCreateInfo render_pass_info {
        .sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount    = attachments_count,
        .pAttachments       = attachments,
        .subpassCount       = subpasses_count,
        .pSubpasses         = subpasses
    };

    HandleErrors(vkCreateRenderPass(device, &render_pass_info, allocation_callback, &render_pass))
}

void VulkanAPI::destroy_render_pass() {
    if(render_pass)
        vkDestroyRenderPass(device, render_pass, allocation_callback);
    render_pass = nullptr;
}

void VulkanAPI::initialize_framebuffers() {
    const uint32_t attachment_count = 2;

    VkImageView attachments[attachment_count] {
        depth_stencil_image_view
    };

    VkFramebufferCreateInfo framebuffer_info {
        .sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass         = render_pass,
        .width              = surface_width,
        .height             = surface_height,
        .layers             = 1,
        .attachmentCount    = attachment_count,
        .pAttachments       = attachments
    };

    framebuffers.resize(swapchain_images_count);
    for(uint32_t i = 0; i < swapchain_images_count; ++i) {
        attachments[1] = swapchain_image_views[i];

        HandleErrors(vkCreateFramebuffer(device, &framebuffer_info, allocation_callback, &framebuffers[i]))
    }
}

void VulkanAPI::destroy_framebuffers() {
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, allocation_callback);
    }

    framebuffers.clear();
}

const VkAllocationCallbacks *VulkanAPI::get_allocation_callback() const {
    return allocation_callback;
}

VkDevice VulkanAPI::get_device() const {
    return device;
}

uint32_t VulkanAPI::get_queue_family_index(bool transfer) const {
    return transfer ? transfer_queue_family_index : graphics_queue_family_index;
}

const VkCommandBuffer & VulkanAPI::get_main_command_buffer() const {
    return command_buffer;
}

VkCommandBuffer VulkanAPI::begin_transfer() {
    VkCommandBufferBeginInfo command_buffer_begin_info {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags              = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    HandleErrors(vkBeginCommandBuffer(transfer_command_buffer, &command_buffer_begin_info))
    return transfer_command_buffer;
}

void VulkanAPI::end_transfer() {
    HandleErrors(vkEndCommandBuffer(transfer_command_buffer))

    VkSubmitInfo submit_info {
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount     = 1,
        .pCommandBuffers        = &transfer_command_buffer
    };

    /* submit compiled command buffer to GPU */
    HandleErrors(vkQueueSubmit(transfer_queue, 1, &submit_info, transfer_queue_submit_fence))

    /* wait for GPU to process command buffer submission */
    HandleErrors(vkWaitForFences(device, 1, &transfer_queue_submit_fence, VK_TRUE, UINT64_MAX))
    HandleErrors(vkResetFences(device, 1, &transfer_queue_submit_fence))
}

VkDescriptorPool VulkanAPI::get_descriptor_pool() const {
    return descriptor_pool;
}

VkRenderPass VulkanAPI::get_render_pass() const {
    return render_pass;
}

VkPipelineCache VulkanAPI::get_pipeline_cache() const {
    return pipeline_cache;
}

VkPhysicalDeviceMemoryProperties VulkanAPI::get_device_memory_properties() const {
    return gpu_memory_properties;
}
