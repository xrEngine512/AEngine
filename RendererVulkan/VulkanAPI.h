#pragma once
//
// Created by islam on 11.03.17.
//

#include <IGraphicsAPI.h>
#include <MState.h>

#include <vulkan/vulkan.h>
#include <vector>

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


class ObjectFactory;

class VulkanAPI: public IGraphicsAPI, public MState {
    friend class ObjectFactory;

    VkInstance                          instance                            = nullptr;
    VkPhysicalDevice                    gpu                                 = nullptr;
    VkDevice                            device                              = nullptr;
    VkQueue                             queue                               = nullptr,
                                        transfer_queue                      = nullptr;
    VkFence                             queue_submit_fence                  = nullptr,
                                        transfer_queue_submit_fence         = nullptr,
                                        swapchain_image_fence               = nullptr;
    VkSemaphore                         semaphore                           = nullptr;
    VkCommandPool                       command_pool                        = nullptr,
                                        transfer_command_pool               = nullptr;
    VkCommandBuffer                     command_buffer                      = nullptr,
                                        transfer_command_buffer             = nullptr;
    VkDescriptorPool                    descriptor_pool                     = nullptr;
    VkPipelineCache                     pipeline_cache                      = nullptr;
    VkSurfaceKHR                        surface                             = nullptr;
    VkSwapchainKHR                      swapchain                           = nullptr;
    VkRenderPass                        render_pass                         = nullptr;
    VkImage                             depth_stencil_image                 = nullptr;
    VkImageView                         depth_stencil_image_view            = nullptr;
    VkDeviceMemory                      depth_stencil_memory                = nullptr;
    VkFormat                            depth_stencil_format                = VK_FORMAT_UNDEFINED;
    bool                                stencil_available                   = false;

    VkDebugReportCallbackEXT            debug_report                        = nullptr;
    uint32_t                            graphics_queue_family_index         = 0,
                                        transfer_queue_family_index         = 0;
    uint32_t                            swapchain_images_count              = 2;
    uint32_t                            surface_width                       = 0,
                                        surface_height                      = 0;
    uint32_t                            active_swapchain_image_id           = UINT32_MAX;

    std::vector<const char*>            instance_layers                     = {},
                                        instance_extensions                 = {},
                                        device_layers                       = {},
                                        device_extensions                   = {};
    std::vector<VkImage>                swapchain_images                    = {};
    std::vector<VkImageView>            swapchain_image_views               = {};
    std::vector<VkFramebuffer>          framebuffers                        = {};

    VkViewport                          viewport                            = {};
    VkSurfaceFormatKHR                  surface_format                      = {};
    VkSurfaceCapabilitiesKHR            surface_capabilities                = {};
    VkPhysicalDeviceProperties          gpu_properties                      = {};
    VkPhysicalDeviceMemoryProperties    gpu_memory_properties               = {};
    VkDebugReportCallbackCreateInfoEXT  debug_report_callback_info          = {};

    VulkanAPI();

    static std::vector<VkQueueFamilyProperties> enumerate_gpu_queue_families(VkPhysicalDevice gpu);
    static uint32_t find_queue_family(const std::vector<VkQueueFamilyProperties> &, VkQueueFlags);

    VkClearColorValue convert(const Math::AVector& color);

    void initialize_instance(const char* application_name);
    void destroy_instance ();

    std::vector<VkPhysicalDevice> enumerate_gpus();
    std::vector<VkLayerProperties> enumerate_instance_layers();
    std::vector<VkLayerProperties> enumerate_device_layers();

    void initialize_device();
    void destroy_device();

    void initialize_synchronization();
    void destroy_synchronization();
    void destroy_fence(VkFence&);

    void initialize_semaphore();
    void destroy_semaphore();

    void initialize_command_pools();
    void destroy_command_pools();

    void initialize_command_buffers();

    void initialize_descriptor_pool();
    void destroy_descriptor_pool();

    void initialize_pipeline_cache();
    void destroy_pipeline_cache();

    void setup_viewport(int width, int height);

    void initialize_surface(WindowHandle window);
    void destroy_surface();

    void initialize_swapchain();
    void destroy_swapchain();

    void initialize_framebuffers();
    void destroy_framebuffers();

    void initialize_depth_stencil();
    void destroy_depth_stencil();

    void initialize_render_pass();
    void destroy_render_pass();

    void initialize_swapchain_images();
    void destroy_swapchain_images();

    void setup_debug_layers();
    void setup_extensions();
    void activate_debug();
    void deactivate_debug();

public:
    /** Global interface **/
    void initialize(int screen_width, int screen_height, bool vSync, WindowHandle window, bool full_screen,
                    const char *application_name) override;

    bool begin_scene(const Math::AVector &color) override;

    bool end_scene() override;

    bool set_zbuffer_enabled(bool enabled) override;

    bool set_alpha_blending_enabled(bool enabled) override;

    void shutdown() override;

    ~VulkanAPI();

    /** Local interface **/
    const VkAllocationCallbacks* get_allocation_callback() const;

    VkDevice get_device() const;

    uint32_t get_queue_family_index(bool transfer = false) const;

    const VkCommandBuffer & get_main_command_buffer() const;

    uint32_t find_memory_type_index(const VkMemoryRequirements&, const VkMemoryPropertyFlags) const;

    VkCommandBuffer begin_transfer();

    void end_transfer();

    VkDescriptorPool get_descriptor_pool() const;

    VkRenderPass get_render_pass() const;

    VkPipelineCache get_pipeline_cache() const;

    VkPhysicalDeviceMemoryProperties get_device_memory_properties() const;
};



