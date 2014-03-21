/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring the ConfigMgr (Config Manager) pure virtual class.
*     This class provides a template for implementation specific
*     configuration file managers.  Since the class is pure virtual, most
*     of the methods must be overridden in derived classes to create objects
*     that are actually useful.
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

#ifndef SIM_UTIL_CONFIG_MGR_HPP_
#define SIM_UTIL_CONFIG_MGR_HPP_

#include <string>
#include <fstream>

#include "basic_defs.hpp"


class ConfigMgr {
 public:
  // This constructor handles creating and opening a stream to the config file
  // specified by "config_path".
  // NOTE:  A fatal error will be generated and the application will exit
  // with a failure if the "config_path" cannot be opened.
  //
  // "config_path" - pathname for the configuration file.
  ConfigMgr(std::string config_path);

  // The dtor closes the stream and performs other trivial housekeeping
  // details.
  ~ConfigMgr();

  // Loads the data from the configuration file.
  // Must be redefined in the derived classes.
  //
  // Returns - "true" if successful, "false" otherwise.  If there are
  //       serious issues with the config file, the implementor may choose
  //       to simply generate a fatal error message and terminate.
  virtual bool LoadConfiguration() = 0;

  // Mutator/Accessor for the status of the data stored in this object
  //
  // "ready_flag" - signifies that the data stored in the config manager
  //       is ready to be accessed.  Since derived classes actually manage
  //       the data, they must be responsible for maintaining  this field.
  void set_data_ready(const bool ready_flag) { data_ready_ = ready_flag; };
  //
  // Returns - the current state of the "data_ready_" flag.
  bool data_ready() const { return data_ready_; };

 protected:
  // Stream for reading configuration data
  std::ifstream *config_stream_;

 private:
  // The data in this object is ready to be accessed
  bool data_ready_;

  // As per the coding standard
  DISALLOW_COPY_AND_ASSIGN(ConfigMgr);
};

#endif // SIM_UTIL_CONFIG_MGR_HPP_
