#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

void printTree(const fs::path& path, int level = 0) {
  for (const auto& entry : fs::directory_iterator(path)) {
    std::cout << std::string(level * 2, ' ') << entry.path().filename() << '\n';
    if (fs::is_directory(entry)) {
      printTree(entry, level + 1);
    }
  }
}

int main()
{
  printTree(R"(D:\Personal\coding\C++\Projects\Crow-Engine\CrowEngine)");
  return 0;
}
