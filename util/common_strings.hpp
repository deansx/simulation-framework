/*****************************************************************************
*
*   DESCRIPTION:
*     Header file declaring common strings used by the Discrete Event
*     Simulation environment.
*     These strings are gathered here to ensure consistency and to more
*     easily support global changes
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

#ifndef SIM_UTIL_COMMON_STRINGS_HPP_
#define SIM_UTIL_COMMON_STRINGS_HPP_

#include <string>

// Error / Note / Warning prefixes
extern const std::string kCommonStrFatal;
extern const std::string kCommonStrError;
extern const std::string kCommonStrWarn;
extern const std::string kCommonStrNote;
extern const std::string kCommonCopyright;
extern const std::string kCommonLicense;

#endif   // SIM_UTIL_COMMON_STRINGS_HPP_
