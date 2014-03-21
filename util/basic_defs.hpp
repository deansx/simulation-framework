/*****************************************************************************
*
*   DESCRIPTION:
*     Header file describing the declarations shared across many files
*     in the simulation environment
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

#ifndef SIM_UTIL_BASIC_DEFS_HPP_
#define SIM_UTIL_BASIC_DEFS_HPP_


// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif   // SIM_UTIL_BASIC_DEFS_HPP_
