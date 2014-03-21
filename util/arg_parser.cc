/*****************************************************************************
*
*   DESCRIPTION:
*     Source file defining the classes that handle command line argument
*     parsing and configuration file processing.
*
*     The two classes are:
*     
*     ArgParser - main class that handles parsing the arguments and processing
*           the configuration file.  The possible arguments / configuration
*           parameters are initialized to default values that might be
*           overwritten by the user's specification.
*     ParsedArgs - class that actually bundles up the parameters as data
*           members.  This class is also responsible for establishing the
*           default values.
*     
*     NOTE: IF THE ARGUMENTS CHANGE, display_help.cc MAY NEED TO CHANGE
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

#include <stdint.h>
#include <cstdlib>
#include <limits>
#include <cstring>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "common_strings.hpp"
#include "common_messages.hpp"
#include "arg_parser.hpp"


// Constructor simply assigns the default values for each data member.  These
// may be overwritten by user specifications during later processing.
ParsedArgs::ParsedArgs() {
  config_path_ = "./setup.txt";
  log_path_ = "./logfile.csv";
  stimulus_path_ = "./stim.csv";
  run_until_time_ = 1.0E5;
  display_help_ = false;
}


// Currently, no work for the dtor
ParsedArgs::~ParsedArgs() {
}


#ifdef TEST_HARNESS
// Dump the member data for debugging.
void ParsedArgs::Dump() {
  std::string show_help;
  if (display_help_) {
    show_help = "YES";
  } else {
    show_help = "NO";
  }
  std::cout << "\nConfig Path:  " << config_path_
            << "\nStimul Path:  " << stimulus_path_
            << "\nLog    Path:  " << log_path_
            << "\nRun Until:    " << run_until_time_
            << "\nShow Help:    " << show_help
            << std::endl;
}
#endif


// Compares the argument string with a string containing a value to
// test against.
//
// Returns - "true" if they match.
bool ArgParser::CheckArg(std::string arg_string, 
                         const std::string &test_string) {
  // arg_str was passed by value, so it can be modified in this function
  // without affecting the caller's value.  Convert it to all uppercase
  std::transform(arg_string.begin(), arg_string.end(), 
                 arg_string.begin(), toupper);

  // return !arg_string.compare(test_string);
  return (arg_string == test_string);
}


// Validate that "eval_time" is within the valid range for the simulation
// environment.  Generates an error message if the time
// is out of range.
//
// "eval_time" - the time value to check
// "name" - user recognizable name for the time value. Used in composing
//       error messages.
// Returns - "true" if the time is within range, "false" otherwise.
bool ArgParser::ValidateTime(SimTime::UserTime eval_time,
                             const std::string &name) {
  // NOTE: IF THE ARGUMENTS CHANGE, display_help.cc MAY NEED TO CHANGE
  //       AS WELL.
  bool time_ok = false;
  static const SimTime::UserTime kMaxUserTime = SimTime::GetMaxUserTime();
  if ((eval_time > 0.0L) && (eval_time <= kMaxUserTime)) {
    // Time must be a positive number that is less than the maximum
    // value set for this platform.
    // stime.GetMaxUserTime() >= the_args.run_until_time_)
    // Specified time within valid range
    time_ok = true;
  } else {
    // Specified run_until_time outside the valid range
    // "to_string()" isn't available in a number of gcc implementations,
    // so using stringstream
    std::stringstream message;
    message << "The time \"" << name << "\" of " << eval_time
            << " units is outside the supported\nrange.  \"" << name
            << "\" must be greater than 0.0 and less than,\nor equal to, "
            << SimTime::GetMaxUserTimeLongString() << " units.";
    UtilStdMsg(kCommonStrError, message.str());
  }  // run_until_time range check
  return time_ok;
}  // ValidateTime


// Verify that "arg_string" does, indeed, include a number, and that
// the number is in the correct range.  Generates an error message in the
// event that the method fails to recognize a valid number.
//
// "arg_string" - string that should include a number that is within the
//       expected range for a UserTime value
// "name" - user recognizable name for the time value. Used in composing
//       error messages.
// "time" - receives the newly recognized time value, if the "arg_string"
//       represented a valid time
// Returns - "true" if the verification succeeds, "false" otherwise.
//         - the extracted number in the "time" parameter, if the
//           conversion succeeds.  The "time" parameter is unchanged
//           if the conversion failed.
bool ArgParser::VerifyTimeString(const char *arg_string,
                                 const std::string &name,
                                 SimTime::UserTime *time) {
  // initialize value to be returned
  bool return_val = false;
  // Attempt to convert the string into a number.
  // Note:  Neither "stold()", nor "strtold" are available with my
  // not-quite up-to-date development environment, so using "strtod()"
  // instead.  Code might have been something like:
  // "temp_time = std::stold(arg_string);"
  char *end_ptr;
  SimTime::UserTime temp_time = 
      static_cast<SimTime::UserTime>(strtod(arg_string, &end_ptr));
  if (*end_ptr == 0) {
    // "strtod()" sets "*end_ptr" to 0 if it recognizes exactly one number
    // in the string.
    // Let's see if the number represents a valid time, in simulator terms
    if (ValidateTime(temp_time, name)) {
      *time = temp_time;
      return_val = true;
    } else {
      // ValidateTime() has already generated an error message.  Since the
      // conversion failed, no change is made to the time parameter
      return_val = false;
    }
  } else {
    // We are not interested in strings containing multiple distinct number
    // sequences (example: "3.24 9.53"), nor strings with mixed digit and
    // (non-digit)characters (example: "3x").
    // Therefore, we don't need to worry about the case where "strtod()" has
    // extracted a valid number, but could continue processing the string to
    // try to extract another number.  In these cases, "*end_ptr" would be
    // non-zero, but "temp_time" would contain a technically valid number.
    // However, for this test, we consider such an original string to not
    // represent a useful number.
    return_val = false;
    std::stringstream message;
    message << "\"" << arg_string << "\" does not appear to represent a "
            << "valid value for \"" << name << "\".\nThe value must be a "
            << "number that is greater than 0.0 and less than,\nor equal to, "
            << SimTime::GetMaxUserTimeLongString() << " units.";
    UtilStdMsg(kCommonStrError, message.str());
  }
  return return_val;
}  // VerifyTimeString

// Issues an error message on cerr when the argument appears to be
// malformed.  This method is a utility method to issue the message.
// It does no additional checking.
//
// "arg_string" - string containing the argument that appears to be
//       malformed.  Used as a component of the error message.
// Returns - "false", which could be used to set error flags in the
//       caller.
bool ArgParser::ArgError(const std::string &arg_string) {
  std::string message("Unrecognized, or invalid, argument:  \"");
  message.append(arg_string).append("\".\n");
  UtilStdMsg(kCommonStrError, message);
  return false;
}

// Iterate through the argument vector, parse out the arguments and validate
// them as appropriate.
// Note: Specifying a valid switch, without an accompanying field argument,
// (for example, just "-O") will not be flagged as an error.  However, the
// default value will not be overwritten.
// Caches the parsed arguments in the form of the parsed_args_ data
// member of this object.  User specified parameters overwrite the defaults
//
// "argc" - number of command-line arguments
// "argv" - the command line arguments
// Returns - "true" if no errors were encountered, "false" otherwise
bool ArgParser::ParseArgs(long argc, char *argv[]) {
  // Records whether any bad arguments were parsed.
  bool good_args = true;
  if (argc < 2) {
    // No arguments to the command were specified.
    return good_args;
  }
  while (--argc) {
    if (**(++argv) == '-') {
      // This is a switch

      // Initialize the flag we'll use if we encounter an unrecognized / invalid
      // argument.
      bool bad_arg = false;
      // The next two definitions represent a trade off towards managing the
      // th scope of the variables over the penalty of creating them for each
      // pass through the loop.

      // Capture the argument string before any case transforms.  We may use
      // this unadulterated form for the an error message.
      std::string err_str = *argv;
      // Will be sacrificed to case transformation when matching full words, 
      // as opposed to just switches.
      std::string compare_str;

      // Convert the character just beyond the '-' to uppercasse and use it
      // for the switch statement.  Within each case, the value for the
      // switch is extracted, if appropriate.  Often, this is accomplished
      // by extracting the "-*", where '*' represents the flag and using
      // the remaining substring.
      switch (*(*argv + 1) = toupper(*(*argv + 1))) {
        case 'C':
          // Configuration file name
          if (0 != *(*argv + 2))
            parsed_args_.config_path_ = (*argv + 2);
          break;
        case 'H':
          // Display the help file
          // Only -[H|h] is required, but we'll accept the full word, in any
          // case combination, so we do a bit of extra checking.
          if (strlen(*argv) == 2) {
            // The switch statement has already determined that the argument is
            // "-[Hh] so set display help to on
            parsed_args_.display_help_ = true;
          } else if (strlen(*argv) == 5 && CheckArg(*argv, "-HELP")) {
            // The argument string is some variation of "-HELP"
            parsed_args_.display_help_ = true;
          } else {
            // the -[Hh] argument doesn't match a request for the help.
            bad_arg = true;
          }
          break;
        case 'L':
          // Output file name
          if (0 != *(*argv + 2))
            parsed_args_.log_path_ = (*argv + 2);
          break;
        case 'S':
          // Stimulus file name
          if (0 != *(*argv + 2))
            parsed_args_.stimulus_path_ = (*argv + 2);
          break;
        case 'T':
          // Run Until Time in user units
          if (0 != *(*argv + 2)) {
            //std::cout << "Time arg is:  " << *argv << std::endl;
            if ((strlen(*argv) == 5) && CheckArg(*argv, "-TMAX")) {
              // The user might have told us to just use the maximum time
              // allowed to run on this platform.
              parsed_args_.run_until_time_ = SimTime::GetMaxUserTime();
            } else {
              // Verify that this argument does, indeed, include a number,
              // and that the number is in the correct range.
              if (!VerifyTimeString(*argv + 2, "RunUntilTime",
                                    &parsed_args_.run_until_time_)) {
                // Couldn't verify the argument
                bad_arg = true;
                std::stringstream message;
                message << "Unable to use \"" << (*argv + 2)
                        << "\" as RunUntilTime parameter.\nUsing, the "
                        << "default value \"" << parsed_args_.run_until_time_
                        << "\" instead.";
                UtilStdMsg(kCommonStrWarn, message.str());
              }
            }
          }
          break;
        default:
          bad_arg = true;
          break;
      }  // end switch
      if (bad_arg) {
        //std::cout << "switch, but bad parse: ";
        good_args = ArgError(err_str);
      }
    } else {
      //std::cout << "not aswitch: ";
      good_args = ArgError(*argv);
    }  // end if '-'
  }  // end while
  return good_args;
}  // ParseArgs
