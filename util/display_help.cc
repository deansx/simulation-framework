/*****************************************************************************
*
*   DESCRIPTION:
*     File containing the code to display the help file associated with
*     the default version of the ArgParser class.  If the arguments
*     processed by ArgParser change, this method will need to change, as
*     well.  This file contains a single routine: DisplayHelp() that
*     writes the help file to std::cout.  The routine was broken out
*     into a separate file to improve readability of the main routine(s)
*     and to allow multiple main()'s using the same ArgParser to share
*     the help.
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

#include <iostream>
#include <string>

#include <display_help.hpp>


// Display the help message on cout.
//
// "exe_name" is a string containing the full name of the executable, for
//       example, "text_event.exe".
// "description" is a string containing the description text for the
//       executable.  This text should be formatted in a fashion
//       consistent with the rest of the help message.
void DisplayHelp(const std::string &exe_name, 
                 const std::string &description) {
  // The escape characters consume space in the following source code, but
  // not on the display screen, so body text needs to actually be offset
  // two spaces to the left to align properly on the display.
  std::cout << 
  "\n\n" << exe_name << "\n"
  "\n"
  "Usage:  " << exe_name << " [-CPathToConfigFile] [-LPathToLogFile]\n"
  "                      [-SPathToStimulusFile] [-TRunUntilTime]\n"
  "                      [-h]\n"
  "\n"
  "    Required Arguments:\n"
  "\n"
  "        None\n"
  "\n"
  "    Optional Arguments:\n"
  "\n"
  "        \"-C\" Followed immediately by a string specifying the pathname\n"
  "             for the parameter configuration file.\n"
  "             If this argument is not specified, \"./setup.txt\" will\n"
  "             be used.\n"
  "        \"-L\" Followed immediately by a string specifying the pathname\n"
  "             for the data log file.\n"
  "             If this argument is not specified, \"./logfile.csv\" will\n"
  "             be used.\n"
  "        \"-S\" Followed immediately by a string specifying the pathname\n"
  "             for the the stimulus file.\n"
  "             If this argument is not specified, \"./stim.csv\" will\n"
  "             be used.\n"
  "        \"-T\" Followed immediately by an floating point value\n" 
  "             specifying the upper simulation run time limit, in terms\n"
  "             user time units.  This value MUST be > 0.0 AND <= the\n"
  "             maximum magnitude allowed for this platform!\n"
  "             If no run until time is specified, the simulation will\n"
  "             terminate after 100000.0 user time units.\n"
  "             Users should be aware that each user time unit represents\n"
  "             some quantity of internal time ticks.  This value is\n"
  "             currently a constant set in \"sim_time.cc.\" the underlying\n"
  "             datatypes should be large enough to represent values for\n" 
  "             most situations.\n"
  "             NOTE:  \"RunUntilTime\" may be specified as the string\n"
  "                    \"MAX\", (\"-TMAX\") to set the simulation time\n"
  "                    to the maximum allowable value on this platform.\n"
  "             Temporary Note:  Some development environments don't\n"
  "                              support converting a text value to a\n"
  "                              full precision UserTime.  If you're\n"
  "                              using the command-line argument, you\n"
  "                              may be limited in terms of precision\n"
  "                              and magnitude. You might want to try\n"
  "                              specifying \"-TMAX\" instead.\n"
  "        \"-h\" Optionally followed immediately by the string \"elp\"\n"
  "             results in this message being printed to standard out.\n"
  "\n"
  "        Note: Specifying a valid switch, without an accompanying\n"
  "        field argument, (for example, just \"-L\") will not be\n"
  "        flagged as an error.  However, the default value will not\n"
  "        be overwritten.\n"
  "\n"
  "    Example:\n"
  "\n"
  "        " << exe_name << " -S./mystim.csv -T1.5E7\n"
  "\n"
  "Description:\n"
  "\n" << description << "\n"
  "The " << exe_name << " executable was built with 'g++ -std=c++11'.\n"
  "\n";
}
