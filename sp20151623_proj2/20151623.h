#ifndef _macros_h_
#define _macros_h_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<dirent.h>
#include<sys/stat.h>

#define FALSE 0
#define TRUE 1
#define IS_END_STRING(x) (*(x) == EOF || *(x) == '\0' || *(x) == '\n')
#define IS_INDENT(x) (*(x) == ' ' || *(x) == '\t')

#define FIRST_BYTE(x) ((x)&(0x0F00))
#define SECOND_BYTE(x) ((x)&(0X00F0))
#define THIRD_BYTE(x) ((x)&(0x000F))
#define IS_COMMENT(x) (!((x)^'.'))
#define IS_DELIMITER(x) ((x) == ' ' || (x) == ',' || (x) == '\r' || (x) == '\t' || (x) == '\n')
#define IS_ALPHABET(x) (((x)|32) >= 'a' && ((x)|32) <= 'z')
#define IS_DECIMAL(x) ((x) >= '0' && (x) <= '9')
#define IS_VALID_PREFIX(x) ((x) == '#' || (x) == '@' || (x) == '+')

typedef void (*comFuncPtr)(char *, char *);

typedef int reg24;

typedef struct _History {
	char command[111];
	struct _History *link;
} History;	// structure for history
History *historyHead, *last;	// empty linked list

typedef struct _Instruction {
	char mnemonic[7], format[4];
	int opcode;
	struct _Instruction *link;
} Instruction;	// structure for instructions
Instruction *instructionHead[20];	// empty hashtable

typedef struct _Symbol {
	char symbol[33];
	int locCtr;
	struct _Symbol *link;
} Symbol;	// structure for symbol
Symbol *symbolHead[47];

unsigned char virtualMem[1048576];	// for virtual memory, 1048576 = 16 ^ 5
int dumpLastAddr;	// last address of dump command
int symtabSize;	// stores symbol table's size
bool quitFlag;	// when quitFlag stores 1 (TRUE), terminates this program

int cmp(const void *a, const void *b);
int hashFunction(char *val);
char* getCommand(char str[], int *commandNum);
int hexstrToInt(char *str);
void addHistory(char com[]);
void commandHelp(char *tok, char com[]);
void commandDir(char *tok, char com[]);
void commandQuit(char *tok, char com[]);
void commandHistory(char *tok, char com[]);
void commandDump(char *tok, char com[]);
void commandEdit(char *tok, char com[]);
void commandFill(char *tok, char com[]);
void commandReset(char *tok, char com[]);
void commandMnemonic(char *tok, char com[]);
void commandOplist(char *tok, char com[]);
void commandType(char *tok, char com[]);
void commandAssemble(char *tok, char com[]);
void commandSymbol(char *tok, char com[]);
void commandCat(char *tok, char com[]);
void commandCmp(char *tok, char com[]);
void commandCopy(char *tok, char com[]);
void commandTouch(char *tok, char com[]);
void commandHead(char *tok, char com[]);
void commandEcho(char *tok, char com[]);
void loadInstruction();
// define on commands.c
int strToDecimal(char *val);
int symtabHashFuction(char *val);
int pass1(char fileName[], int *programLen);
int pass2(char fileName[], int programLen);
// define on assemble.c

#endif
