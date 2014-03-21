/*****************************************************************************
*
*   DESCRIPTION:
*     Implementation file for the time object of the Discrete Event Simulation
*     system.
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
#include <limits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <stdio.h>

#include "sim_time.hpp"
#include "common_strings.hpp"
#include "common_messages.hpp"
#include "common_utilities.hpp"

// Scale factor specifying how many simulation ticks represent a single
// user time unit.  For example, if the user time unit is seconds, and
// you want the granularity of the simulation to be milliseconds, set
// this to 1000.0L
constexpr static SimTime::UserTime kTicksPerUserTimeUnit = 100.0L;
// Maximum number of ticks that can be represented on this architecture.
constexpr static SimTime::SimTick kMaxTicks = 
                           std::numeric_limits<SimTime::SimTick>::max();
// The maximum number of ticks converted to a real number
constexpr static SimTime::UserTime kMaxRealNumTicks = 
                      static_cast<SimTime::UserTime>(kMaxTicks);
// Since each time unit might be represented by some number of simulation
// ticks, we need to set the maximum time unit to account for the scaling
constexpr static SimTime::UserTime kMaxUserTimeUnits = 
                      kMaxRealNumTicks / kTicksPerUserTimeUnit;


// Default ctor initializes to zero
SimTime::SimTime() {
  ticks_ = 0;
}


// Construct from a UserTime value
SimTime::SimTime(const SimTime::UserTime to_time) {
  SimTime::SetTime(to_time);
}


// Copy Ctor, copies time from 'other'.
// Although this violates the coding standard, it seemed reasonable for the
// sake of efficiency, since time objects are often embedded in other
// simulation objects.  They also get passed around & assigned frequently.
// 
// "other" - the SimTime object whose value will be copied into "this"
SimTime::SimTime(const SimTime& other) {
  CopyFrom(other);
}


// Dtor
SimTime::~SimTime() {
}


// Overload '=' to properly copy time to "this" object from "other"
// Although this violates the coding standard, it seemed reasonable for the
// sake of efficiency, since time objects are often embedded in other
// simulation objects.  They also get passed around & assigned frequently.
//
// "other" - SimTime object whose time value will be assigned to this
//       object.
// Returns - a reference to the changed object.
SimTime &SimTime::operator=(const SimTime& other) {
  CopyFrom(other);
  return *this;
}


// Add time from the "addend" SimTime object to "this" object's time.
// Issues a fatal error and exits on overflow.  This is not expected to occur
// during "normal operations," so "OrDie" was not appended.
//
// "addend" - the SimTime value to add to "this" object.
void SimTime::AddTime(const SimTime &addend) {
  SimTime::UserTime overflow = static_cast<SimTime::UserTime>(ticks_) + 
             static_cast<SimTime::UserTime>(addend.ticks_);
  if (!(overflow > kMaxRealNumTicks)) {
    ticks_ += addend.ticks_;
  } else {
    SimTime::UserTime add_time = addend.GetUserTime();
    std::stringstream message;
    message << "Adding " << add_time << " to " << this->GetUserTime()
            << " would exceed\npermissible range.";
    UtilFatalErrorAndDie(message.str());
  }
}


// Add the "addend" UserTime's value to this object's time.
// Issues a fatal error and exits on overflow.  This is not expected to occur
// during "normal operations," so "OrDie" was not appended.
//
// "addend" - the UserTime value to add to "this" object.
void SimTime::AddTime(const UserTime &addend) {
  SimTime::UserTime real_ticks2add = addend * kTicksPerUserTimeUnit;
  SimTime::UserTime overflow = static_cast<SimTime::UserTime>(ticks_) + 
             real_ticks2add;
  if (!(overflow > kMaxRealNumTicks)) {
    ticks_ += this->RealTicks2Ticks(real_ticks2add);
  } else {
    std::stringstream message;
    message << "Adding " << addend << " to " << this->GetUserTime()
            << " would exceed\npermissible range.";
    UtilFatalErrorAndDie(message.str());
  }
}


// Set "this" object's time to the time specified in the "to_time" object.
//
// "to_time" - the SimTime value used to set "this" object's new time.  The
//       "to_time" object is assumed to be within range, so no over/under flow
//       checks are performed
void SimTime::SetTime(const SimTime &to_time) {
  CopyFrom(to_time);
  // Don't check for overflow, assume parameter's time is OK
}


// Set "this" object's time to the UserTime value specified in "to_time"
// Issues a fatal error and exits on overflow.  This is not expected
// to occur during "normal operations," so "OrDie" was not appended.
//
// "to_time" - the UserTime value used to set "this" object's new time.
void SimTime::SetTime(const UserTime &to_time) {
  if ((to_time >= 0.0) && !(to_time > kMaxUserTimeUnits)) {
    ticks_ = this->RealTicks2Ticks(to_time * kTicksPerUserTimeUnit);
  } else if (to_time < 0.0) {
    ticks_ = 0;
    std::stringstream message;
    message << "Simulation cannot process negative times.  Using 0.0\n"
               "instead of the specified value of " << to_time;
    UtilStdMsg(kCommonStrWarn, message.str());
  } else {
    std::stringstream message;
    message << to_time << " exceeds permissible range.\nMaximum Value for "
                          "Time Units is: " << kMaxUserTimeUnits;
    UtilFatalErrorAndDie(message.str());
  }
}


// Get this object's UserTime.  Since times are stored in simulator
// ticks, we need to convert the tick count into a user time value.
//
// Returns - the UserTime represented by this time object
SimTime::UserTime SimTime::GetUserTime() const {
  return ticks_ / kTicksPerUserTimeUnit;
}


// Returns - the maximum UserTime value that can be represented in ticks
//       for the time object.
SimTime::UserTime SimTime::GetMaxUserTime() {
  return kMaxUserTimeUnits;
}


// Returns - a string containing the MaxUserTime value formatted at full
//       precision (plus, perhaps a bit more, for effect)
std::string SimTime::GetMaxUserTimeLongString() {
  // This stream will be formatted to represent kMaxUserTimeUnits at a
  // very large precision
  std::stringstream formatted;
  // No need to save the settings, since formatted is a local variable
  UtilSetStreamMaxPrecision(&formatted);
  formatted <<  kMaxUserTimeUnits;

  return formatted.str();
}


#ifdef TEST_HARNESS
// Returns - the ticks/user time unit factor.  Only used for the test
//       harness
SimTime::UserTime SimTime::GetTicksPerUserTimeUnit() {
  return kTicksPerUserTimeUnit;
}
#endif


// Convert a real number of ticks into an integral number of ticks.  We don't
// fault for overflow, because this should never be exposed to outside the
// SimTime module.  Handles rudimentary rounding.
//
// Returns - the converted number of SimTicks.
SimTime::SimTick SimTime::RealTicks2Ticks(const SimTime::UserTime &real_ticks) {
  SimTime::SimTick fltm;
  if (!((real_ticks + 0.5) > kMaxRealNumTicks)) {
    fltm = static_cast<SimTime::SimTick>(floor(real_ticks + 0.5L));
  } else if (real_ticks < 0.0) {
    fltm = 0;
  } else {
    fltm = kMaxTicks;
  }
  return fltm;
}
