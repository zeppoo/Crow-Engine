#include "SettingsManager.hpp"

namespace crowe
{
  SettingsManager SettingsManager::instance;

  SettingsManager::SettingsManager() {}

  SettingsManager& SettingsManager::getInstance()
  {
    return instance;
  }

  void SettingsManager::destroyInstance() {
  }
}
