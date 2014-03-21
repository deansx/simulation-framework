/*****************************************************************************
*
*   DESCRIPTION:
*     shared_test_code defines functions that are used by multiple test
*     drivers to provide common functionality.
*     
*     These functions are only expected to be used for test infrastructure,
*     not for production code.
*
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
#include <string>
#include <fstream>
#include <iostream>

#include "shared_test_code.hpp"
#include "common_messages.hpp"


void SharedPrintFinalResults(const std::string &label,
                             const unsigned int passed,
                             const unsigned int failed) {
  std::cout << "===== " << label << ":  =====\n";
  std::cout << "      Passed:  " << passed << std::endl;
  std::cout << "      Failed:  " << failed << std::endl;
}

void SharedExportPassCount(const unsigned int passed) {
  std::string pass_path("./test_out/pass_count.txt");
  std::ofstream pass_stream(pass_path);
  if (pass_stream.is_open() && pass_stream.good()) {
    pass_stream << passed;
    pass_stream.close();
  } else {
    // Script needs the file there, so we'll exit badly.
    std::string message("Unable to open \"");
    message.append(pass_path).append("\" for recording count of tests "
                                     "passed.");
    UtilFatalErrorAndDie(message);
  }
}
