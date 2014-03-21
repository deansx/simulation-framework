/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the test scaffolding for the stim_loader class.  In
*     brief, this provides a main() and code to exercise the stim_loader.
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
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "basic_defs.hpp"
#include "common_strings.hpp"
#include "common_messages.hpp"
#include "sim_time.hpp"
#include "stim_loader.hpp"
#include "sim_exec.hpp"
#include "sim_text_event.hpp"
#include "stim_text_event_loader.hpp"
#include "arg_parser.hpp"
#include "log_text_event.hpp"



void CopyrightNotice()
{
  std::cout << kCommonCopyright << std::endl;
}

void InitSession(long argc, char * argv[]) {

  std::cout << "\n********************************************"
               "\n***  Welcome to the Stimulus Load Test!  ***"
               "\n********************************************\n"
            << std::endl;

  // Would invalidate the compare, when the copyright date changes...
  // CopyrightNotice();

  std::string setup_path = "./setup.txt";
  std::string stimulus_path = "./test_ref/stim.csv";
  std::string log_path = "./test_out/STIM_LOAD_FL2.txt"; 
  SimTime::UserTime run_until_time = 1.0E6;
  const SimTime::UserTime kDefaultRunUntilTime = 1.0E5;

  // Stream to build message.
  std::stringstream message;
  // We'll use an instance of "ArgParser" to validate the "run_until_time"
  ArgParser arg_parser;
  if (arg_parser.ValidateTime(run_until_time, "Run Until Time")) {
    message << "Simulation \"Run until time\" set to "
            << run_until_time << " time units.\n";
    UtilStdMsg(kCommonStrNote, message.str());
  } else {
    run_until_time = kDefaultRunUntilTime;
    message << "Simulation will use the default \"Run Until Time\" of "
            << kDefaultRunUntilTime << " units.";
    UtilStdMsg(kCommonStrWarn, message.str());
  }

  // The stimulus loader is specific to each type of stimulus file, so it
  // can't be generically created in the exec's Init() method.
  StimTextEventLoader *stim_text_event_loader = new StimTextEventLoader(
                                                              stimulus_path);
  // Set up the log manager
  LogTextEvent *log_mgr = new LogTextEvent(log_path);
  // Write the header row
  log_mgr->WriteHeaderOrDie();

  // Initialize the simulation executive
  SimExec::the_exec()->Init(SimTime(run_until_time), nullptr, log_mgr,
                            stim_text_event_loader);
}  // initSession

int main(int argc, char *argv[])
{
  InitSession(argc, argv);
  // Run the discrete event simulation engine
  SimTime sim_run = SimExec::the_exec()->Run();

  std::cout << "\n=>=>=>=>=>=>=>>> Simulation Complete <<<=<=<=<=<=<=<=\n";

  // Will close the streams and clean up the storage for the stimulus loader
  // and log manager. 
  SimExec::the_exec()->TearDown();

  return EXIT_SUCCESS;
}  // main
