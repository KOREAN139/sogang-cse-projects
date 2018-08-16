--------------------
How To Compile & Run
--------------------

# make
# ./20151623.out

-----------------------
How To Delete Simulator
-----------------------

# make clean

----------------------
Command Line Arguments
----------------------

Example:  sicsim>hi

	assemble | bp [address / clear] | cat | cmp | copy | d[ir] | du[mp] | echo | e[dit] | f[ill] | head | h[elp] | hi[story] | loader [filename1] ... |
	opcode | opcodelist | progaddr [address] | q[uit] | reset | run | symbol | touch | type
  (required) Specifies the command that simulator should do

  	assemble filename
	- assemble given assembly - written file.
	  If given *.asm file does not make any error, *.obj ( object file ) and *.lst ( list file ) file will be generated.
	  If any error occurs, error message will be printed and terminates this function.
	bp [address / clear]
	- when no arguments has entered - print break point table
	  when address has entered - create break point at given address
	  when "clear" has entered - clear break point table
  	cat [filename(s)]
	- display file's contents. If filename does not entered, standard input will
	  be used for getting contents. If more than one filenames were given, shell will
	  display only 'exist' file's contents.
	* CAUTION *
	* 'cat' command classify filenames by 'INDENT' not 'comma'
	* If standard input is used for getting contents, 'ctrl+c' will let you get out of shell
	cmp filename1 filename2
	- compare two files. If two files are different, it will show you where the first
   	  difference is found at. If two files are same, nothing happens.
	* CAUTION *
	* 'cmp' command classify two filenames by 'INDENT' not 'comma'
	copy filename1 filename2
	- copy one file to other. When 'filename2' is already exists, this command will overlap that
	  file with 'filename1''s content.
	* CAUTION *
	* 'copy' command classify two filenames by 'INDENT' not 'comma'
        d[ir] 
	- shows items in current directory. Directory will be followed by '/', 
	  and executable file will be followed by '*'
        du[mp] [start, end] 
	- display memory. It allows set display range that you want to show
	  If you don't pass any parameters, simulator will only show 160 memory cells
	echo [message]
	- print what user entered. When no message have entered, empty line will be printed
	e[dit] address, value 
	- edit memory at (address) as (value)
	f[ill] start, end, value 
	- fill memory from (start) to (end) with (value)
	head lines filename
	- shows file's contents n-line. Lines will be counted from the very first of file.
	  If given n is bigger than total lines in file, this will show every contents
	  of file.
	* CAUTION *
	* 'head' command classify two arguments by 'INDENT' not 'comma'
	* this command will preceive given number as 'HEXADECIMAL'
	h[elp] 
	- display commands that you can use
	hi[story] 
	- display commands that successfully executed before
	loader [filename1] ...
	- link & load given object files (up to 3) in memory using linking loader
	opcode mnemonic 
	- let you know the opcode of given (mnemonic)
	opcodelist 
	- display instructions with <mnemonic, opcode> pair, in hash table shape
	progaddr [address]
	- set PROGADDR as given address
	q[uit] 
	- terminates simulator
	reset 
	- reset all memory cell as 0
	run
	- run program with given PROGADDR (or previously stopped address).
          If any break point exists, sicsim will stop execting program at break point.
	  else, execute to end of program.
	  after this, sicsim will display register values on screen.
	symbol
	- display SYMTAB on screen in lexicographical order.
	  format is shown below.
	  '\t''symbol''\t''LOCCTR'
	touch filename(s)
	- create new file(s) with 0-byte
	* CAUTION *
	* 'touch' command classify two filenames by 'INDENT' not 'comma'
	type filename
	- display file's contents on screen

--------------------------------
Files Includes With This Project
--------------------------------

	20151623.c		20151623.h		Makefile	opcode.txt		README		SP_2017_proj1_report.doc
	commands.c		assemble.c

-------------------------
Developer of This Program
-------------------------

	Sang gu Han, Undergraduate Computer Science and Engineering at Sogang Univ.
	20151623
