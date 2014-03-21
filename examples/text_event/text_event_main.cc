/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the initialization and main() function for the
*     text_event program.  text_event.exe is a sample program that
*     demonstrates an extremely simple implementation of the Discrete Event
*     Simulation engine.  This implementation is based on a fairly basic
*     TextEvent object.  The source code might provide a starting template
*     for building more useful simulators.
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
#include <iomanip>
#include <string>

#include "basic_defs.hpp"
#include "common_strings.hpp"
#include "common_messages.hpp"
#include "common_utilities.hpp"
#include "sim_time.hpp"
#include "arg_parser.hpp"
#include "sim_exec.hpp"
#include "sim_text_event.hpp"
#include "stim_text_event_loader.hpp"
#include "log_text_event.hpp"
#include "display_help.hpp"
#include "sim_version.hpp"

// Display the copyright notice on cout.
void CopyrightLicenseNotice() {
  std::cout << kCommonCopyright << std::endl;
  std::cout << kCommonLicense << std::endl;
}

// Description for the help
const std::string help_descr = 
    "    text_event.exe is a sample program that demonstrates an extremely\n"
    "    simple implementation of the Discrete Event Simulation engine.\n"
    "    This implementation is based on a fairly basic TextEvent object.\n";


// Handle setup for the simulation run.  Specific responsibilities include:
// (1) Display welcome, copyright, license, etc. text.  
// (2) Parse commandline arguments
// (3) Initialize stimulus loader
// (4) Initialize the simulation executive.
// Once this function completes successfully, the simulation should be 
// ready to run
// Exits with EXIT_FAILURE status if problems with the argument / stimulus
// loading are encountered.
void InitSessionOrDie(long argc, char * argv[]) {

  std::cout << "\n********************************************"
               "\n***  Welcome to the Text Event Example!  ***"
               "\n********************************************"
            << std::endl;

  SimVersion version;
  version.PrintVersion("Executing:");
  std::cout << std::endl;

  CopyrightLicenseNotice();

  // Process the user specified arguments
  ArgParser the_args;
  if (the_args.ParseArgs(argc, argv)) {
    // The arguments appear OK, see if they specified the HELP switch
    if (the_args.parsed_args().display_help_) {
      DisplayHelp("text_event.exe", help_descr);
      // Since they asked for help, we'll disregard other args
      exit(EXIT_SUCCESS);
    }
  } else {
    // A problem was encountered in arg processing.  Proceeding beyond this
    // point might have unexpected results.
    UtilFatalErrorAndDie("Unrecognized, or improper argument(s).\n"
                         "Use -h for help");
  }
 
  // Create the stimulus loader for StimTextEvent(s).  The stimulus loader
  // is specific to each type of stimulus file, so it can't be generically
  // created in the exec's Init() method.
  //
  // We can't run the simulation without stimulus, so the code will issue
  // a fatal error and exit with failure status if it encounters any
  // issues with the specified file.  This may happen in this function, if
  // the file simpy doesn't exist, or the user doesn't have the proper 
  // access to it.  The issue may be encountered by the constructor, if the
  // file does not appear to be a proper stimulus file.
  StimTextEventLoader *stim_text_event_loader;
  if (UtilFileExistsRead(the_args.parsed_args().stimulus_path_)) {
    // The stimulus file exists, and appears to be readable, let's try
    // to crack it open.  This call attempts to open, and validate, the
    // specified stimulus file.
    stim_text_event_loader = new 
                 StimTextEventLoader(the_args.parsed_args().stimulus_path_);
  } else {
    std::string message = "The specified Stimulus File: \"" + 
                          the_args.parsed_args().stimulus_path_ + "\" ";
    std::string post_msg = "\nSimulation requires a valid stimulus file to "
                           "execute.\nPlease check the Stimulus File "
                           "pathname and try again.";
    if (UtilFileExists(the_args.parsed_args().stimulus_path_)) {
      // The file exists, but it's either not a regular file, or the user
      // doesn't have read access to it.
      message += "exists.\nHowever, either you do not have read access to "
                 "the file,\nor the pathname doesn't specify a regular "
                 "file (perhaps\nit identifies a directory).";
    } else {
      // The file doesn't exist.
      message += "could not be found.";
    }
    message += post_msg;
    UtilFatalErrorAndDie(message);
  }

  // Create the log file manager.  The log format and contents may be
  // specific to each simulation implementation, so the specific
  // "log_manger" is created here, and passed to the executive.
  LogTextEvent *log_manager;
  if ((!UtilFileExists(the_args.parsed_args().log_path_)) ||
      UtilFileExistsWrite(the_args.parsed_args().log_path_)) {
    // The specified log file doesn't exist, or it exists, and the user
    // has write access to it.
    // The constructor will issue a fatal error and exit with failure status
    // if it encouters ay issues with the specified file.  This decision
    // was based on the assumption that, if the developer wrote logging
    // code for the simulation, they would require a valid log file.
    // Otherwise, all logging code would always have to check for the
    // existence of a valid log manager before each write.
    // If there is no logging code, developers can specify "nullptr" to
    // the "SimExec::Init()" method.
    log_manager = new LogTextEvent(the_args.parsed_args().log_path_);
  } else {
    std::string message;
    if (UtilFileExists(the_args.parsed_args().log_path_)) {
      // The log file exists, so there must be a problem with the user's
      // access.
      message = "The specified Log File: \"" + 
                the_args.parsed_args().log_path_ + "\" exists.\n"
                "However, you do not seem to have write access to the file.";
    } else {
      // Probably an issue with the UtilFileExists* code.  If the specified
      // log file doesn't exist, the code above should have tried to call
      // the ctor.
      message = "Unexpected issue with the Log File pathname:\n  \"" +
                the_args.parsed_args().log_path_ + "\"";
    }
    UtilFatalErrorAndDie(message);
  }

  // Initialize the simulation executive with the various run parameters
  // and file managers.  Once the initialization is complete, the simulation
  // is ready to run.
  SimExec::the_exec()->Init(SimTime(the_args.parsed_args().run_until_time_),
                            nullptr,
                            log_manager,
                            stim_text_event_loader);
}  // initSession

int main(int argc, char *argv[]) {
  InitSessionOrDie(argc, argv);
  // Run the discrete event simulation engine
  SimTime sim_run = SimExec::the_exec()->Run();

  std::cout << "\n=>=>=>=>=>=>=>>> Simulation Complete at time " 
            << sim_run.GetUserTime() << " <<<=<=<=<=<=<=<=\n";
  // Clean up the simulation executive.  This handles cleaning up the storage
  // and, as appropriate, streams that were passed to the exec's "Init"
  // method.
  SimExec::the_exec()->TearDown();

  return EXIT_SUCCESS;
}  // main
