/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the declaration of the method to display the help file
*     for a program using the standard ArgParser class.
*     If the arguments processed by ArgParser change, this method will
*     need to change, as well.
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

#ifndef SIM_UTIL_DISPLAY_HELP_HPP_
#define SIM_UTIL_DISPLAY_HELP_HPP_
#include <string>

// Display the help message on cout.
//
// "exe_name" is a string containing the full name of the executable, for
//       example, "text_event.exe".
// "description" is a string containing the description text for the
//       executable.  This text should be formatted in a fashion consistent
//       with the rest of the help message.  See arg_parser_main.cc in
//       tests/arg_parser for an example.
extern void DisplayHelp(const std::string &exe_name, 
                        const std::string &description);

#endif
