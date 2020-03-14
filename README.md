# DGP
Deterministically generate complex passwords


To Compile: 

I used QT Creator to compile DGP on MacOX and Windows without any major issues. You could compile this in <insert your favorite IDE> you will only need to add the required references for QT.

Again, if you want just the commandline version, you can compile it with the following command using GCC:

g++ -std=c++11 DGP-CLI.cpp dgp.cpp -o DGP-CLI

If you are on Windows you can use MinGW to compile DGP, however, I had no luck with getting it to work. 
DGP-CLI and DGP-Qt bith compile using MSVC2017 on windows 10 (using QT Creator). MSVC2019 is binary compatible with MSVC2017 should that *should* work as well.

DGP-CLI Commands:
	-h		 Shows the help menu
	-p		 Sets the master password
	-a		 Only use alphanumeric values
	-n		 Name the password (add quotes if spaces are used), Changing the name alters the outcome and case is ignored.
	-l <len> 	 Length of the generated password. The default password length is 8
	-r <len> 	 Number of rounds to use. Default rounds is 1 and change the number of rounds will change the password generated.
	-v <version> 	Select the version of the version of the algoritm you want to use.
			1 is the first version and
			2 is the version which has a bug fix



  Examples:
  
  ./DGP-CLI -p "My Super secret password goes here" -n "Github" -v 1
  
    Will result in:
    
    ============= YOUR PASSWORD =============
                                         
                  xGjsZg}QIUIU
                                         
    =========================================
    
The QT version should be pretty straight forward and has all the same features as the command-line interface.
    
If you find any bugs or want add something new, please do so. Any kind of contribution would be greatly appreciated.

