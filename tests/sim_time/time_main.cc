/*****************************************************************************
*
*   DESCRIPTION:
*     main() function for the SimTime test code.
*     
*     There are two distinct modes for the test.
*     
*     One mode is in operation when the executable is run with no arguments.
*     This component makes up the bulk of the code here. It tests various
*     operations on the SimTime object and programatically checks for
*     expected behavior.  A count is kept of passed/failed as each test is
*     performed.  In this mode, the executable returns the count of failed
*     tests upon completion of execution.  As would be expected, 0 is
*     is returned if all tests were successful.
*     
*     The other mode is in operation when the executable is invoked with
*     an appropriate argument.  In this mode, the tests are designed to
*     validate fatal error conditions, like overflowing the time object's
*     capacity.  These tests expect to terminate the program, and results
*     are checked with an external test script.
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

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <limits>
#include <stdio.h>
#include <math.h>

#include "sim_time.hpp"
#include "common_strings.hpp"
#include "common_messages.hpp"
#include "shared_test_code.hpp"


// Number of ticks in each unit of user time.  Global so that it's available
// to all functions in module
SimTime::UserTime g_tpu = 0.0;


void initSession(long ac, char* av[]) {
	for (int i=0;i<ac;i++) {
		printf("%d - %s\n", i, av[i]);
	}
}

class TstCls {
 public:
  TstCls() : my_time_(0.0) {
  }
  // Accessor
  //
  // Returns - time object representing this object's time
  SimTime get_my_time() const;
  
  // Mutator
  //
  // "to_time" "UserTime" variable used to set this object's time
  void set_my_time(double const to_time);
 private:
  SimTime my_time_;
};

// Accessor
//
// Returns - time object representing this object's time
SimTime TstCls::get_my_time() const {
  return my_time_;
}

// Mutator
//
// "to_time" "UserTime" variable used to set this object's time
void TstCls::set_my_time(double const to_time) {
  my_time_.SetTime(to_time);
}


// Runs a test based on the commandline argument
//
// "test" - name of the test to run.
void SingleTest(char *test) {
  // Will be initialized to 0
  SimTime stime;
  SimTime::UserTime too_big = SimTime::GetMaxUserTime() * 2.0;
  if (!strcmp("OVERFLOW_ON_SET", test)) {
    // CRASHES...
    std::cout << "Assign time that overflows ticks...\n";
    stime.SetTime(too_big);
  } else if (!strcmp("OVERFLOW_ON_ADD", test)) {
    std::cout << "Try adding UserTime to overflow...\n";
    stime.SetTime(SimTime::GetMaxUserTime());
    stime.AddTime(SimTime::GetMaxUserTime());
  } else if (!strcmp("OVERFLOW_ON_ADD_SIMTIME", test)) {
    std::cout << "Try adding SimTime to overflow...\n";
    stime.SetTime(SimTime::GetMaxUserTime());
    SimTime stime2(SimTime::GetMaxUserTime());
    stime.AddTime(stime2);
  } else if (!strcmp("MAX_TIME_STR", test)) {
    std::cout << "Display a string with MaxUserTime expanded...\n";
    std::string max_time = SimTime::GetMaxUserTimeLongString();
    std::cout << max_time << std::endl;
    exit(EXIT_SUCCESS);
  } else {
    std::cerr << "!!! Single Test: \"" << test << "\" Not Recognized!!!\n";
  }
}

int main(int argc, char *argv[]) {
  // Initialize the variable to avoid function calls.
  g_tpu = SimTime::GetTicksPerUserTimeUnit();
  unsigned int passed = 0;
  unsigned int failed = 0;

  if (argc > 1) {
    // With any arguments, see if we should run the "SingleTest" checks.
    // These tests should cause this program to exit with either a fail
    // status, for the fatal error tests, or a success status for the
    // other tests.
    // It's a test failure if the function call returns to main().
    SingleTest(argv[1]);
    // no args, 
    // OK, that's bad.  Shouldn't have returned from a Fatal Error check
    std::cerr << "SingleTest Check \"" << argv[1] << "\" FAILED !!! - "
                 "Actual test should have exited!\n";
    exit(EXIT_FAILURE);
  }
  // NO args, run standard checks.
  SimTime stime(100.0);
  std::cout << "Ctor to 100.0; Time is: " << stime.GetUserTime() << std::endl;
  if (stime.ticks() == (100 * g_tpu)) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  SimTime stime2(150.0);
  SimTime stime2same(150.0);
  if (stime2.ticks() == (150 * g_tpu)) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  // TIME COMPARISONS
  std::cout << "Time1 is: " << stime.GetUserTime() << " - Time2 is: "
            << stime2.GetUserTime() << std::endl;
  if (stime.EarlierThan(stime2)) {
    std::cout << "Time1 is Earlier than Time2" << std::endl;
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "Time1 is Later than Time2" << std::endl;
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  if (stime.AsEarlyAs(stime2)) {
    std::cout << "Time1 is As Early As than Time2" << std::endl;
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "Time1 is Later than Time2" << std::endl;
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  if (!stime2.EarlierThan(stime)) {
    std::cout << "Time2 is NOT Earlier than Time1" << std::endl;
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "Time2 is Earlier than Time1" << std::endl;
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  if (!stime2.AsEarlyAs(stime)) {
    std::cout << "Time2 is NOT As Early As Time1" << std::endl;
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "Time2 is As Early As Time1" << std::endl;
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  if (!stime2.SameAs(stime)) {
    std::cout << "Time2 is NOT the Same As Time1" << std::endl;
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "Time2 is the Same As Time1" << std::endl;
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  if (stime2.SameAs(stime2same)) {
    std::cout << "Time2 is the SAME As Time2same" << std::endl;
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "Time2 is NOT the Same As Time2same" << std::endl;
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }

  // ADDITION
  stime.AddTime(stime2);
  std::cout << "Time (after adding 150.0 to 100.0) is now: "
  << stime.GetUserTime() << std::endl;
  if (stime.ticks() == (250 * g_tpu)) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  stime.AddTime(37.53);
  std::cout << "Time, after adding 37.53 seconds is: " << stime.GetUserTime() << std::endl;
  SimTime::SimTick targ = 250 * g_tpu + static_cast<SimTime::SimTick>(37.53L * g_tpu);
  if (stime.ticks() == targ) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  stime.SetTime(12.43);
  std::cout << "Time, after setting to 12.43 seconds is: " << stime.GetUserTime() << std::endl;
  if (stime.ticks() == (static_cast<SimTime::SimTick>(12.43L * g_tpu))) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  stime.SetTime(stime2);
  std::cout << "Time, after setting to stime2 (150.0): " << stime.GetUserTime() << std::endl;
  if (stime.ticks() == (150 * g_tpu)) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  std::cout << "\n\nBoundary Tests...\n";
  std::cout << "Try to set to negative time...\n";
  stime.SetTime(-200.0);
  std::cout << "Time after attempting to set to -200.0: " << stime.GetUserTime() << std::endl;
  if (stime.ticks() == 0L) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  std::cout << "Assign time that just fits in ticks...\n";
  SimTime::SimTick max_ticks = std::numeric_limits<SimTime::SimTick>::max();
  SimTime::UserTime too_big = static_cast<SimTime::UserTime>(max_ticks/100.0L);
  stime.SetTime(too_big);
  
  if (stime.ticks() == max_ticks) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  std::cout << "In Main Max Ticks: " << max_ticks << std::endl;

  std::cout << "\n\nTesting a class with a SimTime data member...\n";
  TstCls mtc;
  mtc.set_my_time(0.01);
  SimTime itm = mtc.get_my_time();
  SimTime::SimTick rval = llround(0.01 * g_tpu);
  SimTime::UserTime tval = itm.GetUserTime();
  std::cout << "Tval is set to: " << tval << std::endl;
  if (itm.ticks() == rval) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }

  std::cout << "\n\nTesting the assignment operator...\n";
  SimTime jtm;
  itm.SetTime(0.02);
  jtm = itm;
  if (jtm.ticks() == rval*2) {
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  
  // CHAINED EXPRESSIONS
  std::cout << "\n\nTesting expression chaining...\n";
  TstCls *tc_p = new TstCls;
  tc_p->set_my_time(17.3);
  if (tc_p->get_my_time().EarlierThan(stime2same)) {
    std::cout << tc_p->get_my_time().GetUserTime() << " is earlier than " 
              << stime2same.GetUserTime() << std::endl;
    std::cout << "=== PASSED!\n\n";
    passed++;
  } else {
    std::cout << tc_p->get_my_time().GetUserTime() << " is NOT earlier than "
              << stime2same.GetUserTime() << std::endl;
    std::cout << "************\n*** FAILED !!!\n************\n\n";
    failed++;
  }
  
  // Results for the tests implemented in code that don't crash.
  SharedPrintFinalResults("STANDARD TEST RESULTS", passed, failed);
  // As above.
  SharedExportPassCount(passed);

  return failed;
}  // main
