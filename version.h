/*
   DGP - Deterministically Generated Passwords
   Filename: version.h
   Description: Version information required for Windows compilation.
   Author: Adam Oldham (ekcdd@gmx.com)

   Copyright (c) 2020  Adam Oldham
   All rights reserved.

   Distributed under the MIT/X11 software license, see the accompanying
   file license.txt or http://www.opensource.org/licenses/mit-license.php.

*/


#ifndef VERSION_H
#define VERSION_H

#define VER_FILEVERSION             0,2,0,0
#define VER_FILEVERSION_STR         "0.2.0.0\0"

#define VER_PRODUCTVERSION          0,2,0,0
#define VER_PRODUCTVERSION_STR      "0.2\0"

#define VER_SHORTNAME               "DGP"
#define VER_COMPANYNAME_STR         "ekcdd"
#define VER_FILEDESCRIPTION_STR     "Generate complex and derived passwords"
#define VER_INTERNALNAME_STR        "Deterministically Generated Passwords"
#define VER_LEGALCOPYRIGHT_STR      "Copyright © 2020 ekcdd"
#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#ifdef QT
#define VER_ORIGINALFILENAME_STR    "DGP-Qt.exe"
#define VER_PRODUCTNAME_STR         "DGP-Qt"
#else
#define VER_ORIGINALFILENAME_STR    "DGP-CLI.exe"
#define VER_PRODUCTNAME_STR         "DGP-CLI"
#endif

#define VER_COMPANYDOMAIN_STR       "https://www.github.com/ekcdd/DGP/"

#endif // VERSION_H
