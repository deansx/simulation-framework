/*****************************************************************************
*
*   DESCRIPTION:
*     main() function for the code to test the common_messages and
*     common_strings modules.
*     
*     The specific test to execute is controlled by a command line argument.
*     
*     Many of the tests cause an "EXIT_FAILURE", so the program needs to be
*     invoked separarely to execute each one.
*     
*     No "help" is provided, since this executable is intended to be run
*     under the control of the test script.
*
*     
*   STATUS:  Prototype
*   VERSION:  1.00
*   CODER:  Dean Stevens
*   
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
*****************************************************************************/
#include <cstring>
#include <iostream>
#include <sstream>

#include "common_strings.hpp"
#include "common_messages.hpp"


int main(int argc, char *argv[]) {

  std::cout << std::endl;

  // All of these, except the common string section should crash
  char *test;
  if (argc >= 2) {
    // strcpy(argv[1], test);
    test = argv[1];
    std::cout << "Running " << test << " test...\n";
  } else {
    // 3 line message
    UtilFatalErrorAndDie("You must specify the name of the test to be run on"
                         " the\ncommand line.\nNo test specified.");
  }
  
  if (!strcmp("ONE_LINE", test)) {
    // A single message line
    UtilStdMsg("foobert", "Foo  1");
    UtilStdMsg(kCommonStrError, "Line 1");
    UtilStdMsg(kCommonStrWarn, "Line 1");
    UtilStdMsg(kCommonStrNote, "Line 1");
    UtilStdMsg(kCommonStrFatal, "DEAD 1");
    UtilFatalErrorAndDie("Line 1");
  } else if (!strcmp("TWO_LINE", test)) {
    // Two message lines
    UtilStdMsg(kCommonStrError, "Line 1\nLine 2");
    UtilStdMsg(kCommonStrWarn, "Line 1\nLine 2");
    UtilStdMsg(kCommonStrNote, "Line 1\nLine 2");
    UtilFatalErrorAndDie("Line 1\nLine 2");
  } else if (!strcmp("THREE_LINE", test)) {
    // Three message lines
    UtilStdMsg(kCommonStrError, "Line 1\nLine 2\nLine 3");
    UtilStdMsg(kCommonStrWarn, "Line 1\nLine 2\nLine 3");
    UtilStdMsg(kCommonStrNote, "Line 1\nLine 2\nLine 3");
    UtilFatalErrorAndDie("Line 1\nLine 2\nLine 3");
  } else if (!strcmp("FOUR_LINE", test)) {
    // Four message lines
    UtilFatalErrorAndDie("Line 1\nLine 2\nLine 3\nLine 4");
  } else if (!strcmp("FIVE_LINE", test)) {
    // Five message lines
    UtilFatalErrorAndDie("Line 1\nLine 2\nLine 3\nLine 4\nLine 5");
  } else if (!strcmp("SIX_LINE", test)) {
    // Six message lines
    UtilFatalErrorAndDie("Line 1\nLine 2\nLine 3\nLine 4\nLine 5\nLine 6");
  } else if (!strcmp("SEVEN_LINE", test)) {
    // Seven message lines
    UtilFatalErrorAndDie("Line 1\nLine 2\nLine 3\nLine 4\nLine 5\n"
                         "Line 6\nLine 7");
  } else if (!strcmp("BAD_STREAM", test)) {
    // Log output class tests
    UtilFatalErrorAndDie("Unable to write output record.\n"
                         "Output stream either not open, or returned bad "
                         "status.\n");
  } else if (!strcmp("TIME_OVER", test)) {
    // Time overflow test, using stringstream to create one of the lines
    std::stringstream line1;
    long double add_time = 2.7238E14;
    long double to_time = 3.14162E11;
    line1 << "Adding " << add_time << " to " 
          << to_time << " would exceed\npermissible range.";
    UtilFatalErrorAndDie(line1.str());
  } else {
    // Unrecognized argument
    std::stringstream line1;
    line1 << "\"" << test << "\" Not Recognized!!!\nNo tests will be run.";
    UtilFatalErrorAndDie(line1.str());
  }
  
  return 1;
}
