#include "Core/App.hpp"
#include "Logger.hpp"
#include "Config/SettingsManager.hpp"
#include "Core/Window.hpp"

namespace core
{
  App App::instance;

  App::App()
  {
    logger::Info("Application Started");
  }

  void App::StartApplication()
  {

  }

  void App::RunApplication()
  {

  }

  bool App::ShutdownApplication()
  {

  }
}