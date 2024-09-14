//
// Created by jjasmine on 3/7/24.
//
//! \file sammine.cpp
//! \brief The main file to produce an executable that is the sammine compiler

#include "Compiler.h"
#include "argparse/argparse.hpp"
using sammine_lang::Compiler;
using sammine_lang::compiler_option_enum;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("sammine");

  std::map<compiler_option_enum, std::string> compiler_options;

  auto &group = program.add_mutually_exclusive_group(true);
  group.add_argument("-f", "--file")
      .help("An input file for compiler to scan over.");
  group.add_argument("-s", "--str")
      .help("An input string for compiler to scan over.");
  group
      .add_argument("", "--llvm-ir") // TODO: Somehow make the internal compiler
                                     // aware of this llvm flag
      .default_value("true")
      .implicit_value("false")
      .help("sammine compiler spits out LLVM-IR to stdout");
  try {
    program.parse_args(argc, argv); // Example: ./main -abc 1.95 2.47
    if (program.present<std::string>("-f"))
      compiler_options[compiler_option_enum::FILE] =
          program.present("-f") ? program.get("-f") : "String-input";
    if (program.present("-s"))
      compiler_options[compiler_option_enum::STR] =
          program.present("-s") ? program.get("-s")
                                : FileRAII(program.get("-f")).getInternalStr();
    compiler_options[compiler_option_enum::LLVM_IR] = program.get("--llvm-ir");

    std::cerr << compiler_options[compiler_option_enum::STR] << std::endl;

  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  Compiler jjasmine(compiler_options);
  jjasmine.start();
  return 0;
}
