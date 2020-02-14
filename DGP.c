//
//  DGP.c
//  DGP (Deterministically Generated Passwords)
//  Version: 0.1.0
//
//  Copyright (c) 2020 Adam Oldham (ekcdd@gmx.com)
//
//  Distributed under the MIT/X11 software license, see the accompanying
//  file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // tolower

// Version Information
const int Version_Major  = 0;
const int Version_Minor = 1;
const int Version_Revision = 0;
const char *shortName = "DGP";
const char *fullName = "Deterministically Generated Passwords";
const char *desc = "Generate complex and derived passwords";

// Function declarations
long GenerateSeed(char *data,unsigned long len);
char* str2lwr(char *str,size_t len);

int dpg_rand(int max);
int ParseInt(const char *s,int *i);

// CLI functions
void ShowHelp(void);
void parseCLIArgs(int argc, const char * argv[]);

// Global variable used for the random number generator
static unsigned long next = 1;

// Password charaters
static const unsigned char alpha_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const unsigned char num_table[] = "0123456789";

static const unsigned char special_table[24] =
"`~!@#$%^&*()/{[]}\\|,<.>?";

// Flags
int AlphaNumericOnly = 0;
int Rounds = 0;
const int DefaultPassLength = 12;
const int Min_Length = 8;
int PassLength = 0;
char *masterKey;
unsigned long masterKeyLen = 0;
char *keyName;
unsigned long keyNameLen = 0;


int main(int argc, const char * argv[]) {
    printf("%s %d.%d.%d\n",shortName,Version_Major,Version_Minor,Version_Revision);
    if (argc <= 1) {
        ShowHelp();
        return EXIT_SUCCESS;
    }
    parseCLIArgs(argc, argv);
    // Do some sanity checks
    if (masterKey == NULL)
    {
        printf("error: no master key set, use -h for more infomation.\n");
        exit(EXIT_FAILURE);
    }
    if (keyName == NULL)
    {
        printf("error: a password must have a unique name, use -h for more infomation.\n");
        exit(EXIT_FAILURE);
    }
    if (PassLength == 0)
    {
        printf("info: setting password length to %d\n",DefaultPassLength);
        PassLength = DefaultPassLength;
    }
    if (Rounds == 0)
    {
        printf("info: setting rounds to 1\n");
        Rounds = 1;
    }
    
    keyName = str2lwr(keyName, keyNameLen);
    unsigned long li = 0;
    for (int i = 0; i < Rounds;i++) {
        unsigned long s1 = GenerateSeed(masterKey,masterKeyLen);
        unsigned long s2 = GenerateSeed(keyName,keyNameLen);
        li += s1 + s2;
    }
    next = li;
    char *result = (char *)malloc(PassLength);
    int lastNum = 0;
    int lastspec = 0;
    int pos1 = dpg_rand(9);
    int pos2 = dpg_rand(9);
    for (int i = 0;i < PassLength;i++)
    {
        if (lastNum == pos1) {
            result[i] = num_table[dpg_rand(10)];
            lastNum = 0;
        }
        if (lastspec == pos2 && AlphaNumericOnly == 0) {
            result[i] = special_table[dpg_rand(24)];
            lastspec = 0;
        }
        else {
            result[i] = alpha_table[dpg_rand(52)];
            lastNum++;
            lastspec++;
        }
    }
    int width = 41;
    int length = (int)strlen(result) - 1;  // Discount the terminal '\0'
    int pad = (length >= width) ? 0 : (width - length) / 2;

    printf("\n\n");
    printf("============= YOUR PASSWORD =============\n");
    printf("                                         \n");
    printf("%*.*s%s\n", pad, pad, " ", result);
    printf("                                         \n");
    printf("=========================================\n");
    printf("\n\n");
    //printf("Password: %s\n",result);

    return 0;
}

void parseCLIArgs(int argc, const char * argv[])
{
    const char *a = argv[1];
    int i = 1;
    const char *next;
    while (i < argc) {
        if (strcmp(a,"-h") == 0) {
            ShowHelp();
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(a,"-p") == 0) {
            next = argv[i + 1];
            if (next == NULL)
            {
                printf("error: no password after -p, see -h for more information\n");
                exit(EXIT_FAILURE);
            }
            masterKeyLen = strlen(next);
            masterKey = malloc(masterKeyLen);
            strcpy(masterKey,next);
            i += 2;
            
        }
        else if (strcmp(a,"-n") == 0) {
            next = argv[i + 1];
            if (next == NULL)
            {
                printf("error: you must include a name to generate a unique password, see -h for more information\n");
                exit(EXIT_FAILURE);
            }
            keyNameLen = strlen(next);
            keyName = malloc(keyNameLen);
            strcpy(keyName,next);
            i += 2;
        }
        else if (strcmp(a,"-a") == 0) {
            if (AlphaNumericOnly == 1)
            {
                printf("warning: argument -a has already been set\n");
            }
            AlphaNumericOnly = 1;
            i++;
        }
        else if (strcmp(a,"-r") == 0) {
            next = argv[i + 1];
            if (next == NULL)
            {
                printf("error: you must have a number after -r, see -h for more information\n");
                exit(EXIT_FAILURE);
            }
            if (ParseInt(next, &Rounds) == 0)
            {
                printf("warning: %s was not a valid number, a single round will be used\n",next);
                Rounds = 1;
            }
            if (Rounds < 1)
            {
                printf("warning: a round must be equal to or greater than one, a single round will be used\n");
                Rounds = 1;
                i++;
            }
            i += 2;
        }
        else if (strcmp(a,"-l") == 0) {
            next = argv[i + 1];
            if (next == NULL)
            {
                printf("error: you must have a number after -l, see -h for more information\n");
                exit(EXIT_FAILURE);
            }
            if (ParseInt(next, &PassLength) == 0)
            {
                printf("warning: %s was not a valid number, the default length of %d will be used\n",next,DefaultPassLength);
                PassLength = DefaultPassLength;
            }
            if (PassLength == 0)
            {
                printf("error: cannot have a password length of zero, cannot continue\n");
                exit(EXIT_FAILURE);
                
            }
            if (PassLength < Min_Length)
            {
                printf("error: using a password equal or less than %d characters is not secure!\n",Min_Length);
                exit(EXIT_FAILURE);
            }
            i += 2;

        }
        else {
            printf("warning: %s is an unnkown command line unknown argument\n",a);
            i++;
        }
        a = argv[i];
    }
}

char *str2lwr(char *str,size_t len)
{
    char *ptr =  malloc(len + 1);
    memset(ptr, 0, len + 1);
    for (int i = 0;i < len;i++)
    {
        ptr[i] = tolower(str[i]);
    }
    return ptr;
}
int ParseInt(const char *s,int *i)
{
    char *ep;
    long l;

    l=strtol(s,&ep,0);

    if(*ep!=0)
        return 0;

    *i=(int)l;
    return 1;
 }

int dpg_rand(int max)
{
    next = next * (1103515245 + 97111);
    unsigned long ud = (unsigned long)(next/65536) % max;
    //printf("%lu %lu, %ld\n",next,ud,l);
    return (int)ud;
}


long GenerateSeed(char *data,unsigned long len)
{
    long hash1 = (6975 <<16) + 6975;
    long hash2 = hash1;
    for (int i = 0;i < len;i++)
    {
        char c = data[i];
        hash1 = ((hash1 << 5) + hash1) ^ c;
        if (c == 0)
            break;
        hash2 = ((hash2 << 5) + hash2) ^ c;
    }
    long r = hash1 + (hash2 * 1577836800);
    return r;
}


void ShowHelp()
{
    printf("%s\n%s\n\n",fullName,desc);
    printf("Commands:\n");
    printf("\t-h\t\t Shows this menu\n");
    printf("\t-p\t\t Sets the master password\n");
    printf("\t-a\t\t Only use alphanumeric values\n");
    printf("\t-n\t\t Name the password\n");
    printf("\t\t\t Changing the name alters the outcome\n");
    printf("\t\t\t and case is ignored\n");
    printf("\t-l <len> Length of the generated password\n");
    printf("\t\t\t The default password length is 8\n");
    printf("\t-r <len> Number of rounds to use.\n");
    printf("\t\t\t Default rounds is 1 and change the\n");
    printf("\t\t\t number of rounds will change \n");
    printf("\t\t\t the password generated\n");
    printf("\n\n");
}
