/*****************************************************************************
*
*   DESCRIPTION:
*     Defines some common utility functions used by the simulation
*     environment.
*     These functions are gathered here simply as a common place for them to
*     live.  There is not necessarily any logical grouping among them.
*   
*   STATUS:  Prototype
*   VERSION:  0.00
*   CODER:  Dean Stevens
*
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
 ****************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <ios>
#include <iostream>

#include "common_utilities.hpp"


// Quick test for the simple existence of "filename".  Does not confirm
// file type, nor access to the file.
//
// "filename" - pathname to test for existence
// Returns - "true" if the file exists. "false" otherwise.
bool UtilFileExists(const std::string& filename) {
  bool ret_val = false;
  // "buf" will hold the data returned from "stat()"
  struct stat stat_data;
  if (stat(filename.c_str(), &stat_data) != -1) {
    // file exists
    ret_val = true;
  }
  return ret_val;
}


// Verifies the the file, "filename", exists, is a regular file and that
// the current user has the specified "access" to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
bool UtilFileExistsAccess(const std::string& filename, mode_t access) {
  bool ret_val = false;
  // "buf" will hold the data returned from "stat()"
  struct stat stat_data;
  if (stat(filename.c_str(), &stat_data) != -1) {
    // file exists, use the macro to verify that it is a regular file, and
    // then test to see if the user has the specified access.
    if (S_ISREG(stat_data.st_mode) && 
        ((stat_data.st_mode & access) == access)) {
      ret_val = true;
    }
  }
  return ret_val;
}


// Verifies the the file, "filename", exists, is a regular file and that
// the current user has read access to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
bool UtilFileExistsRead(const std::string& filename) {
  return UtilFileExistsAccess(filename, S_IRUSR);
}


// Verifies the the file, "filename", exists, is a regular file and that
// the current user has write access to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
bool UtilFileExistsWrite(const std::string& filename) {
  return UtilFileExistsAccess(filename, S_IWUSR);
}


// Verifies the the file, "filename", exists, is a regular file and that
// the current user has both read and write access to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
bool UtilFileExistsReadWrite(const std::string& filename) {
  return UtilFileExistsAccess(filename, (S_IRUSR | S_IWUSR ));
}


// Sets the format of the stream specified by "stream" to display a value
// at a very large (32 characters) precision.  Also sets flags
// for scientific notation and upper case.
//
// "stream" - a pointer to the stream to modify.
// Returns - the original settings for the stream in question as a 
//       "UtilStreamSettings" structure.
UtilStreamSettings UtilSetStreamMaxPrecision(std::ios *stream) {
  // Capture the current settings of the stream.
  UtilStreamSettings original_settings;
  original_settings.flags_ = stream->flags();
  original_settings.precision_ = stream->precision();
  original_settings.width_ = stream->width();

  // Set display and postion flags
  stream->setf(std::ios_base::uppercase |
                 std::ios_base::scientific);
  stream->setf(std::ios_base::left,
                 std::ios_base::adjustfield);

  // Set width and precision values
  stream->width(32);
  stream->precision(32);

  // returned value can be used to restore original settings.
  return original_settings;
}


// Sets the format of the stream specified by "stream" to the settings
// specified in "settings"
//
// "settings" - specified the new values for the stream's flags and
//       formatting
// "stream" - a pointer to the stream to modify.
void UtilSetStreamSettings(const UtilStreamSettings &settings,
                           std::ios *stream) {
  // Set width and precision values
  stream->width(settings.width_);
  stream->precision(settings.precision_);

  // Set flags
  stream->flags(settings.flags_);
}
