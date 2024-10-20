#include <iostream>
#include "Core/App.hpp"
#include "Utils/GenerateDefaults.hpp"

int main()
{
  GenerateDefaultPipelineConfig();
  GenerateDefaultRenderPassConfig();

  crowe::App& app = crowe::App::GetInstance();

  app.StartApplication();

  app.RunApplication();

  if(app.ShutdownApplication())
  {
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
