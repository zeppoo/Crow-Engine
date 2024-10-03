#include <iostream>
#include "Core/App.hpp"
#include "TextEditor/FileOpener.hpp"

int main()
{
  const char* TEST_PATH = R"("C:\Users\cleme\Documents\School\Lj3\BO\Periode 1\Crow-Engine\CrowEngine\src\TextEditor\config.txt")";

  crowe::App& app = crowe::App::GetInstance();
  FileOpener::Open(TEST_PATH);

  app.StartApplication();

  app.RunApplication();

  if(app.ShutdownApplication())
  {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
