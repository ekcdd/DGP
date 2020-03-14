/*
   DGP - Deterministically Generated Passwords
   Filename: dgp.cpp
   Description: Everything needed to make DGP work.
   Author: Adam Oldham (ekcdd@gmx.com)

   Copyright (c) 2020  Adam Oldham
   All rights reserved.

   Distributed under the MIT/X11 software license, see the accompanying
   file license.txt or http://www.opensource.org/licenses/mit-license.php.

*/

#include "dgp.h"

static uint64_t next = 1;

static std::string alpha_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static std::string num_table = "0123456789";
static std::string special_table = "`~!@#$%^&*()/{[]}\\|,<.>?";

int32_t dpg_rand(uint64_t max);
std::string ToLower(std::string s);
int64_t GenerateSeed(std::string data, uint64_t len);

std::string GeneratePasswordV1(DGPOptions options);
std::string GeneratePasswordV2(DGPOptions options);

DGPOptions CreateEmptyOptions()
{
    DGPOptions opts;
    opts.Length = Default_Password_Length;
    opts.Rounds = 1;
    opts.Version = DefaultDGPVersion;
    opts.MasterKey = "";
    opts.MasterKeyLength = 0;
    opts.Name = "";
    opts.NameLength = 0;
    opts.AlphaNumericOnly = false;

    return opts;
}
bool DGP(DGPOptions options,std::string &out)
{
    if (options.MasterKey == "")
     {
         return false;
     }
     if (options.Name == "")
     {
         return false;
     }
     if (options.Length == 0)
     {
         options.Length = Default_Password_Length ;
     }
     if (options.Rounds == 0)
     {
         options.Rounds = 1;
     }

    options.Name = ToLower(options.Name);
    switch (options.Version)
    {
        case DGPVersion::One:
            out = GeneratePasswordV1(options);
            return true;
        case DGPVersion::Two:
            out = GeneratePasswordV2(options);
            return true;
        default:
        return false;
    }

}
std::string ToLower(std::string s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return result;
}
int32_t dpg_rand(uint64_t max)
{
    if (next == 0) next++; // make sure next is not zero, thanks Keith for find this bug.
    next = next * (1103515245 + 97111);
    uint64_t ud = static_cast<uint64_t>((next / 65536) % max);
    return  static_cast<int32_t>(ud);
}

int64_t GenerateSeed(std::string data, uint64_t len)
{
    int64_t hash1 = (6975 << 16) + 6975;
    int64_t hash2 = hash1;
    for (unsigned int i = 0; i < len; i++)
    {
        char c = static_cast<char>(data[i]);
        hash1 = ((hash1 << 5) + hash1) ^ c;
        if (c == 0)
            break;
        hash2 = ((hash2 << 5) + hash2) ^ c;
    }
    int64_t r = hash1 + (hash2 * 1577836800);
    return r;
}

std::string GeneratePasswordV1(DGPOptions options)
{
    uint64_t li = 0;
    for (unsigned int i = 0; i < options.Rounds; i++) {
        uint64_t s1 = static_cast<uint64_t>(GenerateSeed(options.MasterKey, options.MasterKeyLength));
        uint64_t s2 = static_cast<uint64_t>(GenerateSeed(options.Name, options.NameLength));
        li += s1 + s2;
    }
    next = li;
    char *result = new char[options.Length];
    int lastNum = 0;
    int lastspec = 0;
    int pos1 = dpg_rand(9);
    int pos2 = dpg_rand(9);
    for (unsigned int i = 0; i < options.Length; i++)
    {
        if (lastNum == pos1) {
            result[i] = num_table[static_cast<uint32_t>(dpg_rand(10))];
            lastNum = 0;
        }
        // This *should* be else if but changing this will result in
        // a differently generated password. So for compatibility
        // reasons, this will be left as it is forever or until
        // the algorithm is changed causing different passwords.
        if (lastspec == pos2 && options.AlphaNumericOnly == false) {
            result[i] = special_table[static_cast<uint32_t>(dpg_rand(24))];
            lastspec = 0;
        }
        else {
            result[i] = alpha_table[static_cast<uint32_t>(dpg_rand(52))];
            lastNum++;
            lastspec++;
        }
    }

    std::string p = std::string(result, options.Length);
    delete[] result;

    return p;
}

std::string GeneratePasswordV2(DGPOptions options) {
    uint64_t li = 0;
    for (unsigned int i = 0; i < options.Rounds; i++) {
        uint64_t s1 = static_cast<uint64_t>(GenerateSeed(options.MasterKey, options.MasterKeyLength));
        uint64_t s2 = static_cast<uint64_t>(GenerateSeed(options.Name, options.NameLength));
        li += s1 + s2;
    }
    next = li;
    char *result = new char[options.Length];
   // std::string result = "";
    int typeLength = 0;
    int lastType = -1;
    unsigned int opt;
    if (options.AlphaNumericOnly)
    {
        opt = 20;
    }
    else {
        opt = 30;
    }
    for (unsigned int i = 0; i < options.Length; i++)
    {
        int charType = dpg_rand(opt);
        if ( charType == lastType && typeLength >= 4)
        {
            while ((charType = dpg_rand(opt)) == lastType)
            {
                // Do nothing but update charType
            }
            lastType = charType;
            typeLength = 0;

        }
        typeLength++;
        lastType = charType;


        if (charType < 10) {
            result[i] = alpha_table[static_cast<uint32_t>(dpg_rand(52))];
        }else if (charType < 20)
        {
            result[i] = num_table[static_cast<uint32_t>(dpg_rand(10))];
        }
        else {
            result[i] = special_table[static_cast<uint32_t>(dpg_rand(24))];
        }
    }

    std::string p = std::string(result, options.Length);
    delete[] result;

    return p;
}
