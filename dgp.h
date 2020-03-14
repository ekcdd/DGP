#ifndef DGP_H
#define DGP_H

#include "includes.h"

#define Default_Password_Length  12
#define Min_Password_Length 8
// Version Information
/*
   DGP - Deterministically Generated Passwords
   Filename: dgp.h
   Description: Contains structures, version information and
                declarations for DGP functions
   Author: Adam Oldham (ekcdd@gmx.com)

   Copyright (c) 2020  Adam Oldham
   All rights reserved.

   Distributed under the MIT/X11 software license, see the accompanying
   file license.txt or http://www.opensource.org/licenses/mit-license.php.

*/


const int Version_Major = 0;
const int Version_Minor = 2;
const int Version_Revision = 0;
const std::string shortName = "DGP";
const std::string fullName = "Deterministically Generated Passwords";
const std::string desc = "Generate complex and derived passwords";

enum class DGPVersion {
    One = 1,    // 0.1
    Two = 2,     // 0.2 onwards
    Test = 255

};
static std::map<unsigned int,std::string> VersionMap {
  { static_cast<unsigned int>(DGPVersion::One),"One" },
  { static_cast<unsigned int>(DGPVersion::Two),"Two" }
};

struct DGPOptions {
    DGPVersion Version;
    unsigned int Rounds;
    bool AlphaNumericOnly;
    uint32_t Length;
    uint32_t NameLength;
    std::string Name;
    uint32_t MasterKeyLength;
    std::string MasterKey;
};
const DGPVersion DefaultDGPVersion = DGPVersion::Two;

bool DGP(DGPOptions options,std::string &out);
DGPOptions CreateEmptyOptions();
#endif // DGP_H
