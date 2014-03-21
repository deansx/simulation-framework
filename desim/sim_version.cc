/*****************************************************************************
*
*   DESCRIPTION:
*     Implementation file for the version string manager of the simulation
*     system.
*     Maintains the several strings representing version and build
*     information.
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

#include <iostream>
#include <string>
#include <cstdio>

#include "sim_version.hpp"

// Define & initialize the static const member variables

// Operating system platform
#if defined(LINUX)
const std::string SimVersion::os_platform_ = " LINUX";
#elif defined(CYGWIN)
const std::string SimVersion::os_platform_ = " cygwin";
#elif defined(UNIX)
const std::string SimVersion::os_platform_ = " UNIX";
#elif defined(OSX)
const std::string SimVersion::os_platform_ = " OS X";
#elif defined(WIN32)
const std::string SimVersion::os_platform_ = " Win32";
#elif defined(WIN64)
const std::string SimVersion::os_platform_ = " Win64";
#else
const std::string SimVersion::os_platform_ = " ***UNKNOWN OS***";
#endif

// Date & version number data.
// It's STRONGLY recommended that you not edit these values manually.
// Instead, it's a best practice to manage the version data with the
// dedicated script "update_version.bsh"
// The script is located in the project's scripts directory, at:
// "../scripts/"
const std::string SimVersion::revision_date_ = "2014.03.04";
const std::string SimVersion::version_number_ = "01.01";
const std::string SimVersion::build_datetime_ = __DATE__ " " __TIME__;


// The version number string follows the format:
// "v.major_version.minor_version" where both version number components
// consist of two digits, for example: "Version: v.01.01"
//
// Returns - the version number string.
std::string SimVersion::VersionNumberString() {
  std::string ret_string("Version: v.");
  ret_string.append(version_number_);
  return ret_string;
}


// The revision date represents the date when the version number was most
// recently changed.  The revision date string uses the format:
// "Revision Date: yyyy.mm.dd" - both month and day are 0 padded, for
// example: "Revision Date: 2014.03.04"
//
// Returns - the Revision date string.
std::string SimVersion::RevisionDateString() {
  std::string ret_string("Revision Date: ");
  ret_string.append(revision_date_);
  return ret_string;
}


// The build datetime marks the actual time that the system was built,
// more correctly the time that the source file associated with this
// header was compiled.  The version source file with each significant
// build to update the value storred for this string.
// The build datetime string uses the system standard __DATE__ and
// __TIME__ formats: "Build Datetime: mmm dd YYYY" - the day field
// is space padded, for example: "Build Datetime: Feb 21 2014 10:24:51"
//
// Returns - the build datetime string.
std::string SimVersion::BuildDateTimeString() {
  std::string ret_string("Build Datetime: ");
  ret_string.append(build_datetime_);
  return ret_string;
}


// Assembles the version components (Version Number, Revision 
// Date & OS into a single string with punctuation and line breaks in
// (hopefully) appropriate places.
//
// Returns - a string containing the full formatted description of the
//       build.  
std::string SimVersion::FullVersionBuildString() {
  std::string ret_string = VersionNumberString();
  ret_string.append("\n").append(RevisionDateString())
            .append(";  ").append(BuildDateTimeString())
            .append("\nCompiled For: ").append(os_platform_);
  return ret_string;
}


// The formatted version string is sent to cout.
//
// "message" - text to be printed on a separate line immediately before
//       the version information.  Callers could choose to provide some
//       introduction, such as "Version Information:"  Specify the empty
//       string ("") for message to print just the version information
void SimVersion::PrintVersion(const std::string &message) {
  // Print the introduction, if it's available
  if (!message.empty()) {
    std::cout << message  << std::endl;
  }
  std::cout << SimVersion::FullVersionBuildString() << std::endl;
}
