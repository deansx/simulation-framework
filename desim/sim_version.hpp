/*****************************************************************************
*
*   DESCRIPTION:
*     Declarations for the version string manager class of the simulation
*     system.
*     This class maintains the several strings representing version and build
*     information.
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

#ifndef SIM_DESIM_SIM_VERSION_
#define SIM_DESIM_SIM_VERSION_

#include <string>

#include <basic_defs.hpp>

// Maintains the version / build datetime information for this system.
class SimVersion {
 public:
  SimVersion() { };
  ~SimVersion() { };

  // Returns - a string containing the full formatted description of the
  // build.
  std::string FullVersionBuildString();

  // The version number string follows the format:
  // "v.major_version.minor_version" where both version number components
  // consist of two digits, for example: "Version: v.02.04"
  //
  // Returns - the version number string.
  std::string VersionNumberString();

  // The revision date represents the date when the version number was most
  // recently changed.  The revision date string uses the format:
  // "Revision Date: yyyy.mm.dd" - both month and day are 0 padded, for
  // example: "Revision Date: 2014.01.15"
  //
  // Returns - the Revision date string.
  std::string RevisionDateString();

  // The build datetime marks the actual time that the system was built,
  // more correctly the time that the source file associated with this
  // header was compiled.  The version source file with each significant
  // build to update the value storred for this string.
  // The build datetime string uses the system standard __DATE__ and
  // __TIME__ formats: "Build Datetime: mmm dd YYYY" - the day field
  // is space padded, for example: "Build Datetime: Feb 21 2014 10:24:51"
  //
  // Returns - the build datetime string.
  std::string BuildDateTimeString();

  // The formatted version string is sent to cout.
  //
  // "message" - text to be printed on a separate line immediately before
  //       the version information.  Callers could choose to provide some
  //       introduction, such as "Version Information:"  Specify the empty
  //       string ("") for message to print just the version information
  void PrintVersion(const std::string &message);
 private:
  // These values must be updated with the date of record, and version info
  //  before every release.
  static const std::string revision_date_;
  static const std::string version_number_;
  static const std::string build_datetime_;

  // These values are set at compile time, based on compile time directives
  static const std::string os_platform_;
  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(SimVersion);
}; // end class Version

#endif   // SIM_DESIM_SIM_VERSION_
