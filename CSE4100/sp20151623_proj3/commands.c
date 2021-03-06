#include"20151623.h"

int cmp(const void *a, const void *b) { return -strcmp(((Symbol *)a)->symbol, ((Symbol *)b)->symbol); }

int hashFunction(char *val){
	int ret = 0;
	while(*val){
		ret *= 39;
		ret += *val - 'A';
		val++;
	}	// converts to hash
	//	let each character represents number which how far from 'A'
	//	and let that string is base39 digit
	return ret % 20;
}

char* getCommand(char str[], int *commandNum){
	char *com, tmp;

	while(!IS_END_STRING(str) && IS_INDENT(str)) str++;
	// when this loop ends, str points the first character which is not indent or somethin

	for(com = str; !IS_END_STRING(str) && !IS_INDENT(str); str++);
	tmp = *str, *str = '\0';
	// set the very first character after first word (command) as NULL, com now points only command string

	switch(*com){
		case 'a' : *commandNum = strcmp(com, "assemble") ? -1 : 17;
				   break;
		case 'b' : *commandNum = strcmp(com, "bp") ? -1 : 22;
				   break;
		case 'c' : *commandNum = strcmp(com, "cat") ? strcmp(com, "cmp") ? strcmp(com, "copy") ? -1 : 12 : 11 : 10;
				   break;
		case 'd' : *commandNum = strcmp(com, "d") * strcmp(com, "dir") ? strcmp(com, "du") * strcmp(com, "dump") ? -1 : 4 : 1;
				   break;
		case 'e' : *commandNum = strcmp(com, "e") * strcmp(com, "edit") ?  strcmp(com, "echo") ? -1 : 15 : 5;
				   break;
		case 'f' : *commandNum = strcmp(com, "f") * strcmp(com, "fill") ? -1 : 6;
				   break;
		case 'h' : *commandNum = strcmp(com, "h") * strcmp(com, "help") ? strcmp(com, "hi") * strcmp(com, "history") ? strcmp(com, "head") ? -1 : 14 : 3 : 0;
				   break;
		case 'l' : *commandNum = strcmp(com, "loader") ? -1 : 20;
				   break;
		case 'o' : *commandNum = strcmp(com, "opcode") ? strcmp(com, "opcodelist") ? -1 : 9 : 8;
				   break;
		case 'p' : *commandNum = strcmp(com, "progaddr") ? -1 : 19;
				   break;
		case 'q' : *commandNum = strcmp(com, "q") * strcmp(com, "quit") ? -1 : 2;
				   break;
		case 'r' : *commandNum = strcmp(com, "reset") ? strcmp(com, "run") ? -1 : 21 : 7;
				   break;
		case 's' : *commandNum = strcmp(com, "symbol") ? -1 : 18;
				   break;
		case 't' : *commandNum = strcmp(com, "touch") ? strcmp(com, "type") ? -1 : 16 : 13;
				   break;
		default : *commandNum = -1;
				  break;
	}	// switch for given command is vaild or not

	*str = tmp;
	// restore given line

	while(!IS_END_STRING(str) && IS_INDENT(str)) str++;
	// when this loop ends, str points the first parameter or end of string	

	return str;
}

int hexstrToInt(char *str){	// for convert hex string to deciaml integer
	int ret = 0, len = 0;	// integer to store the result, length of str
	bool errFlag = FALSE, negFlag = FALSE;	// flag to check whether given string is headecimal or not, negative or not

	if(*str == '-') negFlag = TRUE, str++;
	
	while(!IS_END_STRING(str) && !IS_INDENT(str) && len < 6 && *str != ','){
		ret *= 16;
		if(*str < '0' || (*str > '9' && *str < 'A') || (*str > 'F' && *str < 'a') || *str > 'f'){
		   errFlag = TRUE;
		   break;
		}
		ret += *str <= '9' ? *str - '0' : *str <= 'Z' ? *str - 'A' + 10 : *str - 'a' + 10;
		str++; len++;
	}
	
	return len < 6 ? errFlag ? -1 : negFlag ? -3 : ret : -2;
	// -1 for invalid number, -2 for overflow ( over 0xFFFFF ), -3 for negative number
}

void addHistory(char com[]){
	History *newNode;
	newNode = (History *)malloc(sizeof(History));
	strcpy(newNode->command, com);
	newNode->link = NULL;	// set new node with given command
	
	if(!historyHead) historyHead = newNode, last = newNode;
	else last->link = newNode, last = newNode;	// add new node to list's last node
}

void commandHelp(char *tok, char com[]){
	if(!*tok){
		printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\nassemble filename\ntype filename\nsymbol\ncat [filename(s)]\ncmp filename1 filename2\ncopy filename1 filename2\ntouch filename(s)\nhead lines filename\necho [message]\n");	// print the list
		addHistory(com);	// add command to list
	} else puts("Invalid command");	// if any character that is not indent followed by command, it's invalid command
	return;
}

void commandDir(char *tok, char com[]){
	DIR *currDir;
	struct dirent *currFile;
	struct stat currStat;
	
	if(!*tok){
		if((currDir = opendir("."))){	// for read current directory. When opendir returns NULL, it means error occured.
			while((currFile = readdir(currDir))){	// when readdir returns NULL, that means it reaches to the end of directory
				stat(currFile->d_name, &currStat);
				printf("\t%s", currFile->d_name);
				if(S_ISDIR(currStat.st_mode)) putchar('/');	// when this item is directory
				else if(S_IXUSR & currStat.st_mode) putchar('*');	// when this item is executable
			}
			closedir(currDir);
			puts(""); // prints new line
		}
		addHistory(com);	// add command to list
	} else puts("Invalid command"); // if any character that is not indent followed by command, it's invalid command 
}

void commandQuit(char *tok, char com[]){
	if(!*tok) quitFlag = TRUE;
	else puts("Invalid command"); // if any character that is not indent followed by command, it's invalid command 
	return;
}

void commandHistory(char *tok, char com[]){
	int i = 0;	// for history index
	History *curr;	// for loop

	if(!*tok){
		addHistory(com);	// add command to list
		curr = historyHead;
		while(curr){	// while the history remains
			printf("%-5d %s\n", ++i, curr->command);	// print the history
			curr = curr->link;	// jump to next node
		}
	} else puts("Invalid command");	// if any character that is not indent followed by command, it's invalid command
	return;
}

void commandDump(char *tok, char com[]){
	int i, j, start, end; // loop variables, integer which stores start and end address each
	start = dumpLastAddr; end = 0;

	if(*tok){	// when parameters have entered after 'du' or 'dump'
		start = hexstrToInt(tok);

		if(start < 0 || *tok == ','){	// when error occurs
			if(start == -1 || *tok == ',') puts("Invalid start address has entered");
			else if(start == -2) puts("Please enter the address between 0x00000 through 0xFFFFF");
			else puts("Negative number has entered");
			return;
		}

		while(!IS_END_STRING(tok) && !IS_INDENT(tok) && *tok != ',') tok++;
		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		// now tok points the first character that is not indent right after first parameter, or end of string
		if(*tok == ','){	// when end exists
			tok++;	// for point character after ','
			while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
			// now tok points the first character of second parameter or end of string

			if(!*tok){	// when comma has entered but no parameter followed by
				puts("After comma need to enter end address");
				return;
			}

			end = hexstrToInt(tok);
		
			if(end < 0){	// when error occurs
				if(end == -1) puts("Invalid end address has entered");
				else if(end == -2) puts("Please enter the address between 0x00000 through 0xFFFFF");
				else puts("Negative number has entered");
				return;
			}

			if(start > end){	// when start is bigger than end
				puts("Start address value is bigger than end address value");
				return;
			}
		} else if(*tok){
 			puts("Need to use comma to classify two addresses");	// when two parameters have entered without comma
			return;
		} else dumpLastAddr = start;	// when only start exists
	}

	end = end ? end : start + 159 > 0xFFFFF ? 0xFFFFF : start + 159;	// set end address

	for(i = start / 16 * 16; i <= end / 16 * 16; i += 16){
		printf("%05X ", i);
		for(j = i; j < i + 16; j++){
			j >= start && j <= end ? printf("%02X ", virtualMem[j]) : printf("   ");
		}
		printf("; ");
		for(j = i; j < i + 16; j++){
			j >= start && j <= end && virtualMem[j] >= 0x20 && virtualMem[j] <= 0x7E ? putchar(virtualMem[j]) : putchar('.');
		}
		puts("");
	}	// print

	dumpLastAddr = (end + 1) % (1<<20);

	addHistory(com);	// add command to list
}

void commandEdit(char *tok, char com[]){
	int addr, val;	// stores address, value each
	if(!*tok){	// when only 'e' or 'edit' were given
		puts("Parameters required : no parameters have entered");
		return;
	} else{
		addr = hexstrToInt(tok);

		if(addr < 0 || *tok == ','){	// when error occurs
			if(addr == -1 || *tok == ',') puts("Invalid address has entered");
			else if(addr == -2) puts("Please enter the address between 0x00000 through 0xFFFFF");
			else puts("Negative number has entered");
			return;
		}

		while(!IS_END_STRING(tok) && !IS_INDENT(tok) && *tok != ',') tok++;
		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		// now tok points the first character that is not indent right after first parameter, or end of string
		if(*tok == ','){	// when value exists
			tok++;	// for point character after ','
			while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
			// now tok points the first character of second parameter or end of string

			if(!*tok){	// when comma has entered but no parameter followed by
				puts("After comma need to enter value");
				return;
			}

			val = hexstrToInt(tok);
		
			if(val < 0 || val > 0xFF){	// when error occurs
				if(val == -1) puts("Invalid value has entered");
				else if(val == -2 || val > 0xFF) puts("Please enter the value between 0x00 through 0xFF");
				else puts("Negative number has entered");
				return;
			}
		} else if(*tok){
 			puts("Need to use comma to classify two parameters");	// when two parameters have entered without comma
			return;
		} else{	// when only address exists
			puts("Please enter the value");
			return;
		}
	}
	virtualMem[addr] = val;
	addHistory(com);	// add command to list
}

void commandFill(char *tok, char com[]){
	int i, start, end, val;	// loop variable, stores start, end address and value each

	if(!*tok){	// when only 'f' or 'fill' were given
		puts("Parameters required : no parameters have entered");
		return;
	} else{
		start = hexstrToInt(tok);

		if(start < 0 || *tok == ','){	// when error occurs
			if(start == -1 || *tok == ',') puts("Invalid address has entered");
			else if(start == -2) puts("Please enter the address between 0x00000 through 0xFFFFF");
			else puts("Negative number has entered");
			return;
		}

		while(!IS_END_STRING(tok) && !IS_INDENT(tok) && *tok != ',') tok++;
		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		// now tok points the first character that is not indent right after first parameter, or end of string

		if(*tok == ','){	// when end address exists
			tok++;	// for point character after ','
			while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
			// now tok points the first character of second parameter or end of string

			if(!*tok){	// when comma has entered but no parameter followed by
				puts("After comma need to enter end address");
				return;
			}

			end = hexstrToInt(tok);

			if(end < 0){	// when error occurs
				if(end == -1) puts("Invalid value has entered");
				else if(end == -2) puts("Please enter the value between 0x00 through 0xFF");
				else puts("Negative number has entered");
				return;
			}

			if(start > end){	// when start is bigger than end
				puts("Start address value is bigger than end address value");
				return;
			}

			while(!IS_END_STRING(tok) && !IS_INDENT(tok) && *tok != ',') tok++;
			while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
			// now tok points the first character that is not indent right after second parameter, or end of string
			
			if(*tok == ','){	// when value exists
				tok++;
				while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
				// now tok points the first character of third parameter or end of string

				if(!*tok){	// when comma has entered but no parameter followed by
					puts("After comma need to enter value");
					return;
				}

				val = hexstrToInt(tok);

				if(val < 0 || val > 0xFF){	// when error occurs
					if(val == -1) puts("Invalid value has entered");
					else if(val == -2 || val > 0xFF) puts("Please enter the value between 0x00 through 0xFF");
					else puts("Negative number has entered");
					return;
				}

			} else if(*tok){
				puts("Need to use comma to classify end address and value");
				return;
			} else{
				puts("Please enter the value");
				return;
			}

		} else if(*tok){
			puts("Need to use comma to classify three parameters");	// when two parameters have entered without comma
			return;
		} else{	// when only start address exists
			puts("Please enter start address and value");
			return;
		}
	}
	for(i = start; i <= end; i++) virtualMem[i] = val;	// fill
	addHistory(com);	// add command to list
}

void commandReset(char *tok, char com[]){
	if(!*tok){
		memset(virtualMem, 0, sizeof(virtualMem));	// set every element in virtual memory 0
		addHistory(com);	// add command to list
	} else puts("Invalid command"); // if any character that is not indent followed by command, it's invalid command 
	return;
}

void commandMnemonic(char *tok, char com[]){
	int idx;	// stores index for given parameter
	char *it;	// iterator
	Instruction *curr;	// iterator
	if(!*tok) puts("No mnemonic has entered");
	else{
		for(it = tok; !IS_END_STRING(it) && !IS_INDENT(it); it++);
		*it = '\0';	// tokenize given mnemonic
		idx = hashFunction(tok);
		curr = instructionHead[idx];
		while(curr && strcmp(curr->mnemonic, tok)) curr = curr->link;
		// if given mnemonic exists in table, curr must point some node.
		if(!curr) puts("Invalid mnemonic");
		else{
			printf("opcode is %x\n", curr->opcode);
			addHistory(com);	// add command to list
		}
	}
}

void commandOplist(char *tok, char com[]){
	int i;	// loop variable
	Instruction *it;	// iterator
	if(!*tok){
		for(i = 0; i < 20; i+=puts("")){
			printf("%3d : ", i);
			it = instructionHead[i];
			while(it){
				printf("[%s,%x]", it->mnemonic, it->opcode);
				it = it->link;
				if(it) printf(" -> ");
			}
		}	// print the hash table
		addHistory(com);	// add command to list
	} else puts("Invalid command"); // if any character that is not indent followed by command, it's invalid command 
}

void commandType(char *tok, char com[]){
	FILE *fp;
	char fileName[111];
	int i, c;	// iterator, temporary character storage
	if(!*tok){
		puts("Filename required");
		return;
	} else{	// when file name has entered
		i = 0;
		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) fileName[i++] = *tok++;
		fileName[i] = '\0';
		// copy file name
		
		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		if(*tok){
			puts("Too many arguments");
			return;
		}	// when more than one arguments exists

		if((fp = fopen(fileName, "r"))){	// when file exists
			while(~(c = fgetc(fp))) putchar(c);
			fclose(fp);
		} else{
		   printf("type: %s: no such file\n", fileName);
		   return;
		}
		// when file does not exists
	}
	addHistory(com);	// add command to list
}

void commandAssemble(char *tok, char com[]){
	char fileName[111];
	int i, programLen;
	if(!*tok){
		puts("Filename required");
		return;
	} else{
		i = 0;
		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) fileName[i++] = *tok++;
		fileName[i] = '\0';
		//copy file name

		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		if(*tok){
			puts("Too many arguments");
			return;
		}	// when more than one arguments exists

		if((programLen = assemblerPass1(fileName, &programLen)) < 0 || assemblerPass2(fileName, programLen) < 0) return;
	}
	addHistory(com);
}

void commandSymbol(char *tok, char com[]){
	int i, idx;	// i for iterator, idx for array index
	Symbol *it, *arr;	// SYMTAB iterator, array
	if(*tok){	// when arguments exists
		puts("Command symbol does not need arguments");
		return;
	}
	if(!symtabSize){	// when SYMTAB is empty
		puts("SYMTAB has no symbol");
	} else{
		arr = (Symbol *)malloc(sizeof(Symbol) * symtabSize);	// array for sort symbols in lexicographical order
		for(i = idx = 0; i < 47; i++){
			it = symbolHead[i];
			while(it){	// copy from SYMTAB
				strcpy(arr[idx].symbol, it->symbol);
				arr[idx++].locCtr = it->locCtr;
				it = it->link;
			}
		}
		qsort(arr, symtabSize, sizeof(Symbol), cmp);	// sort
		for(i = 0; i < symtabSize; i++) printf("\t%s\t%X\n", arr[i].symbol, arr[i].locCtr);	// print
		free(arr);	// free
	}
	addHistory(com);	// add to history
}

void commandProgaddr(char *tok, char com[]){
	int addr;
	if(!*tok){	// when arguments does not exist
		puts("command progaddr needs argument");
		return;
	}

	addr = hexstrToInt(tok);

	if(addr < 0){	// when error occurs
	   if(addr == -1) puts("Invalid number has entered");
	   if(addr == -2) puts("Argument is too big for SIC ( over 0xFFFFF )");
	   if(addr == -3) puts("negative number has entered");
	   return;
	}	   

	progAddr = addr;	// set progaddr as given number

	addHistory(com);	// add to history
}

void commandLoader(char *tok, char com[]){
	int i, fileNum;
	char first[111], second[111], third[111];

	if(!*tok){	// when no file name has entered
		puts("command loader needs at least oneobject filename");
		return;
	}

	i = fileNum = 0;
	while(!IS_INDENT(tok) && !IS_END_STRING(tok)) first[i++] = *tok++;
	first[i] = '\0'; fileNum++;
	// copy file name
	
	while(IS_INDENT(tok) && !IS_END_STRING(tok)) tok++;
	if(*tok){	// when second file has entered
		if(!IS_ALPHABET(*tok)){	// when file name starts with non-alphabet character
			puts("Invaild file name has entered");
			return;
		}

		i = 0;
		while(!IS_INDENT(tok) && !IS_END_STRING(tok)) second[i++] = *tok++;
		second[i] = '\0'; fileNum++;
	}

	while(IS_INDENT(tok) && !IS_END_STRING(tok)) tok++;
	if(*tok){	// when third file has entered
		if(!IS_ALPHABET(*tok)){	// when file name starts with non-alphabet character
			puts("Invaild file name has entered");
			return;
		}

		i = 0;
		while(!IS_INDENT(tok) && !IS_END_STRING(tok)) third[i++] = *tok++;
		third[i] = '\0'; fileNum++;
	}

	if(fileNum == 1){	// when number of file is 1
		if(linkingLoaderPass1(fileNum, first) < 0 || linkingLoaderPass2(fileNum, first) < 0) return;
	}else if(fileNum == 2){	// when number of file is 2
		if(linkingLoaderPass1(fileNum, first, second) < 0 || linkingLoaderPass2(fileNum, first, second) < 0) return;
	}else{	// when number of file is 3
		if(linkingLoaderPass1(fileNum, first, second, third) < 0 || linkingLoaderPass2(fileNum, first, second, third) < 0) return;
	}
	// if error occured, do not add this command to history

	printLoadmap();
	// when link & load is done successfully, print load map

	addHistory(com);	// add to history
}

void commandRun(char *tok, char com[]){
	if(*tok){	// when arguments does exist
		puts("command run does not need argument");
		return;
	}
	run();
	addHistory(com);	// add to history
}

void commandBp(char *tok, char com[]){
	int i, point;
	char arg[111];
	if(!*tok){	// print break point table
		printf("breakpoint\n");
		printf("----------\n");
		for(i = 0; i < 1048576; i++) bptab[i] ? printf("%04X\n", i) : 0;
	} else{	// add break point or clear break point table
		i = 0;
		while(!IS_INDENT(tok) && !IS_END_STRING(tok)) arg[i++] = *tok++;
		arg[i] = '\0';

		while(IS_INDENT(tok) && !IS_END_STRING(tok)) tok++;
		if(!IS_END_STRING(tok)){
			puts("Too many arguments");
			return;
		}

		if(!strcmp(arg, "clear")) memset(bptab, 0, sizeof(bptab));	// clear
		else{	// create
			point = hexstrToInt(arg);
			if(point < 0){
				puts("Invalid argument");
				return;
			}
			if(bptab[point]) printf("At %06X break point already exists\n", point);
			bptab[point] = TRUE;
		}
	}
	addHistory(com);	// add to history
}

void commandCat(char *tok, char com[]){
	FILE *fp;
	char fileName[111];
	int i, c, catCounter = 0;	// iterator, temporary character storage, counts successfully done file
	if(!*tok) while(~(c = getchar())) putchar(c);
	// when no file name has entered, cat command uses stdin
	else{	// when file name has entered
		do{
			i = 0;
			while(!IS_END_STRING(tok) && !IS_INDENT(tok)) fileName[i++] = *tok++;
			fileName[i] = '\0';
			// copy file name
			if((fp = fopen(fileName, "r"))){	// when file exists
				while(~(c = fgetc(fp))) putchar(c);
				fclose(fp);
				++catCounter;
			} else printf("cat: %s: no such file\n", fileName);
			// when file does not exists
			while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
			// now tok points next file name's first character or end of string
		} while(*tok);
	}
	if(catCounter) addHistory(com);	// if there is any of given file done successfully, add command to list
}

void commandCmp(char *tok, char com[]){
	FILE *sfp, *dfp;
	char sourceName[111], destName[111];
	int i = 0, line, s, d;	// iterator, stores line number, temporary storage
	bool diffFlag = 0;	// flag that shows whether two files are different or not
	if(!*tok){	// when no file name has entered
		puts("File name required");
		return;
	} else{
		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) sourceName[i++] = *tok++;
		sourceName[i] = '\0';
		// copy first file name

		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		// now tok points second file name's first character of end of string

		if(!*tok){	// when there are no second file name
			puts("Second file name required");
			return;
		}

		i = 0;
		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) destName[i++] = *tok++;
		destName[i] = '\0';
		// copy second file name

		if((sfp = fopen(sourceName, "r")) && (dfp = fopen(destName, "r"))){	// when both file exists
			s = d = 0;	// initiallize storage
			for(i = line = 1; ~s && ~d; i++){
				s = fgetc(sfp); d = fgetc(dfp);
				if(s ^ d){	// when diffrence have found
					diffFlag = 1;
					break;
				}
				if(s == '\n') ++line, i = 0;	// when line changes
			}
			if(diffFlag) printf("%s %s differ : byte %d, Line %d\n", sourceName, destName, i, line);
		} else{	// when some of files does not exist
			puts("Invalid file name");
			return;
		}
	}
	addHistory(com);	// add command to list
}

void commandCopy(char *tok, char com[]){
	FILE *sfp, *dfp;
	char sourceName[111], destName[111];
	int i = 0, c;	// iterator, temporary storage
	if(!*tok){	// when no file name has entered
		puts("File name required");
		return;
	} else{
		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) sourceName[i++] = *tok++;
		sourceName[i] = '\0';
		// copy source file name

		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		// now tok points destiation file name's first character or end of string

		if(!*tok){	// when there are no destination file name
			puts("Destination file name required");
			return;
		}

		i = 0;
		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) destName[i++] = *tok++;
		destName[i] = '\0';
		// copy destination file name

		if((sfp = fopen(sourceName, "r"))){	// when source file exists
			if(!(dfp = fopen(destName, "w"))){	// when failed to open destination file
				puts("Failed to make / access file");
				return;
			}
			while(~(c = fgetc(sfp))){
			   if(fputc(c, dfp) == EOF){
				   puts("File writing error occured");
				   return;
			   }
			}	// copy
			fclose(sfp); fclose(dfp);
		} else{	// when file does not exist
			printf("copy: %s: no such file\n", sourceName);
			return;
		}
	}
	addHistory(com);	// add command to list
}

void commandTouch(char *tok, char com[]){
	FILE *fp;
	char fileName[111];
	int i = 0;	// iterator
	if(!*tok){
		puts("File name required");
		return;
	} else{
		do{
			i = 0;
			while(!IS_END_STRING(tok) && !IS_INDENT(tok)) fileName[i++] = *tok++;
			fileName[i] = '\0';
			// copy given file name
			if((fp = fopen(fileName, "a"))){
				fclose(fp);
				// create 0-byte new file or touch already created file
			} else puts("File create / access error");
			while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
			// now tok points next file name's first character or end of string
		} while(*tok);
	}
	addHistory(com);
}

void commandHead(char *tok, char com[]){
	FILE *fp;
	char fileName[111];
	int line, i = 0, c;	// for store given line, iterator
	if(!*tok){	// when no parameters are entered
		puts("No parameters had entered");
		return;
	} else{
		line = hexstrToInt(tok);
		if(line < 0){	// given number is not hex
			puts("Please enter correct hexadecimal");
			return;
		}

		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) tok++;
		while(!IS_END_STRING(tok) && IS_INDENT(tok)) tok++;
		// now tok points the first character of file name or end of string

		if(!*tok){	// when there are no more parameter left
			puts("Please enter file name");
			return;
		}

		while(!IS_END_STRING(tok) && !IS_INDENT(tok)) fileName[i++] = *tok++;
		fileName[i] = '\0';
		// copy file name

		if((fp = fopen(fileName, "r"))){
			while(~(c = fgetc(fp)) && line){
				putchar(c);
				if(c == '\n') --line;
			}
			fclose(fp);
		} else{
			printf("head: %s: no such file\n", fileName);
			return;
		}
	}
	addHistory(com);
}

void commandEcho(char *tok, char com[]){
	char *it;
	it = com;
	while(!IS_END_STRING(it) && IS_INDENT(it)) it++;
	while(!IS_END_STRING(it) && !IS_INDENT(it)) it++;
	// now it points right after the command echo

	if(*it) it++;
	printf("%s\n", it);
	addHistory(com);
}

void loadInstruction(){
	FILE *fp;	// for reading opcode.txt file
	char mnemonic[6], formats[4];	// for store mnemonic and given formats
	int opcode, idx;	// for store opcode, index
	Instruction *newNode, *it;	// for allocate new nodes, and iterator
	fp = fopen("opcode.txt", "r");
	while(~fscanf(fp, "%x %s %s", &opcode, mnemonic, formats)){	// until file pointer reaches EOF
		newNode = (Instruction *)malloc(sizeof(Instruction));
		newNode->opcode = opcode;
		strcpy(newNode->mnemonic, mnemonic);
		strcpy(newNode->format, formats);
		newNode->link = NULL;	// set new node with inputs from file
		idx = hashFunction(mnemonic);	// get hash through hash function
		if(instructionHead[idx]){
			it = instructionHead[idx];
			while(it->link) it = it->link;
			it->link = newNode;
		} else instructionHead[idx] = newNode;	// link to table
	}
	fclose(fp);
	return;
}
