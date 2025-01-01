//
// Created by jjasmine on 3/8/24.
//

#pragma once
#include <fstream>
#include <string>

//! \file FileRAII.h
//! \brief A RAII-wrapper around C++ file type to use in the project for lexer
//! and parser

//! \brief FileRAII class
class FileRAII {
public:
  // Constructor opens the file
  explicit FileRAII(const std::string &filename)
      : file(filename), is_opened(true) {
    if (!file.is_open()) {
      is_opened = false;
    }

    str = std::string((std::istreambuf_iterator<char>(file)),
                      (std::istreambuf_iterator<char>()));
  }

  // Destructor closes the file
  ~FileRAII() {
    if (is_opened) {
      file.close();
    }
  }

  // Function to check if the file is open
  bool isOpen() const { return is_opened; }

  std::string getInternalStr() { return str; }

private:
  std::ifstream file;
  bool is_opened;
  std::string str;
};
