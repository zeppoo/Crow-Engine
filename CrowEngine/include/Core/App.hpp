#pragma once

#include "Window.hpp"
#include "../Vulkan/BackendInterface.hpp"
#include <memory>
namespace core
{
  enum AppState {
    APP_STATE_IDLE = 0,
    APP_STATE_RUNNING,
    APP_STATE_SHUTDOWN,
  };

  class App {
  public:
    static App &GetInstance()
    { return instance; }

    void StartApplication();

    void RunApplication();

    bool ShutdownApplication();

  private:
    App();

    App(const App &) = delete;

    App &operator=(const App &) = delete;

    // Members
    static App instance;
  };
}
