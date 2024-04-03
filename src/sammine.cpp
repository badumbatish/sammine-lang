//
// Created by jjasmine on 3/7/24.
//
//! \file sammine.cpp
//! \brief The main file to produce an executable that is the sammine compiler

#include "Compiler.h"
#include "argparse/argparse.hpp"
using sammine_lang::Compiler;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("sammine");

  auto &group = program.add_mutually_exclusive_group(true);
  group.add_argument("-f", "--file")
      .help("An input file for compiler to scan over.");
  group.add_argument("-s", "--str")
      .help("An input string for compiler to scan over.");

  try {
    program.parse_args(argc, argv); // Example: ./main -abc 1.95 2.47
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  std::string f = program.present("-f") ? program.get("-f") : "String-input";
  std::string string_input =
      program.present("-s") ? program.get("-s") : FileRAII(f).getInternalStr();
  Compiler jjasmine(string_input, f);
  jjasmine.start();
  return 0;
}
