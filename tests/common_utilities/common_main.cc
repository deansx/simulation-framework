/*****************************************************************************
*
*   DESCRIPTION:
*     main() function for the code to test the common_utilities
*     
*     The specific test to execute is controlled by a command line argument.
*     
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
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>

#include "common_strings.hpp"
#include "common_messages.hpp"
#include "common_utilities.hpp"
#include "shared_test_code.hpp"


// Class to manage pass/fail score information & to print each test
// result as the test is completed.
class Scores {
 public:
  Scores() : passed_(0), failed_(0) {}
  ~Scores() { }

  // Passing tests call one of the two "Pass()" methods to record
  // success and to notify the user that the test was successful.

  // Passing tests call this method to increment their counter
  //
  // "test_name" name of the test that just passed
  void Pass(const std::string &test_name) {
    Passed(test_name, "");
  }
  
  // Passing tests call this method to increment their counter, if they
  // need to display additional message text along with the line stating
  // that the test passed.
  //
  // "test_name" name of the test that just passed
  // "message" the addional text to display, after the
  //       "test_name Passed..." line
  void Pass(const std::string &test_name, const std::string &message) {
    Passed(test_name, message);
  }

  // Failing tests call one of the two "Fail()" methods to record
  // failure and to notify the user that the test was not successful.

  // Failing tests call this method to increment their counter
  //
  // "test_name" name of the test that just failed
  void Fail(const std::string &test_name) {
    Failed(test_name, "");
  }

  // Failing tests call this method to increment their counter, if they
  // need to display additional message text along with the line stating
  // that the test failed.
  //
  // "test_name" - name of the test that just failed
  // "message" - the addional text to display, after the
  //       "test_name Failed..." line
  void Fail(const std::string &test_name, const std::string &message) {
    Failed(test_name, message);
  }

  // Access the current counts

  // Returns - the number of tests that have passed
  unsigned int passed() { return passed_; }

  // Returns - the number of tests that have failed
  unsigned int failed() { return failed_; }

 private:
  // Actual score keeper methods, these are wrapped by overloaded public
  // Pass()/Fail() methods:

  // Passing tests...
  //
  // "test_name" - name of the test that just passed
  // "message" - the addional text to display, after the standard
  //       test identification line
  void Passed(const std::string &test_name, const std::string &message) {
    std::cout << "   " << test_name << " Passed...\n";
    // Optional message
    if (!message.empty()) {
      std::cout << "      " << message << std::endl;
    }
    passed_++;
  }
  // Failing tests...
  void Failed(const std::string &test_name, const std::string &message) {
    std::cerr << "!!!" << test_name << " Failed...\n";
    // Optional message
    if (!message.empty()) {
      std::cout << "      " << message << std::endl;
    }
    failed_++;
  }
  // Pass/Fail Counters
  unsigned int passed_;
  unsigned int failed_;
};

// Global instance to manage scores.
Scores g_scores;


// function wrappers for corresponding UtilFile* functions, so that we
// can use a function pointer to all of them in run test
// For all of these:
// "filename" - pathname of the file to test
// "access" - Read/Write etc. access to be tested
// Returns - "true" if the file meets the specified conditions, "false"
//       otherwise.
bool FileExists(const std::string& filename, const mode_t access) {
  return UtilFileExists(filename);
}
bool FileExistsRead(const std::string& filename, const mode_t access) {
  return UtilFileExistsRead(filename);
}
bool FileExistsWrite(const std::string& filename, const mode_t access) {
  return UtilFileExistsWrite(filename);
}
bool FileExistsReadWrite(const std::string& filename, const mode_t access) {
  return UtilFileExistsReadWrite(filename);
}


// Function pointer type to abstract the function used for the specific
// test.
typedef  bool (*FileValid)(const std::string&, mode_t access);


// Message identifying the test currently being run.
//
// "test" - the name of the test that is being run
// "file" - the pathname to the file that will be tested.
void RunningMsg(const std::string &test, const std::string &file) {
  std::cout << "\nRunning:  " << test << " test on " << file << "...\n";
}


// Tests manipulating, and resetting, a stream's format parameters.
void StreamFormatTest() {
  std::stringstream test_stream;
  long double pi = 3.1415926535897932384626434L;
  long double pi_exp = pi * 1.0e6L;
  float small_float = 123.45;
  test_stream << "Orig Format:\n"
              << "abc " << small_float << " " << pi
              << " " << pi_exp << "\nChg format:\n";
  UtilStreamSettings cacheset = UtilSetStreamMaxPrecision(&test_stream);
  test_stream << "def " << small_float << " " << pi 
              <<  " " << pi_exp;
  UtilSetStreamSettings(cacheset, &test_stream);
  test_stream << "\nSettings should be back to normal:\n" 
              << "ghi " << small_float << " " << pi 
              <<  " " << pi_exp;
  std::cout << test_stream.str() << std::endl;
  std::string ref_str = "Orig Format:\nabc 123.45 3.14159 3.14159e+06\n"
      "Chg format:\ndef                             1.23449996948242187"
      "500000000000000E+02 3.14159265358979323851280895940619E+00 3.141"
      "59265358979323855237453244627E+06\n"
      "Settings should be back to normal:\n"
      "ghi 123.45 3.14159 3.14159e+06";

  std::string test_name("STREAM_FMT");
  if (ref_str == test_stream.str()) {
    g_scores.Pass(test_name, "");
  } else {
    g_scores.Fail(test_name, "");
  }
}


// Run a specied file exists / access test.
//
// "test_name" - the name of the test
// "file_base" - the leaf name of the file path
// "validate_file" - contains the function pointer to the test function used
//       for this test.
// "access" - Read/Write access type to check
// "flip_eval" - Reverses the sense of the evaluation.  If we expect the
//       test to return "false", this value should be "true"
void RunTest(const std::string &test_name,
             const std::string &file_base,
             FileValid validate_file,
             const mode_t access,
             const bool flip_eval) {
  static const std::string ref_root("test_ref/");
  static const std::string not_test("NOT_");
  RunningMsg(test_name, file_base);
  if (validate_file(ref_root + file_base, access)) {
    // validate returned "true",
    if (!flip_eval) {
      // we're not reversing the sense of the evaluation, so this passes
      g_scores.Pass(test_name);
    } else {
      // validate returned "true", but we're reversing the sense, so the test
      // failed.  This could happen, for example, if validate told us that a
      // read only file existed and was available for write.
      g_scores.Fail(test_name, "Validate was true, it should have been "
                               "false...");
    }
  } else {
    // validate returned "false"
    if (!flip_eval) {
      // we're not reversing the sense of the evaluation, so this fails
      g_scores.Fail(test_name);
    } else {
      // We expected this to be "false, for example we tested a write only
      // file for read access.
      g_scores.Pass(test_name);
    }
  }
  // "NOT" files never exist, so these don't get reversed.  Validate
  // should ALWAYS return "false"
  if (!validate_file(ref_root + not_test + file_base, access)) {
    g_scores.Pass(not_test + test_name);
  } else {
    g_scores.Fail(not_test + test_name);
  }
}

int main(int argc, char *argv[]) {

  // Test Scores Class, not part of normal module test, but can be used
  // if a change is made to the Scores class code.
  if (0) {
    g_scores.Pass("PASS_NO_MESSAGE", "");
    g_scores.Pass("PASS_WITH_MESSAGE", "Very nice pass message!");
    g_scores.Fail("FAIL_NO_MESSAGE", "");
    g_scores.Fail("FAIL_WITH_MESSAGE", "Very nice fail message!");
    unsigned int pass = g_scores.passed();
    unsigned int fail = g_scores.failed();
    std::cout << "Scores Class Test:\n"
              << "   Passed: " << pass << std::endl
              << "   Failed: " << fail << std::endl;
    if ((pass == 2) && (fail == 2)) {
      exit(EXIT_SUCCESS);
    } else {
      exit(EXIT_FAILURE);
    }
  }

  // Specific tests, so that we can use them as case values for a switch
  // statement.
  enum TestToRun { FILE_EXISTS, FILE_READ, FILE_WRITE, FILE_RW, STREAM_FMT };

  std::cout << std::endl;
  
  char *test;
  bool single_test;
  TestToRun test_arg;
  if (argc >= 2) {
    // User specified an argument, so we might be running the specified test
    single_test = true;
    test = argv[1];
    if (!strcmp("FILE_EXISTS", test)) {
      test_arg = FILE_EXISTS;
    } else if (!strcmp("FILE_READ", test)) {
      test_arg = FILE_READ;
    } else if (!strcmp("FILE_WRITE", test)) {
      test_arg = FILE_WRITE;
    } else if (!strcmp("FILE_RW", test)) {
      test_arg = FILE_RW;
    } else if (!strcmp("STREAM_FMT", test)) {
      test_arg = STREAM_FMT;
    } else if (!strcmp("X", test)) {
      test_arg = FILE_EXISTS;
    } else if (!strcmp("X", test)) {
      test_arg = FILE_EXISTS;
    } else if (!strcmp("X", test)) {
      test_arg = FILE_EXISTS;
    } else if (!strcmp("X", test)) {
      test_arg = FILE_EXISTS;
    } else {
      // Unrecognized argument
      std::stringstream line1;
      line1 << "\"" << test << "\" Not Recognized!!!\nNo tests will be run.";
      UtilFatalErrorAndDie(line1.str());
    }
  } else {
    // no test specified, so we'll run them all
    single_test = false;
    UtilStdMsg(kCommonStrNote, "Running all common_utilities tests...");
  }

  // Switch based on the argument.  Note:  If no argument specified, we
  // execute all known tests.
  switch (test_arg) {
    case FILE_EXISTS:
      // test for existence of a file, both positive & negative.
      RunTest("FILE_EXISTS", "f_exists_rw.txt", FileExists, 0, false);
      RunTest("FILE_EXISTS", "f_exists_exe.exe", FileExists, 0, false);
      RunTest("FILE_EXISTS", "dir_exists", FileExists, 0, false);
      if (single_test) break;
    case FILE_READ:
      RunTest("FILE_READ", "f_exists_rw.txt", FileExistsRead, 0, false);
      RunTest("FILE_READ", "f_exists_r.txt", FileExistsRead, 0, false);
      // file exists, but is write-only, so flip sense of validate
      RunTest("FILE_READ", "f_exists_w.txt", FileExistsRead, 0, true);
      // dir exists, but is not a regular file, so flip sense of validate
      RunTest("FILE_READ", "dir_exists", FileExistsRead, 0, true);
      RunTest("FILE_READ_ACCESS", "f_exists_r.txt", UtilFileExistsAccess,
              S_IRUSR, false);
      // file exists, but is write-only, so flip sense of validate
      RunTest("FILE_READ_ACCESS", "f_exists_w.txt", UtilFileExistsAccess,
              S_IRUSR, true);
      if (single_test) break;
    case FILE_WRITE:
      RunTest("FILE_WRITE", "f_exists_rw.txt", FileExistsWrite, 0, false);
      RunTest("FILE_WRITE", "f_exists_w.txt", FileExistsWrite, 0, false);
      // file exists, but is write-only, so flip sense of validate
      RunTest("FILE_WRITE", "f_exists_r.txt", FileExistsWrite, 0, true);
      RunTest("FILE_WRITE_ACCESS", "f_exists_w.txt", UtilFileExistsAccess,
               S_IWUSR, false);
      // file exists, but is read-only, so flip sense of validate
      RunTest("FILE_WRITE_ACCESS", "f_exists_r.txt", UtilFileExistsAccess,
               S_IWUSR, true);
      if (single_test) break;
    case FILE_RW:
      RunTest("FILE_RW", "f_exists_rw.txt", FileExistsReadWrite, 0, false);
      // file exists, but is write-only, so flip sense of validate
      RunTest("FILE_RW", "f_exists_w.txt", FileExistsReadWrite, 0, true);
      // file exists, but is read-only, so flip sense of validate
      RunTest("FILE_RW", "f_exists_r.txt", FileExistsReadWrite, 0, true);     
      // dir exists, but is not a regular file, so flip sense of validate
      RunTest("FILE_RW", "dir_exists", FileExistsRead, 0, true);      
      if (single_test) break;
    case STREAM_FMT:
      StreamFormatTest();
      if (single_test) break;
    default:
      if (single_test) {
        UtilFatalErrorAndDie("Unrecognized test Enumerator.");
      }
  }

  if (0) {
    // Test the "run_test.bsh" script's sub-test failure path
    g_scores.Fail("foobert");
    g_scores.Fail("zooey");
    g_scores.Fail("joba");
  }

  std::cout << "\n\n";

  // std::cout <<"Tests Passed:  " << g_scores.passed() << std::endl;
  // std::cout <<"Tests Failed:  " << g_scores.failed() << std::endl;

  // Results for the test(s) that just ran
  std::string label(test);
  label.append(" TEST RESULTS");
  SharedPrintFinalResults(label, g_scores.passed(), g_scores.failed());
  // As above.
  SharedExportPassCount(g_scores.passed());

  std::cout << "\n\n";
  if (g_scores.failed() == 0) {
    std::string message("ALL \"");
    message += (std::string(test) + "\" Tests Passed!\n");
    UtilStdMsg(kCommonStrNote, message);
  } else {
    std::stringstream message;
    message << g_scores.failed() << " " << test
              << " tests failed!!!";
    UtilStdMsg(kCommonStrError, message.str());
  }
  std::cout << std::endl;
  return g_scores.failed();
}
