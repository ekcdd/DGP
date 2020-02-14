# DGP
Deterministically generate complex passwords


To Compile: 

Note: I have only attempted to compile this with GCC on unix, it *may* compile on Windows as-is.

gcc DGP.c -o DGP

Commands:
	-h		 Shows the help mnu
	-p		 Sets the master password
	-a		 Only use alphanumeric values
	-n		 Name the password (add quotes if spaces are used), Changing the name alters the outcome and case is ignored.
	-l <len> Length of the generated password. The default password length is 8
	-r <len> Number of rounds to use. Default rounds is 1 and change the number of rounds will change the password generated.
  
  
  Examples:
  
  ./DGP -p "My Super secret password goes here" -n "Github"
  
    Will result in:
    
    ============= YOUR PASSWORD =============
                                         
                  xGjsZg}QIUIU
                                         
    =========================================
    
    
If you find any bugs or want add something new, please do so. Any kind of contribution would be greatly appreciated.

