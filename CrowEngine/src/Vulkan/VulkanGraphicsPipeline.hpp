#pragma once

#include "../crow_lib.hpp"

namespace crowe
{
    class VulkanGraphicsPipeline {
    public:
        VulkanGraphicsPipeline();
        ~VulkanGraphicsPipeline();

    private:
        void createGraphicsPipeline();

    };
}
