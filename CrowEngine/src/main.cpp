#include <iostream>
#include "Core/App.hpp"
#include "Utils/GenerateDefaults.hpp"

int main()
{
  GenerateDefaultPipelineConfig();
  GenerateDefaultRenderPassConfig();

  core::App& app = core::App::GetInstance();

  app.StartApplication();

  app.RunApplication();

  if(app.ShutdownApplication())
  {
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
