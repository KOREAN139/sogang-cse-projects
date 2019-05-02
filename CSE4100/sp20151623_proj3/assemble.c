#include"20151623.h"

#define SET_ERRORFLAG errorFlag = TRUE; break;

int strToDecimal(char *val){
	int ret = 0, neg = 1;
	if(*val == '-'){
		neg = -1;
		val++;
	}
	while(*val){
		if(!IS_DECIMAL(*val)){
			ret = 1 << 30;
			neg = 1;
			break;
		}
		ret *= 10;
		ret += *val++ - '0';
	}
	return ret * neg;	// return 1 << 30 if given string is not decimal, or given decimal value if not
}

int symtabHashFuction(char *val){
	int ret = 0;
	while(*val){
		ret *= 19;
		ret += *val - (*val >= 'a' ? 'a'-36 : *val >= 'A' ? 'A'-10 : '0');
		ret = (ret + 47) % 47;
		val++;
	}	// converts to hash
	//	let each character represents number which how far from 'A' (let's define 'A'-'0' = 10, 'a'-'A' = 26)
	//	and let that string is base 19 digit, modulo 47
	return ret;
}

int assemblerPass1(char fileName[], int *programLen){
	int locCtr, startAddr, insLen, c, cntLine, cntChar, cntGap, i, idx, prefix, val, constMode, neg, firstFlag, firstAddr;
	// for store LOCCTR, start address, bytes to increase LOCCTR, temporary variable
	// counter for line, characters, characters between strings, index, hash-index, prefix if any, value if any
	// constMode stores 1 if constant is string, 2 if constant is hexadecimal ( in BYTE ), stores 1 if "RESB", 3 if "RESW"
	// neg stores 1 if val is nonnegative, -1 or not, firstFlag stores (instruction line - 1), firstAddr stores first executable instruction's address
	char label[33], opcode[33], operand[33];
	FILE *asmPointer, *intPointer, *locPointer;	// file pointer points aseembly file, intermediate file, location file each
	bool errorFlag;	// when errorFlag stores 1 (TRUE), ksll the function
	Instruction *opIt;	// iterate around OPTAB
	Symbol *symPtr, *newSymbol;

	if(!(asmPointer = fopen(fileName, "r"))){	// when file does not exist
		printf("assemble: %s: no such file\n", fileName);
		return -1;	// return error value
	}
	if(!(intPointer = fopen("intermediate", "w"))){	// when failed to make intermediate file
		puts("assemble: Failed to create intermediate file");
		fclose(asmPointer);
		return -1;
	}
	if(!(locPointer = fopen("location", "w"))){	// when failed to make intermediate file
		puts("assemble: Faile to create location file");
		fclose(asmPointer); fclose(intPointer);
		return -1;
	}
	symtabSize = startAddr = locCtr = cntLine = cntChar = c = 0; 
	errorFlag = FALSE; firstFlag = firstAddr = 0;
	for(i = 0; i < 47; i++){
		while(symbolHead[i]){
			symPtr = symbolHead[i];
			symbolHead[i] = symbolHead[i]->link;
			free(symPtr);
		}
	}
	// initialize part
	
	while(~c && ~(c = fgetc(asmPointer)) && !errorFlag){
		++cntLine; cntChar = 1; neg = insLen = prefix = val = 0;
		*opcode = *label = *operand = '\0';

		fprintf(intPointer, "%4d\t", cntLine * 5);

		// check if this line is comment
		if(IS_COMMENT(c)){
			while(~c && !IS_END_STRING(&c)){
				fprintf(intPointer, "%c", c);
				c = fgetc(asmPointer);
			}
			fprintf(intPointer, "\n");
			continue;
		}	// if this line is comment, skip this line

		if(IS_DECIMAL(c)){	// when label's first character is number
			printf("%s:%d:%d: error: Label's first character cannot be number\n", fileName, cntLine, cntChar);
			SET_ERRORFLAG
		}

		// check whether label exists or not
		if(IS_ALPHABET(c)){	// when label exists
			i = cntGap = 0;
			while(~c && !IS_DELIMITER(c)){
				label[i++] = c;
				c = fgetc(asmPointer);
				++cntGap;
				if(!IS_DELIMITER(c) && !IS_ALPHABET(c) && !IS_DECIMAL(c)){	// when there exists non-alphabet and non-numeric character
					printf("%s:%d:%d: error: Label cannot have character '%c' (non-alphabet, non-numeric)\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}
			}
			label[i] = '\0';	// set NULL character at the end of string

			idx = symtabHashFuction(label);
			symPtr = symbolHead[idx];
			while(symPtr && strcmp(label, symPtr->symbol)) symPtr = symPtr->link;
			if(symPtr){
				printf("%s:%d:%d: error: Symbol '%s' has already used before\n", fileName, cntLine, cntChar, label);
				SET_ERRORFLAG
			}	// if symbol already exists in SYMTAB

			cntChar += cntGap;
		}

		if(*label) fprintf(intPointer, "%s\t", label);
		else fprintf(intPointer, "!\t");

		while(!IS_END_STRING(&c) && IS_INDENT(&c)) c = fgetc(asmPointer), ++cntChar;	// skips indent
		if(!*label && IS_END_STRING(&c)) continue;	// when given line is blank

		firstFlag += 1;

		// check given format is valid
		if(!IS_ALPHABET(c) && !IS_VALID_PREFIX(c)){	// if something that is not alphabet shown before opcode, it's error
			printf("%s:%d:%d: error: Unexpected character %c appeared\n", fileName, cntLine, cntChar, c);
			SET_ERRORFLAG
		}
		
		if(IS_VALID_PREFIX(c)){	// when prefix exists, save that prefix and let c store first character of opcode
			prefix = c == '+';
			if(!prefix){	// when '#', '@' comes before opcode
				printf("%s:%d:%d: error: Prefix '%c' cannot be used here\n", fileName, cntLine, cntChar, c);
				SET_ERRORFLAG
			}
			fprintf(intPointer, "%c", c);
			c = fgetc(asmPointer);
		}

		// check whether opcode is vaild or not
		i = 0; cntGap = prefix;
		while(IS_ALPHABET(c)){	// get given opcode
			opcode[i++] = c;
			c = fgetc(asmPointer);
			++cntGap;
		}
		opcode[i] = '\0';	// set NULL character at the end of string

		while(!IS_END_STRING(&c) && IS_INDENT(&c)) c = fgetc(asmPointer), ++cntGap;	// skips indent

		fprintf(intPointer, "%s", opcode);

		if(!strcmp("START", opcode) || !strcmp("END", opcode) || !strcmp("BASE", opcode) || !strcmp("BYTE", opcode) || !strcmp("WORD", opcode) || !strcmp("RESB", opcode) || !strcmp("RESW", opcode)){	// if opcode is directive
			if(prefix){	// when directive has prefix
				printf("%s:%d:%d: error: Directive do not support prefix '%c'\n", fileName, cntLine, cntChar, '+');
				SET_ERRORFLAG
			}
			--firstFlag;
			// do proper action for given directive
			if(!strcmp("START", opcode)){
				if(firstFlag){	// when START appears after first instruction
					printf("%s:%d:%d: error: 'START' directive must be used at very first instruction\n", fileName, cntLine, cntChar);
					SET_ERRORFLAG
				}
				if(IS_END_STRING(&c)){	// when no operands entered
					printf("%s:%d:%d: error: No starting address\n", fileName, cntLine, cntChar+cntGap);
					SET_ERRORFLAG
				}
				i = 0;
				while(~c && !IS_DELIMITER(c)){
					operand[i++] = c;
					c = fgetc(asmPointer);
				}
				operand[i] = '\0';
				startAddr = hexstrToInt(operand);
				if(startAddr < 0){	// when given operand is not valid
					printf("%s:%d:%d: error: ", fileName, cntLine, cntChar+cntGap);
					puts(startAddr == -1 ? "Invalid value (not hexadecimal)" : startAddr == -2 ? "Operand has too big value" : "Negative Address has entered");
					SET_ERRORFLAG
				}
				locCtr = startAddr;	// set LOCCTR as startAddr
				while(!IS_END_STRING(&c) && IS_INDENT(&c)) c = fgetc(asmPointer), ++cntGap;	// skip indent
				if(!IS_END_STRING(&c)){	// when there are any character after operand
					printf("%s:%d:%d: error: Too many arguments\n", fileName, cntLine, cntChar+cntGap);
					SET_ERRORFLAG
				}
				insLen = 0;	// START directive does not make LOCCTR increase
				fprintf(intPointer, "\t%s", operand);
			}
			if(!strcmp("END", opcode)){
				if(IS_ALPHABET(c)){
					i = 0;
					while(~c && !IS_DELIMITER(c)){
						label[i++] = c;
						c = fgetc(asmPointer);
					}
					label[i] = '\0';
					while(!IS_END_STRING(&c) && IS_INDENT(&c)) c = fgetc(asmPointer), ++cntGap;	// skip indent
					if(!IS_END_STRING(&c)){	// when there are any character after operand
						printf("%s:%d:%d: error: Too many arguments\n", fileName, cntLine, cntChar+cntGap);
						SET_ERRORFLAG
					}
					idx = symtabHashFuction(label);
					symPtr = symbolHead[idx];
					while(symPtr && strcmp(label, symPtr->symbol)) symPtr = symPtr->link;
					if(!symPtr){	// when given symbol does not exist in SYMTAB
						printf("%s:%d:%d: error: Symbol '%s' does not exist\n", fileName, cntLine, cntChar+cntGap, label);
						SET_ERRORFLAG
					}
					if(symPtr->locCtr != firstAddr){	// when given label is not point the first instruction
						printf("%s:%d:%d: error: Label '%s' does not point first executable instruction \n", fileName, cntLine, cntChar+cntGap, label);
						SET_ERRORFLAG
					}
					fprintf(intPointer, "\t%s", label);
				} else if(!IS_END_STRING(&c)){	// when given label does not start with alphabet
					printf("%s:%d:%d: error: Label cannot start with '%c'\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}
				*programLen = locCtr - startAddr;
				*label = '\0';
			}
			if(!strcmp("BASE", opcode)){
				if(IS_END_STRING(&c)){	// when there is no operand
					printf("%s:%d:%d: error: Base directive needs operand\n", fileName, cntLine, cntChar+cntGap);
					SET_ERRORFLAG
				}
				if(!IS_ALPHABET(c)){	// when given label does not start with alphabet
					printf("%s:%d:%d: error: Label cannot start with '%c'\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}

				fprintf(intPointer, "\t");
				i = 0;
				while(~c && !IS_DELIMITER(c)){	// get operand
					fprintf(intPointer, "%c", c);
					c = fgetc(asmPointer);
					++cntGap;
				}

				while(~c && IS_INDENT(&c)) c = fgetc(asmPointer), ++cntGap;	// skips indent
				if(!IS_END_STRING(&c)){	// when there is any character after operand
					printf("%s:%d:%d: error: Unexpected character '%c' appreared\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}
			}
			if(!strcmp("BYTE", opcode)){
				if(c != 'C' && c != 'X'){	// when constant is not valid
					printf("%s:%d:%d: error: Invalid constant '%c' used\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}
				fprintf(intPointer, "\t%c", c);
				constMode = c == 'C' ? 1 : 2;
				if((c = fgetc(asmPointer)) != '\''){	// when format is invalid
					printf("%s:%d:%d: error: Invalid format for generating constant\n", fileName, cntLine, cntChar+++cntGap);
					SET_ERRORFLAG
				}
				fprintf(intPointer, "%c", c);
				c = fgetc(asmPointer); cntGap += 2; i = 0;
				while(!IS_END_STRING(&c) && c != '\''){
					fprintf(intPointer, "%c", c);
					if(constMode & 2 && !IS_DECIMAL(c) && (c|32) < 'a' && (c|32) > 'f'){
						printf("%s:%d:%d: error: '%c' is not hexadecimal value\n", fileName, cntLine, cntChar+cntGap, c);
						SET_ERRORFLAG
					}	// when constant is hexadecimal but non-hexadeciaml value has given
					++i; ++cntGap;
					c = fgetc(asmPointer);
				}
				if(c != '\''){	// when string ends before ''' appears
					printf("%s:%d:%d: error: Invalid match for '''\n", fileName, cntLine, cntChar+cntGap);
					SET_ERRORFLAG
				}
				fprintf(intPointer, "%c", c);
				insLen = constMode & 2 ? (i+1)/constMode : i;
			}
			if(!strcmp("WORD", opcode)){
				insLen = 3;	// word == 3bytes ( in SIC/XE )
				neg = 1;
				if(c == '-') neg = -1, c = fgetc(asmPointer);	// when negative value has entered
				while(!IS_END_STRING(&c) && IS_DECIMAL(c) && val < 1<<24){	// get operand
					val *= 10;
					val += c - '0';
					c = fgetc(asmPointer);
					++cntGap;
				}
				if(~c && !IS_DELIMITER(c)){	// when there exists non-decimal character in operand
					printf("%s:%d:%d: error: '%c' is not decimal\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}
				val *= neg;	// give value proper domain
				if(val > (1<<23)-1 || val < -(1<<23)){	// when overflow occurs
					while(val) val /= 10, --cntGap;
					printf("%s:%d:%d: error: Operand is too big to store in a word\n", fileName, cntLine, cntChar+cntGap);
					SET_ERRORFLAG
				}
				fprintf(intPointer, "\t%d", val);
			}
			if(!strcmp("RESB", opcode) || !strcmp("RESW", opcode)){
				constMode = strcmp("RESB", opcode) ? 3 : 1;	// stores 1 when opcode is "RESB", and stores 3 when opcode is "RESW"
				while(!IS_END_STRING(&c) && IS_DECIMAL(c) && val < 1<<20){	// get operand
					val *= 10;
					val += c - '0';
					c = fgetc(asmPointer);
					++cntGap;
				}
				if(val >= 1<<20){	// when overflow occurs
					while(val) val /= 10, --cntGap;
					printf("%s:%d:%d: error: Operand is too big\n", fileName, cntLine, cntChar+cntGap);
					SET_ERRORFLAG
				}
				if(~c && !IS_DELIMITER(c)){	// when there exists non-decimal character in operand
					printf("%s:%d:%d: error: '%c' is not decimal\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}
				fprintf(intPointer, "\t%d", val);
				insLen = constMode * val;
			}
		} else{	// if opcode is not directive
			idx = hashFunction(opcode);
			opIt = instructionHead[idx];
			while(opIt && strcmp(opIt->mnemonic, opcode)) opIt = opIt->link;
			if(!opIt){	// when given opcode does not exist in OPTAB
				printf("%s:%d:%d: error: %s is invalid opcode\n", fileName, cntLine, cntChar+prefix, opcode);
				SET_ERRORFLAG
			}
			insLen = opIt->format[0] - '0';
			if(insLen < 3 && prefix){	// when there exist prefix where it should not
				printf("%s:%d:%d: error: Inapproapriate prefix '%c' exists\n", fileName, cntLine, cntChar, '+');
				SET_ERRORFLAG
			} else insLen += prefix;
			
			prefix = 0;
			if(IS_VALID_PREFIX(c)){	// when '#' or '@' exists
				if(c == '+'){	// when there exist prefix where it should not
					printf("%s:%d:%d: error: Inapproapriate prefix '%c' exists\n", fileName, cntLine, cntChar, '+');
					SET_ERRORFLAG
				}
				prefix = c;
				c = fgetc(asmPointer);
				++cntGap;
			}

			i = 0;
			while(!IS_END_STRING(&c) && !IS_DELIMITER(c)){
				if(!IS_ALPHABET(c) && !IS_DECIMAL(c)){	// when non-alphabet and non-decimal character exists
					printf("%s:%d:%d: error: '%c' is not allowed for operand\n", fileName, cntLine, cntChar+cntGap, c);
					SET_ERRORFLAG
				}
				operand[i++] = c;
				c = fgetc(asmPointer);
				++cntGap;
			}
			operand[i] = '\0';
			
			if(prefix && !*operand){	// when only prefix exists
				printf("%s:%d:%d: error: Operand should be followed after prefix\n", fileName, cntLine, cntChar+cntGap);
				SET_ERRORFLAG
			}

			fprintf(intPointer, "\t");
			if(prefix) fprintf(intPointer, "%c", prefix);
			fprintf(intPointer, "%s", operand);

			while(!IS_END_STRING(&c) && IS_INDENT(&c)) c = fgetc(asmPointer), ++cntGap;	// skip indent

			if(!IS_END_STRING(&c)){
				if(c != ','){	// when other operand exists but no delimiter exists
					printf("%s:%d:%d: error: operands should be classified by ','\n", fileName, cntLine, cntChar+cntGap);
					SET_ERRORFLAG
				}

				while(!IS_END_STRING(&c) && IS_DELIMITER(c)) c = fgetc(asmPointer), ++cntGap;	// let c store first character of operand

				i = 0;
				while(!IS_END_STRING(&c) && !IS_DELIMITER(c)){
					if(!IS_ALPHABET(c) && IS_DECIMAL(c)){	// when non-alphabet and non-decimal character exists
						printf("%s:%d:%d: error: '%c' is not allowed for operand\n", fileName, cntLine, cntChar+cntGap, c);
						SET_ERRORFLAG
					}
					operand[i++] = c;
					c = fgetc(asmPointer);
					++cntGap;
				}
				operand[i] = '\0';

				fprintf(intPointer, "\t%s", operand);
			}
		}
		while(!IS_END_STRING(&c)) c = fgetc(asmPointer), ++cntGap;	// skip the last part of this line ( this part will be handled by pass2 algorithm )
		cntChar += cntGap;
		fprintf(intPointer, "\n");

		// if all instruction is fine and symbol exists, put symbol into SYMTAB
		if(*label){
			// get hash-index
			idx = symtabHashFuction(label);

			// increase symtabSize
			++symtabSize;

			//	allocate new node for SYMTAB
			newSymbol = (Symbol *)malloc(sizeof(Symbol));
			newSymbol->locCtr = locCtr;
			strcpy(newSymbol->symbol, label);
			newSymbol->link = NULL;

			// insert symbol in SYMTAB
			if(!symbolHead[idx]) symbolHead[idx] = newSymbol;
			else{
				symPtr = symbolHead[idx];
				while(symPtr->link) symPtr = symPtr->link;
				symPtr->link = newSymbol;
			}
		}
		if(!firstAddr && firstFlag == 1) firstAddr = locCtr;
		fprintf(locPointer, "%4d\t%X\t", cntLine * 5, locCtr);	// save LOCCTR for current line
		locCtr += insLen;	// increase LOCCTR properly
		fprintf(locPointer, "%X\n", locCtr);	// save PC for current line
	}

	if(errorFlag){	// if any error occurs, SYMTAB need to be cleared
		for(i = 0; i < 47; i++){
			while(symbolHead[i]){
				symPtr = symbolHead[i];
				symbolHead[i] = symbolHead[i]->link;
				--symtabSize;
				free(symPtr);
			}
		}
		symtabSize = 0;
	}

	fclose(asmPointer), fclose(intPointer), fclose(locPointer);
	return errorFlag ? -1 : *programLen;
}

int assemblerPass2(char fileName[], int programLen){
	int i, idx, locCtr, c, intLine, locLine, prefix, opAddr, constVal, cntObj, firstAddr;
	// i for iterator, hash-index, LOCCTR, temporary variable, line number from imediate, location file each, store proper integer for prefix if any, object code, constant values, count text record's length, first executable address
	int modi[111], idxModi;	// stores location which needs to be relocated
	bool errorFlag, idxFlag, resFlag;	// stores that error occured or not, flag that indicates this instruction is indexed addressing or not, flag that last recorded instruction is 'RESB' or 'RESW'
	char fName[111], line[111], label[33], opcode[33], first[33], second[33], obj[77], *it;
	// stores file name without file name extention, gets line from intermediate file, charater type array for store label, opcode, operands each
	// it for iterator
	char regs[][3] = {"A", "X", "L", "B", "S", "T", "F", "", "PC", "SW"};	// regsisters
	FILE *intPointer, *locPointer, *objPointer, *lstPointer;	// file pointers
	reg24 B, PC;	// stores B, PC
	Symbol *symPtr;	// SYMTAB iterator
	Instruction *opIt;	// OPTAB iterator

	if(!(intPointer = fopen("intermediate", "r"))){
		puts("Cannot find intermediate file");
		return -1;
	}
	if(!(locPointer = fopen("location", "r"))){
		puts("Cannot find location file");
		fclose(intPointer);
		return -1;
	}
	// open intermediate, location file

	strcpy(fName, fileName);
	for(i = 0; fName[i] != '.'; i++);
	fName[i] = '\0';
	// get file name without extention from file name

	if(!(objPointer = fopen("tmpobj", "w"))){
		puts("File open error");
		fclose(intPointer), fclose(locPointer);
		return -1;
	}
	if(!(lstPointer = fopen("tmplst", "w"))){
		puts("File open error");
		fclose(intPointer), fclose(locPointer), fclose(objPointer);
		return -1;
	}
	// open temporary list, object file

	errorFlag = resFlag = FALSE; cntObj = B = idxModi = 0; locLine = -1;

	while(~fscanf(intPointer, "%d\t", &intLine)){
		opAddr = prefix = 0;
		idxFlag = FALSE;
		*label = '\0';

		fscanf(intPointer, "%s", label);
		fgets(line, 100, intPointer);
		// read from intermediate file
		if(locLine < intLine) fscanf(locPointer, "%d%X%X", &locLine, &locCtr, &PC);
		// if it is excutable line, read LOCCTR and PC from location file

		if(!cntObj){
			*obj = 'T';
			sprintf(obj+1, "%06X", locCtr);	// write starting address for object code in this record
			cntObj = 9;
			resFlag = FALSE;
		}
		
		fprintf(lstPointer, "%4d\t", intLine);
		if(*label == '.'){	// when this line is comment
			fprintf(lstPointer, "\t");
			if(!IS_END_STRING(&line[1])) fprintf(lstPointer, "  %s", line);
			else fprintf(lstPointer, "\n");
			continue;
		} 
		
		it = line + 1; i = 0;
		if(IS_VALID_PREFIX(*it)){	// when prefix exists
			prefix |= *it == '+' ? 4 : 0;
			it++;
		}
		while(!IS_END_STRING(it) && !IS_INDENT(it)) opcode[i++] = *it++;
		opcode[i] = '\0'; it++;
		// get opcode from imediate  file
		//sarangsarangS2

		idx = hashFunction(opcode);
		opIt = instructionHead[idx];
		while(opIt && strcmp(opIt->mnemonic, opcode)) opIt = opIt->link;
 		// find opcode in OPTAB, if it does not exist in OPTAB, it means opcode stores directive

		if(opIt){	// when opcode stores opcode
			// get symbol and fine in SYMTAB
			fprintf(lstPointer, "%04X\t", locCtr);
			if(*label == '!') fprintf(lstPointer, "\t");
			else fprintf(lstPointer, "%s\t", label);
			if(prefix & 4) fprintf(lstPointer, "+");
			fprintf(lstPointer, "%s\t", opcode);
			
			opAddr = (opIt->opcode) << ((opIt->format[0] - '1') * 8 + (prefix & 4) * 2);	// set opcode

			if(opIt->format[0] == '3'){	// when this instruction froms format 3
				if(IS_VALID_PREFIX(*it)) prefix |= (*it++ == '#') ? 1 : 2;
				else prefix |= 3;	// 1 for immediate, 2 for indirect, 3 for simple ( n, i bit as lsb )
				opAddr |= (prefix & 3) << (16 + (prefix & 4) * 2);	// set n, i bit
				opAddr |= (prefix & 4) << 18;	// set e bit
			} else{	// when format 1 or format 2 have prefix on operand
				if(IS_VALID_PREFIX(*it)){
					printf("%s:%d: error: Format %c should not have prefix\n", fileName, intLine, opIt->format[0]);
					SET_ERRORFLAG
				}
			}
			
			if(opIt->format[0] == '1'){
				if(!IS_END_STRING(it)){	// when this instruction is format 1 but operand exists
					printf("%s:%d: error: Opcode '%s' does not need operand(s)\n", fileName, intLine, opIt->mnemonic);
					SET_ERRORFLAG
				}
			} else{
				for(i = 0; !IS_END_STRING(it) && !IS_INDENT(it); first[i++] = *it++);
				first[i] = '\0';
				if(!IS_END_STRING(it)) it++;
				// get first operand if any

				idx = symtabHashFuction(first);
				symPtr = symbolHead[idx];
				while(symPtr && strcmp(symPtr->symbol, first)) symPtr = symPtr->link;
				if(*first && !symPtr){	// when given symbol does not exist in SYMTAB
					for(i = 0; i < 10 && strcmp(first, regs[i]); i++);	// if first stores register, i must be less than 10
					constVal = strToDecimal(first);
					if((prefix & 3) == 1 && constVal != (1<<30)){	// for immediate addressing ( '#' + decimal )
						if(constVal >= 1 << 20){	// when overflow occurs
							printf("%s:%d: error: Operand has too big value\n", fileName, intLine);
							SET_ERRORFLAG
						} else if(constVal >= 1<<12){	// when instruction needs format 4
							if(prefix & 4) opAddr |= constVal;
							else{	// when prefix '+' is not uesd
								printf("%s:%d: error: Prefix '+' need to be used\n", fileName, intLine);
								SET_ERRORFLAG
							}
						} else if(constVal >= 0) opAddr |= constVal;
						else{	// when negative constant has entered
							printf("%s:%d: error: Negative decimal\n", fileName, intLine);
							SET_ERRORFLAG
						}
						if(!IS_END_STRING(it)){	// when there exists any operand after ('#' + deciaml)
							printf("%s:%d: error: Too many arguments\n", fileName, intLine);
							SET_ERRORFLAG
						}
					} else if(i > 9){	// for undefined symbol
						printf("%s:%d: error: Undefined symbol '%s' is used\n", fileName, intLine, first);
						SET_ERRORFLAG
					}
				}

				for(i = 0; !IS_END_STRING(it) && !IS_INDENT(it); second[i++] = *it++);
				second[i] = '\0'; it++;
				// get second operand if any
				
				if(opIt->format[0] == '2'){
					if(!*first){	// when no operands has entered
						printf("%s:%d: error: No operands\n", fileName, intLine);
						SET_ERRORFLAG
					}
					for(i = 0; i < 10 && strcmp(regs[i], first); i++);
					if(i > 9){	// when invalid register has entered
					   printf("%s:%d: error: '%s' is invalid register\n", fileName, intLine, first);
					   SET_ERRORFLAG
					}
					opAddr |= i << 4;	// set r1
					fprintf(lstPointer, "%s", first);
					if(*second){	// when second register has entered
						for(i = 0; i < 10 && strcmp(regs[i], second); i++);
						if(i > 9){	// when invalid register has entered
							printf("%s:%d: error: '%s' is invalid register\n", fileName, intLine, second);
							SET_ERRORFLAG
						}
						opAddr |= i;	// set r1
						fprintf(lstPointer, ",%s\t\t", second);
					} else fprintf(lstPointer, "\t\t");
				} else{
					if(*first){
						if((prefix & 3) < 3) fprintf(lstPointer, "%c", (prefix & 3) == 2 ? '@' : '#');
						fprintf(lstPointer, "%s", first);
						if(*second){
							if(strcmp("X", second)){	// when second operand exist that is not 'X'
								printf("%s:%d: error: Too many arguments\n", fileName, intLine);
								SET_ERRORFLAG
							} else opAddr |= 1 << (15 + (prefix & 4) * 2);	// set x bit
							fprintf(lstPointer, ",%s\t", second);
						} else fprintf(lstPointer, "\t\t");
						if(symPtr){
							if(prefix & 4){
								opAddr |= ((symPtr->locCtr) & (0x100000 - 1));	// set operand address
								if((prefix & 3) != 1) modi[idxModi++] = locCtr + 1; // when this instruction needs to be relocated
							}
							else{
								if(symPtr->locCtr - PC < 2048 && symPtr->locCtr - PC > -2049){	// PC relative
									opAddr |= ((symPtr->locCtr - PC) & (0x1000 - 1));	// set operand address
									opAddr |= 1 << 13;	// set p bit
								} else if(symPtr->locCtr - B < 4096 && symPtr->locCtr - B > -1){	// Base relative
									opAddr |= ((symPtr->locCtr - B) & (0x1000 - 1));	// set operand address
									opAddr |= 1 << 14;	// set b bit
								} else{	// overflow occured
									printf("%s:%d: error: Prefix '+' need to be used\n", fileName, intLine);
									SET_ERRORFLAG
								}
							}
						}
					} else if((prefix & 3) < 3){
						printf("%s:%d: error: No operands\n", fileName, intLine);
						SET_ERRORFLAG
					} else fprintf(lstPointer, "\t\t");
				}
			}
		} else{	// when opcode stores directive
			if(!strcmp("BYTE", opcode) || !strcmp("WORD", opcode) || !strcmp("RESB", opcode) || !strcmp("RESW", opcode)) fprintf(lstPointer, "%04X", locCtr);
			fprintf(lstPointer, "\t");
			if(*label == '!') fprintf(lstPointer, "\t");
			else fprintf(lstPointer, "%s\t", label);
			fprintf(lstPointer, "%s\t", opcode);

			if(!strcmp("START", opcode)){
				fprintf(lstPointer, "%X", locCtr);
				fprintf(objPointer, "H");
				firstAddr = locCtr;
				if(*label) fprintf(objPointer, "%-6s", label);
				else fprintf(objPointer, "      ");
				fprintf(objPointer, "%06X%06X\n", locCtr, programLen);
				cntObj = 0;
			}
			if(!strcmp("END", opcode)){
				while(!IS_END_STRING(it)) fprintf(lstPointer, "%c", *it++);
				fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
				for(i = 0; i < idxModi; i++) fprintf(objPointer, "M%06X05\n", modi[i]);
				fprintf(objPointer, "E%06X\n", firstAddr);
			}
			if(!strcmp("BASE", opcode)){
				i = 0;
				while(!IS_END_STRING(it)) first[i++] = *it++;
				first[i] = '\0';
				// get operand

				idx = symtabHashFuction(first);
				symPtr = symbolHead[idx];
				while(symPtr && strcmp(symPtr->symbol, first)) symPtr = symPtr->link;
				if(!symPtr){	// when symbol does not exist in SYMTAB
					printf("%s:%d: error: Undefined symbol '%s' is used\n", fileName, intLine, first);
					SET_ERRORFLAG
				}
				fprintf(lstPointer, "%s", first);
				B = symPtr->locCtr;	// set B register
			}
			if(!strcmp("BYTE", opcode)){
				i = 0;
				while(!IS_END_STRING(it)) first[i++] = *it++;
				first[i] = '\0';
				fprintf(lstPointer, "%s\t\t", first);

				for(i = 2; first[i] != '\''; i++) *first == 'X' ? fprintf(lstPointer, "%c", first[i]) : fprintf(lstPointer, "%02X", first[i]);
				first[i--] = '\0';

				if(*first == 'C'){
					if(cntObj + (i - 1) * 2 > 68 || resFlag){
						fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
						sprintf(obj+1, "%06X", locCtr);
						for(i = 2, cntObj = 9; first[i]; i++, cntObj+=2) sprintf(obj+cntObj, "%02X", first[i]);
					} else{
					   	for(i = 2; first[i]; i++) sprintf(obj + cntObj + (i-2) * 2, "%02X", first[i]);
						cntObj += (i-2) * 2;
					}
				} else{
					if(cntObj + i - 1 > 68|| resFlag){
						fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
						sprintf(obj+1, "%06X", locCtr);
						sprintf(obj+9, "%s", first);
						cntObj = 9 + (i - 1);
					} else sprintf(obj + cntObj, "%s", first + 2), cntObj += i-1;
				}
				resFlag = FALSE;
			}
			if(!strcmp("WORD", opcode)){
				i = 0;
				while(!IS_END_STRING(it)) first[i++] = *it++;
				first[i] = '\0';
				constVal = strToDecimal(first) & 0xFFFFFF;
				fprintf(lstPointer, "%s\t%d", first, constVal);

				if(cntObj + 6 > 68 || resFlag){
					fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
					sprintf(obj+1, "%06X", locCtr);
					sprintf(obj+9, "%06X", constVal);
					cntObj = 15;
				} else sprintf(obj + cntObj, "%06X", constVal), cntObj += 6;
				resFlag = FALSE;
			}
			if(!strcmp("RESB", opcode) || !strcmp("RESW", opcode)){
				while(!IS_END_STRING(it)) fprintf(lstPointer, "%c", *it++);
				resFlag = TRUE;
			}
			fprintf(lstPointer, "\n");
			continue;
		}

		if(opIt->format[0] == '1'){
			fprintf(lstPointer, "%02X\n", opAddr);
			if(cntObj + 2 > 68 || resFlag){
				fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
				sprintf(obj+1, "%06X", locCtr);
				sprintf(obj+9, "%02X", opAddr);
				cntObj = 11;
			} else{
				sprintf(obj + cntObj, "%02X", opAddr);
				cntObj += 2;
			}
		}
		if(opIt->format[0] == '2'){
		   	fprintf(lstPointer, "%04X\n", opAddr);
			if(cntObj + 4 > 68 || resFlag){
				fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
				sprintf(obj+1, "%06X", locCtr);
				sprintf(obj+9, "%04X", opAddr);
				cntObj = 13;
			} else{
				sprintf(obj + cntObj, "%04X", opAddr);
				cntObj += 4;
			}
		}
		if(opIt->format[0] == '3'){
		   	fprintf(lstPointer, prefix & 4 ? "%08X\n" : "%06X\n", opAddr);
			if(prefix & 4){
				if(cntObj + 8 > 68 || resFlag){
					fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
					sprintf(obj+1, "%06X", locCtr);
					sprintf(obj+9, "%08X", opAddr);
					cntObj = 17;
				} else{
					sprintf(obj + cntObj, "%08X", opAddr);
					cntObj += 8;
				}
			}
			if(!(prefix & 4)){
				if(cntObj + 6 > 68 || resFlag){
					fprintf(objPointer, "%s%02X%s\n", obj, (cntObj - 9) / 2, obj + 9);
					sprintf(obj+1, "%06X", locCtr);
					sprintf(obj+9, "%06X", opAddr);
					cntObj = 15;
				} else{
					sprintf(obj + cntObj, "%06X", opAddr);
					cntObj += 6;
				}
			}
		}
		resFlag = FALSE;
	}

	fclose(intPointer), fclose(locPointer), fclose(objPointer), fclose(lstPointer);

	if(errorFlag){
		for(i = 0; i < 47; i++){	// cleaning SYMTAB
			while(symbolHead[i]){
				symPtr = symbolHead[i];
				symbolHead[i] = symbolHead[i]->link;
				--symtabSize;
				free(symPtr);
			}
		}
	} else{
		it = fName;
		while(*it) it++;
		intPointer = fopen("tmpobj", "r");
		locPointer = fopen("tmplst", "r");
		strcat(fName, ".obj");
		objPointer = fopen(fName, "w");
		*it = '\0';
		strcat(fName, ".lst");
		lstPointer = fopen(fName, "w");
		while(~(c = fgetc(intPointer))) fputc(c, objPointer);
		while(~(c = fgetc(locPointer))) fputc(c, lstPointer);
		fclose(intPointer), fclose(locPointer), fclose(objPointer), fclose(lstPointer);
	}

	return errorFlag ? -1 : 1;
}
