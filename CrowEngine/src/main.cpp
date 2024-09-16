#include <iostream>
#include "Core/App.hpp"

int main()
{
  crowe::App& app = crowe::App::GetInstance();

  app.StartApplication();

  app.RunApplication();

  if(app.ShutdownApplication())
  {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
