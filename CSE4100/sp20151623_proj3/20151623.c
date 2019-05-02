#include"20151623.h"

int main(){
	int commandNum = 0, i;	// to save the command
	char inputLine[111], *tok;	// inputLine for get input from user, tok for tokenizing
	History *curr;	// for deallocating the list
	Instruction *it;	// for deallocating the hashtable
	Symbol *symPtr;	// for deallocating symbol table
	Extern *exPtr;	// for deallocating external symbol table
	comFuncPtr comFunc[] = { commandHelp, commandDir, 
		commandQuit, commandHistory, commandDump, commandEdit,
	   	commandFill, commandReset, commandMnemonic, commandOplist, 
		commandCat, commandCmp, commandCopy, commandTouch,
   		commandHead, commandEcho, commandType, commandAssemble,
   		commandSymbol, commandProgaddr, commandLoader, commandRun,
   		commandBp	};
	// function pointer to reduce code cluster

	quitFlag = FALSE;
	dumpLastAddr = 0;
	progAddr = 0x00;
	historyHead = last = NULL;
	bpLast = -1;
	memset(virtualMem, 0, sizeof(virtualMem));
	memset(bptab, 0, sizeof(bptab));
	memset(reg, 0, sizeof(reg));
	for(i = 0; i < 20; i++) instructionHead[i] = NULL;
	for(i = 0; i < 47; i++) symbolHead[i] = NULL;
	for(i = 0; i < 18; i++) externHead[i] = NULL;
	//initialize part

	loadInstruction();
	// load instructions 

	do{
		printf("sicsim>");	// shell
		fgets(inputLine, sizeof(inputLine), stdin);	// get input in inputLine
		inputLine[strlen(inputLine)-1] = '\0';	// set the last character of given line as NULL
		tok = getCommand(inputLine, &commandNum);	// tokenizing given line
		// now tok points first character of parameter, of end of string
		if(commandNum < 0) puts("Invaild command"); // if commandNum stores negative value, it means that given command is invaild
		else comFunc[commandNum](tok, inputLine);	// else run function which mathces to given command
	}while(!quitFlag);
	
	while(historyHead){
		curr = historyHead;
		historyHead = historyHead->link;
		free(curr);
	}	// deallocating the list

	for(i = 0; i < 20; i++){
		while(instructionHead[i]){
			it = instructionHead[i];
			instructionHead[i] = instructionHead[i]->link;
			free(it);
		}
	}	// deallocating the table

	for(i = 0; i < 20; i++){
		while(symbolHead[i]){
			symPtr = symbolHead[i];
			symbolHead[i] = symbolHead[i]->link;
			free(symPtr);
		}
	}	// deallocating the table

	for(i = 0; i < 18; i++){
		while(externHead[i]){
			exPtr = externHead[i];
			externHead[i] = externHead[i]->link;
			free(exPtr);
		}
	}	// deallocating the table
	
	return 0;
}
