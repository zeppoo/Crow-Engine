#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>

namespace FileOpener
{
  //const char* DEFAULT_EDITOR = "notepad.exe";
  //const char* CONFIG_PATH = "config.txt";

  void Open(const char* PATH);

  const char* GetChosenEditor();

  void CompileShaders();

  void ChangeEditor(const char* PATH);
}