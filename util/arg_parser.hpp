/*****************************************************************************
*
*   DESCRIPTION:
*     Header file describing the classes that handle command line argument
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
*   STATUS:  Prototype
*   VERSION:  0.00
*   CODER:  Dean Stevens
*
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
*****************************************************************************/

#ifndef SIM_UTIL_ARG_PARSER_HPP_
#define SIM_UTIL_ARG_PARSER_HPP_

#include <string>

#include "basic_defs.hpp"
#include "sim_time.hpp"

// Contains the current values for the run-time parameters to this program.
// Initially, the arguments are assigned default values.  These may be
// changed by the results of parsing the command line arguments
class ParsedArgs {
 public:
  // Sets default values for each data member
  ParsedArgs();
  ~ParsedArgs();

#ifdef TEST_HARNESS
  // Dump the member data for debugging.
  void Dump();
#endif
  std::string config_path_;
  std::string log_path_;
  std::string stimulus_path_;
  SimTime::UserTime run_until_time_;
  bool display_help_;
 private:
  // As per the coding standard
//DISALLOW_COPY_AND_ASSIGN(ParsedArgs);
};

class ArgParser {
 public:
  ArgParser() { };
  ~ArgParser() { };
  // Iterates through the argument vector, parses out the arguments and
  // validates them as being appropriate.
  // Caches the parsed arguments in the form of the parsed_args_ data
  // member of this object.
  //
  // "argc" - number of command-line arguments
  // "argv" - the command line arguments
  // Returns - "true" if no errors were encountered, "false" otherwise
  virtual bool ParseArgs(long argc, char *argv[]);

  // Get a reference to the cached ParsedArgs object.  Breaks encapsulation,
  // but much easier maintenance & simpler code.  There are always trade-offs
  //
  // Returns - a reference to the parsed_args_ object
  ParsedArgs &parsed_args() { return parsed_args_; };

  // Validate that "eval_time" is within the valid range for the simulation
  // environment.  The text to print in the error message is held in "name".
  //
  // Can be safely used by external callers to range check a UserTime
  // value.  There are no side effects that would impact the ArgParser
  // object.
  //
  // "eval_time" - the time value to check
  // "name" - user recognizable name for the time value. Used in composing
  //       error messages.
  // Returns - "true" if the time is within range, "false" otherwise.
  virtual bool ValidateTime(SimTime::UserTime eval_time,
                            const std::string &name);

 protected:
  // Compares the argument string "arg_string" with a string containing a 
  // value to test against "test_string".
  //
  // Returns - "true" if they match.
  virtual bool CheckArg(std::string arg_string,
                        const std::string &test_string);

  // Verify that the argument "arg_string" does, indeed, include a number,
  // and that the number is in the correct range.  Generates an error message
  // in the event that it fails to recognize a valid number.
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
  virtual bool VerifyTimeString(const char *arg_string,
                                const std::string &name,
                                SimTime::UserTime *time);

  // Issues an error message when the argument appears to be malformed.
  // This method is a utility method to issue the message.  It does no
  // additional checking.
  //
  // "arg_string" - string containing the argument that appears to be
  //       malformed.  Used as a component of the error message.
  // Returns - "false", which could be used to set error flags in the
  //       caller.
  virtual bool ArgError(const std::string &arg_string);

 private:
  // The current values for the arguments.  Initialized to defaults, but
  // possibly update as a result of parsing / config file processing.
  ParsedArgs parsed_args_;

  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(ArgParser);  
}; // ArgParser

#endif  // SIM_UTIL_ARG_PARSER_HPP_

