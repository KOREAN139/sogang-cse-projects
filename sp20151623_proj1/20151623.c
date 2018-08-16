#include"20151623.h"

int main(){
	int commandNum = 0, i;	// to save the command
	char inputLine[111], *tok;	// inputLine for get input from user, tok for tokenizing
	History *curr;	// for deallocating the list
	Instruction *it, *prev;	// for deallocating the hashtable
	comFuncPtr comFunc[] = { commandHelp, commandDir, 
		commandQuit, commandHistory, commandDump, commandEdit,
	   	commandFill, commandReset, commandMnemonic, commandOplist, 
		commandCat, commandCmp, commandCopy, commandTouch,
   		commandHead, commandEcho };
	// function pointer to reduce code cluster
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
		it = instructionHead[i];
		while(it){
			prev = it;
			it = it->link;
			free(prev);
		}
	}	// deallocating the table

	return 0;
}
