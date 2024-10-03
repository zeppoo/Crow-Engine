#include "FileOpener.hpp"

namespace FileOpener
{
  void Open(const char* FILE_PATH)
  {
    //const char* appPath = "notepad.exe";
    const char* EDITOR_PATH = GetChosenEditor();// Replace with your file path

    char FULL_COMMAND[100];
    std::sprintf(FULL_COMMAND,"%s %s .", EDITOR_PATH, FILE_PATH);

    system(FULL_COMMAND);
  }

  void CompileShaders(std::string NAME)
  {
    const char* shaderTypeFlag;
    if (NAME.find("vert")) {
      shaderTypeFlag = "-V";
    } else if (NAME.find("frag")) {
      shaderTypeFlag = "-F";
    } else {
      throw std::runtime_error("Unsupported shader type: " + NAME);
    }

    // Command to invoke glslangValidator

    char FULL_COMMAND[100];
    std::sprintf(FULL_COMMAND, "glslangValidator -%s %s -o CompiledShaders/%s.spv", shaderTypeFlag, NAME.c_str(), NAME.c_str());

    system(FULL_COMMAND);
  }

  const char* GetChosenEditor()
  {
    /*
    std::ifstream CONFIG_FILE(R"("C:\Users\cleme\Documents\School\Lj3\BO\Periode 1\Crow-Engine\CrowEngine\src\TextEditor\config.txt")");

    //CONFIG_FILE.open("config.txt");
    //CONFIG_FILE

    std::string line;
    if (CONFIG_FILE.is_open()) {
      std::cout << "this is being pass" << std::endl;
      while (std::getline(CONFIG_FILE, line))
      {
        std::cout << "line is: " << line << std::endl;
      }
    }

    CONFIG_FILE.close();

    const char* EDITOR_PATH = line.c_str();

    CONFIG_FILE.close();

    return EDITOR_PATH;
    */

    return "notepad";
  }

  void ChangeEditor(const char* EDITOR_PATH)
  {

  }
}
