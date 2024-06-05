#ifndef VULK_DEVICE
#define VULK_DEVICE

#include "../crow_lib.hpp"

namespace crowe
{
 void FindPhysicalDevice();
 void CreateLogicalDevice();
 void CheckDeviceSuitability(VkPhysicalDevice device);
}

#endif //VULK_DEVICE
