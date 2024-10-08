//
// Created by jjasmine on 3/7/24.
//
//! \file sammine.cpp
//! \brief The main file to produce an executable that is the sammine compiler

#include "Compiler.h"
#include "fmt/color.h"
#include <argparse/argparse.hpp>
using sammine_lang::Compiler;
using sammine_lang::compiler_option_enum;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("sammine");

  std::map<compiler_option_enum, std::string> compiler_options;

  auto &group_input = program.add_mutually_exclusive_group(true);
  group_input.add_argument("-f", "--file")
      .help("An input file for compiler to scan over.");
  group_input.add_argument("-s", "--str")
      .help("An input string for compiler to scan over.");

  auto &group_diagnostics = program.add_group("diagnostics");
  group_diagnostics
      .add_argument("", "--llvm-ir") // TODO: Somehow make the internal compiler
      .default_value(std::string("false"))
      .implicit_value(std::string("true"))
      .help("sammine compiler spits out LLVM-IR to stdout");
  group_diagnostics.add_argument("", "--diagnostics")
      .default_value(std::string("false"))
      .implicit_value(std::string("true"))
      .help(
          "sammine compiler spits out diagnostics for sammine-lang developers");
  try {
    program.parse_args(argc, argv); // Example: ./main -abc 1.95 2.47
    compiler_options[compiler_option_enum::FILE] =
        program.present("-f") ? program.get("-f") : "";
    compiler_options[compiler_option_enum::STR] =
        program.present("-s") ? program.get("-s") : "";
    compiler_options[compiler_option_enum::LLVM_IR] = program.get("--llvm-ir");
    compiler_options[compiler_option_enum::DIAGNOSTIC] =
        program.get("--diagnostics");

  } catch (const std::exception &err) {
    fmt::print(stderr, fg(fmt::color::red), "Error while parsing arguments\n");
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  Compiler jjasmine(compiler_options);
  jjasmine.start();
  return 0;
}
