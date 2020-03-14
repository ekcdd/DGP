/*
   DGP - Deterministically Generated Passwords
   Filename: DGP-CLI.cpp
   Description: Command line interface for generating DGP passwords
   Author: Adam Oldham (ekcdd@gmx.com)

   Copyright (c) 2020  Adam Oldham
   All rights reserved.

   Distributed under the MIT/X11 software license, see the accompanying
   file license.txt or http://www.opensource.org/licenses/mit-license.php.

*/

#include "includes.h"
#include "dgp.h"

std::string center(std::string input, unsigned int width);
void ShowHelp();
void parseCLIArgs(int argc, const char * argv[],DGPOptions &options);
bool ParseInt(std::string s, int &i);

static bool noDecoration;

int main(int argc, const char * argv[]) {

    std::cout << shortName << " " << Version_Major << "." << Version_Minor << "." << Version_Revision << std::endl;
     if (argc <= 1) {
         ShowHelp();
         return EXIT_SUCCESS;
     }
    DGPOptions options = CreateEmptyOptions();

    parseCLIArgs(argc, argv,options);
    // Do some sanity checks
    if (options.MasterKey == "")
    {
        std::cout << "error: no master key set, use -h for more infomation." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (options.Name == "")
    {
        std::cout << "error: a password must have a unique name, use -h for more infomation." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string result = "";
    if (!DGP(options,result))
    {
        std::cout << "error: could not create password" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (noDecoration == true)
    {
        std::cout << result << std::endl;
    }else {
        const unsigned int width = 41;
        std::cout << std::endl << std::endl;
        std::cout << "============= YOUR PASSWORD =============" << std::endl;
        std::cout << "                                         " << std::endl;
        std::cout << center(result, width) << std::endl;
        std::cout << "                                         " << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << std::endl << std::endl;
    }
    return 0;
}

bool ParseInt(std::string s, int &i)
{
    try {
        i = std::stoi(s);
        return true;
    }
    catch (std::exception &e) {
        std::cout << "error: could not parse "  << s << " as an int: " << e.what() << std::endl;
        return false;
    }
}
bool ParseUInt(std::string s, unsigned int &i)
{
    try {
        i = static_cast<unsigned int>(std::stoul(s));
        return true;
    }
    catch (std::exception &e) {
        std::cout << "error: could not parse "  << s << " as an int: " << e.what() << std::endl;
        return false;
    }
}
void parseCLIArgs(int argc, const char * argv[],DGPOptions &options)
{
    std::string a = argv[1];
    int i = 1;
    std::string  next;
    while (i < argc) {
        if (a == "-h") {
            ShowHelp();
            exit(EXIT_FAILURE);
        }
        else if (a == "-p")
        {
            if (i + 1 >= argc)
            {
                std::cout << "error: no password after -p, see -h for more information" << std::endl;
                exit(EXIT_FAILURE);
            }
            next = argv[i + 1];
            options.MasterKey = next;
            options.MasterKeyLength = static_cast<uint32_t>(options.MasterKey.size());
            i += 2;
        }
        else if (a == "-n") {
            if (i + 1 >= argc)
            {
                std::cout << "error: you must include a name to generate a unique password, see -h for more information" << std::endl;
                exit(EXIT_FAILURE);
            }
            next = argv[i + 1];
            options.Name = next;
            options.NameLength = static_cast<uint32_t>(options.Name.size());

            i += 2;
        }
        else if (a == "-a")
        {
            if (options.AlphaNumericOnly == true)
            {
                std::cout << "warning: argument -a has already been set" << std::endl;
            }
            options.AlphaNumericOnly = true;
            i++;
        }
        else if (a == "-r") {
            if (i + 1 >= argc)
            {
                std::cout << "error: you must have a number after -r, see -h for more information" << std::endl;
                exit(EXIT_FAILURE);
            }
            next = argv[i + 1];
            if (ParseUInt(next, options.Rounds) != true)
            {
                std::cout << "warning: " + next << " was not a valid number, a single round will be used." << std::endl;
                options.Rounds = 1;
            }
            if (options.Rounds < 1)
            {
                std::cout << "warning: a round must be equal to or greater than one, a single round will be used." << std::endl;
                options.Rounds = 1;
                i++;
            }
            else {
                i += 2;
            }
        }
        else if (a == "-v") {
            if (i + 1 >= argc)
            {
                std::cout << "error: you must have a number after -v, see -h for more information" << std::endl;
                exit(EXIT_FAILURE);
            }
            next = argv[i + 1];
            unsigned int ver;
            if (ParseUInt(next, ver) == 0)
            {
                std::cout << "warning: " << next << " was not a valid number, using the default version of 1" << std::endl;
                ver = 1;
            }
            auto it = VersionMap.find(ver);
            if (it == VersionMap.end())
            {
                std::cout << "warning: " << ver << " is not a valid version, default to version 1" << std::endl;
                ver = 1;
            }
            options.Version = static_cast<DGPVersion>(ver);
             i += 2;

        }
        else if (a == "-l") {
            if (i + 1 >= argc)
            {
                std::cout << "error: you must have a number after -l, see -h for more information" << std::endl;
                exit(EXIT_FAILURE);
            }
            next = argv[i + 1];
            if (ParseUInt(next, options.Length) == 0)
            {
                std::cout << "warning: " << next << " was not a valid number, the default length of" << Default_Password_Length << " will be used" << std::endl;
                options.Length = Default_Password_Length;
            }
            if (options.Length == 0)
            {
                std::cout << "error: cannot have a password length of zero, cannot continue" << std::endl;
                exit(EXIT_FAILURE);

            }
            else if (options.Length < Min_Password_Length)
            {
                std::cout << "error: using a password equal or less than " << Min_Password_Length << "characters is not secure!" << std::endl;
                exit(EXIT_FAILURE);
            }
            i += 2;
        }
        else if (a == "-d") {
             if (noDecoration == true)
             {
                 std::cout << "warning: argument -d has already been set" << std::endl;
             }
             noDecoration = true;
             i++;
         }
        else {
            std::cout << "warning: " << a << " is an unnkown command line unknown argument" << std::endl;
            i++;
        }
        if (i >= argc) break;
        a = argv[i];
    }
}
void ShowHelp()
{
    std::cout << fullName << " " << desc << std::endl << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "\t-h\t\t Shows this menu" << std::endl;
    std::cout << "\t-p\t\t Sets the master password" << std::endl;
    std::cout << "\t-a\t\t Only use alphanumeric values" << std::endl;
    std::cout << "\t-n\t\t Name the password" << std::endl;
    std::cout << "\t\t\t Changing the name alters the outcome" << std::endl;
    std::cout << "\t\t\t and case is ignored" << std::endl;
    std::cout << "\t-l <len> Length of the generated password" << std::endl;
    std::cout << "\t\t\t The default password length is 8" << std::endl;
    std::cout << "\t-r <len> Number of rounds to use." << std::endl;
    std::cout << "\t\t\t Default rounds is 1 and change the" << std::endl;
    std::cout << "\t\t\t number of rounds will change " << std::endl;
    std::cout << "\t\t\t the password generated" << std::endl;
    std::cout << "\t-v <version> Select the version of the" << std::endl;
    std::cout << "\t\t\t version of the algoritm you want to use." << std::endl;
    std::cout << "\t\t\t 1 is the first version and" << std::endl;
    std::cout << "\t\t\t 2 is the version which has a bug fix" << std::endl;
    std::cout << "\t-d\t\t Removes decoration from the result" << std::endl;
    std::cout << std::endl << std::endl;

}

std::string center(std::string input, unsigned int width) {
    return std::string((width - input.length()) / 2, ' ') + input;
}

