#include <iostream>
#include "Core/App.hpp"

int main()
{
  crowe::App app{};

  try {
    app.StartApplication();

  } catch (const std::exception &e)
  {
    std::cerr << "Error in main: " << e.what() << '\n';
    return EXIT_FAILURE;
  }

  app.RunApplication();

  app.ShutdownApplication();

  return EXIT_SUCCESS;
}
