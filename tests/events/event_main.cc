/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the test scaffolding for the event classes.  In
*     brief, this provides a main() and code to exercise the event class
*     methods.
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

#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include "common_strings.hpp"
#include "sim_text_event.hpp"
#include "shared_test_code.hpp"

void CopyrightNotice() {
  std::cout << kCommonCopyright << std::endl <<std::endl;
}

void InitSession(long argc, char * argv[]) {

  std::cout << "\n*******************************************"
               "\n***  Welcome to the Event Object Test!  ***"
               "\n*******************************************"
            << std::endl;

  CopyrightNotice();
}

int main(int argc, char *argv[]) {
  InitSession(argc, argv);
  // Results counters
  int passed = 0;
  int failed = 0;

  // Initialize a couple of time objects
  SimTime time_1(17.3);
  SimTime time_2(27.4);

  // Initialize some text event objects, the first two rely on the automatic
  // conversion from usertime to a time event (see the declaration of
  // SimTime).  The last one initializes with one of the time objects
  // defined above
  SimTextEvent text_ev1(17.3,"Event1");
  SimTextEvent text_ev2(27.4,"Event2");
  SimTextEvent text_ev3(time_1,"Event3");

  text_ev1.DumpEvent();
  text_ev2.DumpEvent();
  text_ev3.DumpEvent();

  int indent = 3;

  // Check the initializations:
  std::cout << "Initializing with auto time conversion vs explicit time:";
  if (text_ev1.event_time().ticks() == text_ev3.event_time().ticks()) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  } 

  // Earlier - 2 Events
  std::cout << "Earlier Than w/ 2 events:";
  if (text_ev1.EarlierThan(text_ev2)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "Earlier Than w/ 2 events(reversed):";
  if (!text_ev2.EarlierThan(text_ev1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  // Earlier - Event & Time
  std::cout << "Earlier Than w/ event & time:";
  if (text_ev1.EarlierThan(time_2)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }

  // AsEarlyAs - 2 Events
  std::cout << "AsEarlyAs w/ 2 events, 1st Earlier:";
  if (text_ev1.AsEarlyAs(text_ev2)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "AsEarlyAs w/ 2 events, 2nd Earlier:";
  if (!text_ev2.AsEarlyAs(text_ev1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "AsEarlyAs w/ 2 events, SameTime:";
  if (text_ev1.AsEarlyAs(text_ev3)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "AsEarlyAs w/ 2 events, SameTime (reversed):";
  if (text_ev3.AsEarlyAs(text_ev1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  // AsEarlyAs - Event & Time
  std::cout << "AsEarlyAs w/ event & time, Event Earlier:";
  if (text_ev1.AsEarlyAs(time_2)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "NOT AsEarlyAs w/ event & time, Time Earlier:";
  if (!text_ev2.AsEarlyAs(time_1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "AsEarlyAs w/ event & time, Same Times:";
  if (text_ev1.AsEarlyAs(time_1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }

  // SameTimeAs - 2 Events
  std::cout << "SameTimeAs  w/ 2 events:";
  if (text_ev1.SameTimeAs(text_ev3)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "SameTimeAs  w/ 2 events(reversed):";
  if (text_ev3.SameTimeAs(text_ev1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  // NOT SameTimeAs - 2 Events
  std::cout << "NOT SameTimeAs  w/ 2 events:";
  if (!text_ev1.SameTimeAs(text_ev2)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  std::cout << "NOT SameTimeAs w/ 2 events(reversed):";
  if (!text_ev2.SameTimeAs(text_ev1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  // SameTimeAs - Event & Time
  std::cout << "SameTimeAs w/ event & time:";
  if (text_ev1.SameTimeAs(time_1)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }
  // NOT SameTimeAs - Event & Time
  std::cout << "NOT SameTimeAs w/ event & time:";
  if (!text_ev1.SameTimeAs(time_2)) {
    std::cout << std::setw(indent) << ' ' << "Passed!\n";
    passed++;
  } else {
    std::cout << "!!! FAILED !!!\n";
    failed++;
  }

  std::cout << "\n\n";
  // Results for the test(s) that just ran
  std::string label("AUTOMATIC EVENT TEST RESULTS");
  SharedPrintFinalResults(label, passed, failed);
  // As above.
  SharedExportPassCount(passed);
  
  // Summary
  if (failed == 0) {
    std::cout << "\nSUCCESSS!\n\n";
  } else {
    std::cout << "\n!!! FAILURE !!!\n\n";
  }
  
  return failed;
}  // main
