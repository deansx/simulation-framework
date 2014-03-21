/*****************************************************************************
*
*   DESCRIPTION:
*     Defines the common message display functions used by the simulation
*     environment
*     These messages are gathered here to ensure consistency and to more
*     easily support global changes
*     
*     There is one core message display function "UtilStatusMessage()".
*     The other status display functions are convenience wrappers.
*     
*   
*   STATUS:  Prototype
*   VERSION:  1.00
*   CODER:  Dean Stevens
*
*   LICENSE:  The MIT License (MIT)
*             See LICENSE.txt in the root (sim) directory of this project.
*   Copyright (c) 2014 Spinnaker Advisory Group, Inc.
*
 ****************************************************************************/

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <deque>

#include "common_strings.hpp"
#include "common_messages.hpp"

// Splits the "message" string into multiple lines at newline markers.
//
// "message" - string to be separated into individual lines.
// Returns - the newly divided lines in the "result" queue.  Caller
//       is responsible for creating the "result" queue and managing
//       the storage.
void UtilSplitIntoLines(const std::string &message, 
                        std::deque <std::string> *result) {
  // Create a stringstream with the "message" as its contents.
  std::istringstream msg_stream(message);
  // Will temporarily hold each line as it's extracted from the message
  std::string line;
  // Walk through the stream, get each line and place it on the queue
  while (std::getline(msg_stream, line)) {
    result->push_back(line);
  }
}  // UtilSplitIntoLines


// Displays one of the standard status message types (Error / Note / Warning),
// with multiple properly indented error lines of caller provided content.
// The specific output stream (cout | cerr) is determined by the message
// type.
// 
// "msg_label" - specifies the message type, as declared in
//       "common_strings.hpp"
// "message" - specifies the content of the error message.  The message can
//       contain an arbitrary number of lines, separated by "\n"
void UtilStdMsg(const std::string &msg_label,
                const std::string &message) {
  UtilWhichStream which;
  std::string new_label;
  if ((msg_label == kCommonStrWarn) || (msg_label == kCommonStrNote)) {
    // The caller specified one of the standard types that go to cout
    // transfer the label that was provided
    new_label = msg_label;
    which = kOut;
  } else {
    // Everything else will be consider to be a kCommonStrError.
    new_label = kCommonStrError;
    which = kErr;
  }
  UtilStatusMessage(new_label, which, message);
}  // UtilStdMsg


// Displays a fatal error message, with multiple properly indented error
// lines, on "cerr" and exits with EXIT_FAILURE
//
// "message" - specifies the content of the error message.  The message can
//       contain an arbitrary number of lines, separated by "\n"
void UtilFatalErrorAndDie(const std::string &message) {

  UtilStatusMessage(kCommonStrFatal, kErr, message);
  // The "Exiting" line is always printed
  std::cerr << std::setw(kCommonStrFatal.length()) << ' '
            << "Exiting.\n";
  // Finally terminate with failure status.
  exit(EXIT_FAILURE);
}  // UtilFatalErrorAndDie


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
void UtilStatusMessage(const std::string &msg_label,
                       const UtilWhichStream where,
                       const std::string &message) {
  // The message is buffered in a "stringstream", then output on the
  // specified stream.
  std::stringstream msg_buffer;

  // Split the message into individual lines, cached in "line_queue"
  std::deque <std::string> *line_queue =  new std::deque <std::string>;
  UtilSplitIntoLines(message, line_queue);

  // Format the message for output:
  // The queue shouldn't be empty, but just to be safe...
  if (!line_queue->empty()) {
    // The first line has the message label, so it gets special treatment
    msg_buffer << msg_label << line_queue->front() << std::endl;
    line_queue->pop_front();
  } else {
    msg_buffer << msg_label << std::endl;
  }
  // Loop through the rest of the queue, format each line and put it
  // in the buffer.
  while (!line_queue->empty()) {
    msg_buffer << std::setw(msg_label.length()) << ' '
               << line_queue->front() << std::endl;
    line_queue->pop_front();
  }
  // Clean up the queue;
  delete line_queue;
  
  // Select the appropriate output stream and write the buffer.
  if (where == kOut) {
    std::cout << msg_buffer.str();
  } else {
    std::cerr << msg_buffer.str();
  }
}  // UtilStatusMessage

