/*****************************************************************************
*
*   DESCRIPTION:
*     shared_test_code declares functions that are used by multiple test
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

#ifndef SIM_TESTS_SHARED_TEST_CODE_HPP_
#define SIM_TESTS_SHARED_TEST_CODE_HPP_

#include <string>

// Display a block of text on stdout that shows the final pass/fail
// results for a given test
//
// "label" - text to display in the header for the results block
// "passed" - count of tests that passed
// "failed" - count of tests that failed
extern void SharedPrintFinalResults(const std::string &label,
                                    const unsigned int passed,
                                    const unsigned int failed);

// Writes the pass count to a standard path so that the test shell script
// can read it.
//
// "passed" - count of tests that passed
extern void SharedExportPassCount(const unsigned int passed);


#endif  // SIM_TESTS_SHARED_TEST_CODE_HPP_
