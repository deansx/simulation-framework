/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring common message display functions used by the
*     environment.
*     These message functions are gathered here to ensure consistency and
*     to more easily support global changes
*     
*     Current functions are:
*       UtilStdMsg() - Displays one of the standard message types
*       UtilStatusMessage() - Displays a user-specified message
*       UtilFatalErrorAndDie() - Should be self-explanatory
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

#ifndef SIM_UTIL_COMMON_MESSAGES_HPP_
#define SIM_UTIL_COMMON_MESSAGES_HPP_

#include <string>

// Identifies the output stream ("cout" or "cerr") to receive the message.
enum UtilWhichStream {kOut, kErr};

// Displays one of the standard status message types (Error / Note / Warning),
// with multiple properly indented error lines of caller provided content.
// The specific output stream (cout | cerr) is determined by the message
// type.
// 
// "msg_label" - specifies the message type, as declared in
//       "common_strings.hpp"
//       NOTES:
//       (1) The appropriate output stream ("cout" or "cerr") is determined
//           by the value of "msg_label"
//       (2) If "kCommonStrFatal" behavior is desired, use the specific fatal
//           error function:  "UtilFatalErrorAndDie()"
//       (3) If the message type is not one of (kCommonStrError,
//           kCommonStrWarn, kCommonStrNote), the message will be treated
//           as a "kCommonStrError"
//       (4) If a non-standard message label is desired, use the more generic
//           function:  "UtilStatusMessage()"
// "message" - specifies the content of the error message.  The message can
//       contain an arbitrary number of lines, separated by "\n".
extern void UtilStdMsg(const std::string &msg_label, 
                       const std::string &message);

// Displays a caller specified  message, with multiple properly indented
// error lines, on the specified output stream.
// 
// "msg_label" - specifies the text for the hanging indent.  Content is at
//       the discretion of the caller, but keep in mind that this string
//       will define the depth of the hanging indent, so it is usually a
//       good practice to keep it short.
// "where" - specifies the destination output stream.
// "message" - specifies the content of the error message.  The message can
//       contain an arbitrary number of lines, separated by "\n".
extern void UtilStatusMessage(const std::string &msg_label,
                              const UtilWhichStream where,
                              const std::string &message);

// Displays a fatal error message, with multiple properly indented error
// lines, on "cerr" and exits with EXIT_FAILURE
//
// "message" - specifies the content of the error message.  The message can
//       contain an arbitrary number of lines, separated by "\n"
extern void UtilFatalErrorAndDie(const std::string &message);

#endif   // SIM_UTIL_COMMON_MESSAGES_HPP_
