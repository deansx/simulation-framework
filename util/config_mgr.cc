/*****************************************************************************
*
*   DESCRIPTION:
*     File implementing ctors and dtor of the ConfigMgr (Config Manager)
*     pure virtual class.
*     This class provides a template for implementation specific config file
*     managers.  Since the class is pure virtual, most of the methods must
*     be overridden in derived classes to create objects that are actually
*     useful.
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

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "common_strings.hpp"
#include "common_messages.hpp"
#include "config_mgr.hpp"

// This constructor handles creating and opening a stream to the config file
// specified by "config_path".
// Since this is the ctor, "data_ready_" is initialized to "false"
// NOTE:  A fatal error will be generated and the application will exit
// with a failure if the "config_path" cannot be opened.
//
// "config_path" - pathname for the configuration file.
ConfigMgr::ConfigMgr(std::string config_path) : data_ready_(false) {
  config_stream_ = new std::ifstream(config_path);
  if ((config_stream_->is_open()) && (config_stream_->good())) {
    std::cout << kCommonStrNote << "Opened data output file:  \""
         << config_path << "\" successfully.\n";
  } else {
    // Not necessary, since we're exiting, but just to be safe
    config_stream_ = nullptr;
    // Form the string for the error message
    std::stringstream line1;
    line1 << "Could not open configuration file:  \"" 
          << config_path << "\". (ConfigMgr)";
    UtilFatalErrorAndDie(line1.str());
  }
}


// The dtor closes the stream and destroys the stream object.
// Just to be safe, the dtor also sets "data_ready_" to "false".
ConfigMgr::~ConfigMgr() {
  // Since we're destroying the object, this probably isn't needed, but
  // just to be safe
  data_ready_ = false;

  // delete will force the stream to close, but this seems like good
  // hygiene
  config_stream_->close();
  delete config_stream_;
}
