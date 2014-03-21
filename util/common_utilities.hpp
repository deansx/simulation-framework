/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring some common utility functions used by the
*     simulation environment.  These include:
*       - File existence and access tests.
*       - Stream manipulation methods to modify an output stream's format
*         settings
*     
*     These functions are gathered here simply as a common place for them to
*     live.  There is not necessarily any logical grouping among them.
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

#ifndef SIM_UTIL_COMMON_UTILITIES_HPP_
#define SIM_UTIL_COMMON_UTILITIES_HPP_

#include <sys/types.h>
#include <ios>
#include <iostream>

// FILE EXISTENCE & ACCESS TESTING

// Quick test for the simple existence of "filename".  Does not confirm
// file type, nor access to the file.
//
// "filename" - pathname to test for existence
// Returns - "true" if the file exists. "false" otherwise.
extern bool UtilFileExists(const std::string& filename);

// The following combination methods are slightly more efficient than
// a call to check existence and a separate call to check access.

// Verifies the the file, "filename", exists, is a regular file and that
// the current user has read access to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
extern bool UtilFileExistsRead(const std::string& filename);

// Verifies the the file, "filename", exists, is a regular file and that
// the current user has write access to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
extern bool UtilFileExistsWrite(const std::string& filename);

// Verifies the the file, "filename", exists, is a regular file and that
// the current user has both read and write access to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
extern bool UtilFileExistsReadWrite(const std::string& filename);

// Verifies the the file, "filename", exists, is a regular file and that
// the current user has the specified "access" to it.
//
// "filename" - pathname to test for existence / access
// Returns - "true" if all of the above conditions are met. "false"
//       otherwise.
extern bool UtilFileExistsAccess(const std::string& filename, 
                                 const mode_t access);

// STREAM MANIPULATION:

// Data structure to hold current stream settings.
// Used to cache original settings before changing them to a special
// format.  Returned from "UtilSetStream*", provides an argument describing
// the settings for "UtilSetStreamSettings"
struct UtilStreamSettings {
  std::ios_base::fmtflags flags_;
  std::streamsize precision_;
  std::streamsize width_;
};

// Sets the format of the stream specified by "stream" to display a value
// at a very large (implementation dependent) precision.  Also sets flags
// for scientific notation and upper case.
// Most often used to display the MaxUserUnit at more than full precision.
//
// "stream" - a pointer to the stream to modify.
// Returns - the original settings for the stream in question as a 
//       "UtilStreamSettings" structure.  This value can subsequently
//       be used to restore the steam's settings with
//       "UtilSetSteamSettings()"
extern UtilStreamSettings UtilSetStreamMaxPrecision(std::ios *stream);

// Sets the format of the stream specified by "stream" to the settings
// specified in "settings"
//
// "settings" - specifies the new values for the stream's flags and
//       formatting
// "stream" - a pointer to the stream to modify.
extern void UtilSetStreamSettings(const UtilStreamSettings &settings,
                                  std::ios *stream);

#endif   // SIM_UTIL_COMMON_UTILITIES_HPP_
