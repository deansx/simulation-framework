/*****************************************************************************
*
*   DESCRIPTION:
*     main() function for the ArgParser sample code.  Demonstrates the
*     simplest case of using ArgParser to process the arguments to the
*     executable built with this source.
*
*     The executable recognizes and processes the arguments documented
*     in the default help file.
*     
*   STATUS:  Prototype
*   VERSION:  0.00
*   CODER:  Dean Stevens
*   
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
*****************************************************************************/

#include <iostream>
#include "arg_parser.hpp"
#include "display_help.hpp"


// Description for the help
const std::string help_descr = 
    "    arg_parser.exe is a test framework that both demonstrates the\n"
    "    simplest possible implementation of the ArgParser class, and\n"
    "    provides a test fixture for the class.\n";

int main(int argc, char *argv[]) {

  ArgParser arg_parser;

  if (arg_parser.ParseArgs(argc, argv)) {
    if (arg_parser.parsed_args().display_help_) {
      DisplayHelp("arg_parser.exe", help_descr);
    } else {
      std::cout << "Nice job:  All arguments OK\n";
      arg_parser.parsed_args().Dump();
    }
    return 0;
  } else {
    std::cerr << "Oops, problem with the arguments\n";
    return 1;
  }
}
