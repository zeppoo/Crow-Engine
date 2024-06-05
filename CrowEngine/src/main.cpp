#include <iostream>
#include "Core/App.hpp"

int main()
{
  crowe::App app{};

  app.StartApplication();

  app.RunApplication();

  app.ShutdownApplication();
}
