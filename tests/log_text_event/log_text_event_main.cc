/*****************************************************************************
*
*   DESCRIPTION:
*     main() function for the code to test the LogTextEvent class
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
*   VERSION:  0.00
*   CODER:  Dean Stevens
*   
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
*****************************************************************************/
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "common_strings.hpp"
#include "common_messages.hpp"
#include "log_text_event.hpp"


// Utility function to open a stream, given a "path" argument.
//
// "path" - pathname of the file to which we'll try to open a stream
// Returns - a pointer to the newly opened stream.
std::ofstream *OpenStream(const std::string &path) {
  std::ofstream *new_stream = new std::ofstream;
  new_stream->open(path);
  if (new_stream->is_open()) {
    std::cout << kCommonStrNote << "Opened data output file:  \""
         << path << "\" successfully.\n";
    return new_stream;
  } else {
    std::stringstream line1;
    line1 << "Unable to open data output file:  \"" 
          << path << "\"";
    UtilFatalErrorAndDie(line1.str());
  }
}


// Utility function to compose a log file pathname from constituent
// values.
//
// "test_dir" - pathname to the directory that will hold the log file.
// "test_name" - the name of this test.  Provides the primary content for
//       the leaf name of the path
// "pair_id" - characters that distinguish between file compare pairs
// "extension" - file name extension to use.
// Returns - the newly composed pathname to the log file.  I recognize the
//       overhead of the copy, but this is a one time thing, so...
std::string ComposeLogPath(const std::string &test_dir,
                           const std::string &test_name,
                           const std::string &pair_id,
                           const std::string &extension) {
  std::string log_path;
  log_path.append(test_dir).append(test_name).append(pair_id)
          .append(extension);
  return log_path;
}


int main(int argc, char *argv[]) {

  std::cout << std::endl;

  // Most of these should crash
  char *test;
  if (argc >= 2) {
    // strcpy(argv[1], test);
    test = argv[1];
    std::cout << "Running " << test << " test...\n";
  } else {
    // Need to tell me which test to run
    UtilFatalErrorAndDie("You must specify the name of the test to be run "
                         "as an\nargument on the command line.\nNo test "
                         "specified.");
  }
  // Default path, used for some tests
  std::string test_dir = "./test_out/";
  std::string log_path = test_dir + "outtst.csv";
  // String to distinguish file compare sets from each other, for testing.
  std::string pair_id = "_FL2";
  // we'll use a ".txt" extension for compatibility with existing regression
  // test scripts.
  std::string extension = ".txt";

  if (!strcmp("OPEN_FILE", test)) {
    // Pointer to a log stream
    std::ofstream *log_stream;
    log_stream = OpenStream(log_path);
    if (0) {
      // test the error message...
      log_stream->close();
    }
    if (log_stream->is_open()  && (log_stream->good())) {
      std::cout << "FILE: \"" << log_path << "\" Is Open & OK\n";
      log_stream->close();
    } else {
      std::string message("FILE: \"");
      message += log_path + "\" IS NOT OPEN. It should be!";
      UtilFatalErrorAndDie(message);
    }
  } else if (!strcmp("HEADER", test)) {
    // Try to write the header
    // Instance of the OutputTextEvent output manager object
    log_path = ComposeLogPath(test_dir, test, pair_id, extension);
    LogTextEvent event_log(log_path);
    event_log.WriteHeaderOrDie();
  } else if (!strcmp("RECORD", test)) {
    // A single record written
    // Instance of the OutputTextEvent output manager object
    log_path = ComposeLogPath(test_dir, test, pair_id, extension);
    LogTextEvent event_log(log_path);
    event_log.StageEventTime(23.7);
    event_log.StageEventText("AnEvent");
    event_log.WriteARecordOrDie();
  } else if (!strcmp("FULL_WRITE", test)) {
    // Full Write of the data
    // Instance of the OutputTextEvent output manager object
    log_path = ComposeLogPath(test_dir, test, pair_id, extension);
    LogTextEvent event_log(log_path);
    // The header
    event_log.WriteHeaderOrDie();
    // seed time, base string for payload & suffix stringstream
    SimTime event_time(25.31);
    std::string base_str("payload-");
    std::stringstream suffix;
    // Loop to generate records
    for (int idx=0; idx < 7; idx++) {
      // time for this recod
      event_time.AddTime(idx/10.0);
      // payload for this record
      suffix << event_time.GetUserTime();
      std::string payload = base_str + suffix.str();
      // empty suffix string
      suffix.str("");
      // stage the record, then verify
      event_log.StageEventTime(event_time);
      event_log.StageEventText(payload);
      event_log.WriteARecordOrDie();
    }
  } else if (!strcmp("BAD_PATH", test)) {
    // Try to construct with known bad path
    LogTextEvent event_log("./known_bad/bad_path");
  } else if (!strcmp("HDR_WRT_FAIL", test)) {
    // Writing Header failed
    // Hard to reproduce the case, so just testing message
    UtilFatalErrorAndDie("Failed to write log header.\n"
                         "Output stream returned bad status. "
                         "(LogTextEvent)");
  } else if (!strcmp("HDR_BAD_STREAM", test)) {
    // Stream goes bad after opening
    // open normally.
    LogTextEvent event_log(log_path);
    // Get stream ptr and close it
    std::ofstream *log_stream = event_log.log_stream();
    log_stream->close();
    event_log.WriteHeaderOrDie();
  } else if (!strcmp("TIME_NOT_READY", test)) {
    // Try to write when time data not ready
    // open normally.
    LogTextEvent event_log(log_path);
    //event_log.StageEventTime(23.7);
    event_log.StageEventText("AnEvent");
    event_log.WriteARecordOrDie();
  } else if (!strcmp("TEXT_NOT_READY", test)) {
    // Try to write when text data not ready
    // open normally.
    LogTextEvent event_log(log_path);
    event_log.StageEventTime(23.7);
    //event_log.StageEventText("AnEvent");
    event_log.WriteARecordOrDie();
  } else if (!strcmp("TIME_NOT_READY_VER", test)) {
    // Try to write when time data not ready, catch with verify
    // open normally.
    LogTextEvent event_log(log_path);
    //event_log.StageEventTime(23.7);
    event_log.StageEventText("AnEvent");
    if (!event_log.VerifyStagedReady()) {
      std::cout << test << ": PASSED!\n";
    } else {
      UtilFatalErrorAndDie("LogTextEvent Test: VerifyStagedReady should "
                           "have failed.");
    }
  } else if (!strcmp("TEXT_NOT_READY_VER", test)) {
    // Try to write when text data not ready, catch with verify
    // open normally.
    LogTextEvent event_log(log_path);
    event_log.StageEventTime(23.7);
    //event_log.StageEventText("AnEvent");
    if (!event_log.VerifyStagedReady()) {
      std::cout << test << ": PASSED!\n";
    } else {
      UtilFatalErrorAndDie("LogTextEvent Test: VerifyStagedReady should "
                           "have failed.");
    }
  } else if (!strcmp("REC_WRT_FAIL", test)) {
    // Writing record failed
    // Hard to reproduce the case, so just testing message
    // Log output class tests
    UtilFatalErrorAndDie("Unable to write log record.\n"
                         "Output stream returned bad status. "
                         "(LogTextEvent)");
  } else if (!strcmp("REC_BAD_STREAM", test)) {
    // Stream goes bad after opening
    // open normally.
    LogTextEvent event_log(log_path);
    // Get stream ptr and close it
    std::ofstream *log_stream = event_log.log_stream();
    // Stage the data
    event_log.StageEventTime(23.7);
    event_log.StageEventText("AnEvent");
    log_stream->close();
    if (event_log.VerifyStagedReady()) {
      event_log.WriteARecordOrDie();
    } else {
      UtilFatalErrorAndDie("LogTextEvent Test: Events not properly staged "
                           "for write.");
    }
  } else if (!strcmp("BAD_STREAM", test)) {
    // Log output class tests
    UtilFatalErrorAndDie("Unable to write output record.\n"
                         "Output stream either not open, or returned bad "
                         "status.");
  } else if (!strcmp("TIME_OVER", test)) {
    // Time overflow test, using stringstream to create one of the lines
    std::stringstream line1;
    long double add_time = 2.7238E14;
    long double to_time = 3.14162E11;
    line1 << "Adding " << add_time << " to " << to_time
          << " would exceed\npermissible range.";
    UtilFatalErrorAndDie(line1.str());
  } else {
    // Unrecognized argument
    std::stringstream line1;
    line1 << "\"" << test << "\" Not Recognized!!!\n"
          << "No tests will be run.";
    UtilFatalErrorAndDie(line1.str());
  }
  
  return 1;
}
