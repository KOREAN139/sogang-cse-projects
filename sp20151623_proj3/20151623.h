#ifndef _20151623_h_
#define _20151623_h_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdarg.h>
#include<unistd.h>

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
#define DISP(x) (((*(x) & 0x0F) << 8) | (*(x+1)))
#define ADDRESS(x) ((((*(x) & 0x0F) << 16)) | (*(x+1) << 8) | (*(x+2)))

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

typedef struct _Extern {
	char symbol[7];
	int csAddr, length;
	struct _Extern *link;
} Extern;	// structure for external symbol
Extern *externHead[18];

unsigned char virtualMem[1048576];	// for virtual memory, 1048576 = 16 ^ 5
int dumpLastAddr;	// last address of dump command
int symtabSize;	// stores symbol table's size
int estabSize;	// stores external symol table's size
unsigned int progAddr;	// starting address when excute 'run' or 'loader' command
int fileLen;	// store file length
int lastExecAddr;	// store last executed address (At opcode)
int bpLast;	// last address that stopped
bool quitFlag;	// when quitFlag stores 1 (TRUE), terminates this program
bool bptab[1048576];	// for checking break point
reg24 reg[10];
// 0: A, 1: X, 2: L, 3: B, 4: S, 5: T, 6: F, 8: PC, 9: SW

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
void commandProgaddr(char *tok, char com[]);
void commandLoader(char *tok, char com[]);
void commandRun(char *tok, char com[]);
void commandBp(char *tok, char com[]);
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
int assemblerPass1(char fileName[], int *programLen);
int assemblerPass2(char fileName[], int programLen);
// define on assemble.c
int cmpE(const void *a, const void *b);
int externHashFunction(char *val);
int linkingLoaderPass1(int numOfFile, ...);
int linkingLoaderPass2(int numOfFile, ...);
void printLoadmap();
// define on linkingloader.c
int getTargetAddr(int curr, int flags, int reg[]);
int getVal(int targetAddr, int flags);
void printReg(int reg[]);
void run();
// define on run.c

#endif
