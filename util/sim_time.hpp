/*****************************************************************************
*
*   DESCRIPTION:
*     Header file describing the time object for the Discrete Event Simulation
*     system.
*     NOTE:  The specific implementation of this type may evolve as the
*            requirements for the tool grow.  It may seem inefficient to
*            have a class with (initially) such a simple data member, but
*            this will isolate other parts of the implemenation from future
*            changes to the time representation.
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

#ifndef SIM_UTIL_SIM_TIME_HPP_
#define SIM_UTIL_SIM_TIME_HPP_

#include <stdint.h>
#include <string.h>

#include "basic_defs.hpp"



class SimTime {
 public:
  // Type representing User Time units, for example msec.  See the .cc file for
  // the scale factor applied with this build.  Using long double to ensure that
  // the mantissa can hold all of the precision in a SimTimeTick
  typedef long double UserTime;

  // Type representing the internal simulator "ticks" - This is only public so
  // that the typedef can be used for internal module const static 
  // initialization.
  // Don't use this in your code, as future changes may break your code.
  typedef uint64_t SimTick;

  // Default ctor sets time to 0
  SimTime();
  // Converts specified value to simulator ticks & sets object's time
  // to the result
  //
  // "to_time" - UserTime value that the new object will be  initialized
  //       with.
  SimTime(const UserTime to_time);
  // Since SimTime is embedded in multiple simulation classes, it's not
  // particularly feasible to always provide clone functions, so we go ahead
  // and provide a copy constructor, and an assignment operator.  We recognize
  // the deviation from the coding standard.  We also recognize that the
  // compiler would create both of these automatically, but the coding
  // standard mandates that they be present.
  //
  // "other" - SimTime object whose time value will be used to initialize
  //       the new object.
  SimTime(const SimTime& other);
  ~SimTime();

  // Methods to adjust this object's time:

  // Sets the time in "this" object to the time value specified in "other"
  // Justification for violating the coding standard is presented above.
  //
  // "other" - SimTime object whose time value will be assigned to this
  //       object.
  // Returns - a reference to the changed object.
  SimTime &operator=(const SimTime& other);

  // NOTE:  Negative simulation time is not supported, so any adjustments
  //       that would otherwise result in a sim time of less than zero
  //       will instead cause the sim time to be set to zero.
  // WARNING: Any adjustments that would overflow the variable storing
  //       sim time are considered a fatal error.  These overflows will
  //       will result in the issuance of a fatal error message and an
  //       exit with failure status.
  //       "OrDie" not appended to the following function names, since this
  //       is not expected to occur during "normal operations."
  // For now, as described above, violating the upper bound is considered
  //       a fatal error, while attempting to set a negative simulation
  //       time is handled gracefully.  This may not be desired behavior,
  //       and may change in future versions.
  //
  // "addend" - the value to add to "this" object.
  void AddTime(const SimTime &addend);
  void AddTime(const UserTime &addend);

  // Convenience methods, could also use overloaded assignment operator or
  // copy CTOR.
  //
  // "to_time" - for both of the following methods, the time value used to
  //       set "this" object's new time

  // For this variant, the "to_time" object is assumed to be within range,
  // so no over/under flow checks are performed
  void SetTime(const SimTime &to_time);
  // For this variant, as discussed above, overflow and fatal error are
  // possible.
  void SetTime(const UserTime &to_time);

  // Time comparison methods:

  // Argument/Return value applies to the following group of methods
  //
  // "test" - the time value to test against.
  // Returns - "true" if the condition is true, "false" otherwise
  //       
  // This time is less than (<) the test time
  bool EarlierThan(const SimTime &test) const 
                  { return ticks_ < test.ticks_; };
  // This time is less than, or equal to, (<=)the test time
  bool AsEarlyAs(const SimTime &test) const 
                  { return ticks_ <= test.ticks_; };
  // This time is the same as (==) the test time
  bool SameAs(const SimTime &test) const 
                  { return ticks_ == test.ticks_; };

  // Returns - the time represented by this object, in TimeUnits
  UserTime GetUserTime() const;

  // Compile time determined constant, representing the maximum allowable
  // user time value.  Depends on both the representation of numbers on
  // this platform and the "kTicksPerUserTimeUnit" constant defined in
  // "sim_time.cc".
  //
  // Returns - the max UserTime
  static UserTime GetMaxUserTime();

  // The above value represented as a string that shows full precision.
  // May be useful for formatted error messages.
  //
  // Returns - a string representing max UserTime
  static std::string GetMaxUserTimeLongString();

#ifdef TEST_HARNESS
  // Test/debug accessor for the simulation ticks.  Should never be used in
  // production code, but needed for test harness.
  //
  // Returns - number of ticks represented by this object
  SimTick ticks() const { return ticks_; }

  // Test/debug accessor for the ticks/user time unit factor.  Probably
  // shouldn't be used in production code, but useful for the test harness.
  //
  // Returns - number of ticks per user time unit.
  static UserTime GetTicksPerUserTimeUnit();
#endif
  
 private:
  // Utility method to convert ticks represened by a real number into
  // SimTicks.  Handles rounding to max magnitude of SimTick type.
  // "real_ticks" - the quantity to be converted to "SimTicks"
  //
  // Returns - the converted number of SimTicks.
  SimTick RealTicks2Ticks(const UserTime &real_ticks);

  // Utility method for copying from another SimTime object.  Note the lack
  // of overflow testing.  We assume that overflow checks occurred before
  // 
  // "other" - the SimTime object whose value will be copied into "this"
  void CopyFrom(const SimTime &other) { ticks_ = other.ticks_; }

  // Simulation clock ticks
  SimTick ticks_;
  // Since SimTime is embedded in multiple simulation classes, it's not
  // particularly feasible to always provide clone functions, so we go ahead
  // and provide a copy constructor and an = operator
  // As per the coding standard
  // DISALLOW_COPY_AND_ASSIGN(SimTime);
}; // end class SimTime
#endif   // SIM_UTIL_SIM_TIME_HPP_
