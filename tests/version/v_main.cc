/*****************************************************************************
*
*   DESCRIPTION:
*     Implementation file for the version text fixture of the simulation
*     system.
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
#include <iostream>
#include "sim_version.hpp"

int main(int argc, char *argv[]) {
  SimVersion version;
  std::cout << std::endl << std::endl;
  version.PrintVersion("Test Version Info");
  std::cout << std::endl << version.VersionNumberString() << std::endl;
  std::cout << std::endl << version.RevisionDateString() << std::endl;
  std::cout << std::endl << version.BuildDateTimeString() << std::endl;
  std::cout << std::endl;
  version.PrintVersion("");
  
  return 0;
}  // main
