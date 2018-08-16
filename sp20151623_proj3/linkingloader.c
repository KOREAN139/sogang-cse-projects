#include "20151623.h"

int cmpE(const void *a, const void *b) { return ((Extern *)a)->csAddr - ((Extern *)b)->csAddr; }

int externHashFunction(char *val){
	int ret = 0;
	while(*val){
		ret *= 39;
		ret += *val++;
	}
	return (ret & 0x7fffffff) % 18;
}

int linkingLoaderPass1(int numOfFile, ...){
	int i, idx;
	unsigned int csAddr, csLength, symAddr;
	char fileName[111], line[111], symbol[7];
	FILE *fp;
	Extern *curr, *new;
	va_list names;
	va_start(names, numOfFile);

	for(i = estabSize = 0; i < 18; i++){
		while(externHead[i]){
			curr = externHead[i];
			externHead[i] = externHead[i]->link;
			free(curr);
		}
	}
	csAddr = progAddr;
	// initialize

	while(numOfFile--){
		vsprintf(fileName, "%s", names);
		if(!(fp = fopen(fileName, "r"))){
			printf("Failed to open file: %s\n", fileName);
			return -1;
		}
		// open file

		fgets(line, 110, fp);
		// get first line in object file

		sscanf(line, "%*c%6s%*6x%6x", symbol, &csLength);
		// get control section name and CSLTH

		idx = externHashFunction(symbol);
		curr = externHead[idx];
		while(curr && strcmp(curr->symbol, symbol)) curr = curr->link;
		if(curr){
			puts("Duplicated symbol");
			return -1;
		}
		// find duplicated symbol here

		new = (Extern *)malloc(sizeof(Extern));
		strcpy(new->symbol, symbol);
		new->csAddr = csAddr;
		new->length = csLength;
		new->link = NULL;
		if(!externHead[idx]) externHead[idx] = new;
		else{
			curr = externHead[idx];
			while(curr->link) curr = curr->link;
			curr->link = new;
		}
		++estabSize;
		// put csname in ESTAB

		do{
			memset(line, 0, sizeof(line));
			fgets(line, 110, fp);
			if(*line != 'D') continue;
			// In pass 1, we only consider D record
			for(i = 1; !IS_END_STRING(line+i); i += 12){
				memset(symbol, 0, sizeof(symbol));
				sscanf(line + i, "%6s%6x", symbol, &symAddr);
				idx = externHashFunction(symbol);
				curr = externHead[idx];
				while(curr && strcmp(curr->symbol, symbol)) curr = curr->link;
				if(curr){
					puts("Duplicated symbol");
					return -1;
				}
				// find duplicated symbol here
		
				new = (Extern *)malloc(sizeof(Extern));
				strcpy(new->symbol, symbol);
				new->csAddr = symAddr + csAddr;
				new->length = 0;
				new->link = NULL;
				if(!externHead[idx]) externHead[idx] = new;
				else{
					curr = externHead[idx];
					while(curr->link) curr = curr->link;
					curr->link = new;
				}
				++estabSize;
				// put symbol in ESTAB here
			}
		} while(*line != 'E');
		// read file until last line, end record.

		csAddr += csLength;
		// starting address for next section

		fclose(fp);
	}

	fileLen = csAddr - progAddr;
	va_end(names);
	return 1;
}

int linkingLoaderPass2(int numOfFile, ...){
	int i, idx, val;
	unsigned int csAddr, execAddr, csLength, tmpAddr, modAddr, halfBytes, ref[22], refNum, tLen;
	char fileName[111], line[111], symbol[7], sign;
	FILE *fp;
	Extern *ptr;
	va_list names;
	va_start(names, numOfFile);

	csAddr = progAddr;
	while(numOfFile--){
		vsprintf(fileName, "%s", names);
		if(!(fp = fopen(fileName, "r"))){
			printf("Failed to open file: %s\n", fileName);
			return -1;
		}

		*ref = 0;
		// *ref == 1 ? using reference number : does not use reference number

		fgets(line, 110, fp);
		// get first line in object file

		sscanf(line, "%*c%6s%*6x%6x", symbol, &csLength);
		ref[1] = csAddr;
		// get control section name and CSLTH
		// set reference number for section name

		do{
			val = 0;
			memset(line, 0, sizeof(line));
			fgets(line, 110, fp);
			if(*line == 'R'){	// R record
				if(!IS_ALPHABET(line[1])){	// when reference number is used
					*ref = 1;
					for(i = 1; !IS_END_STRING(line+i); i += 8){
						sscanf(line + i, "%2x%6s", &refNum, symbol);
						idx = externHashFunction(symbol);
						ptr = externHead[idx];
						while(ptr && strcmp(ptr->symbol, symbol)) ptr = ptr->link;
						if(!ptr){	// when such symbol does not defined external or does not defined at all
							printf("Undefined symbol %s\n", symbol);
							return -1;
						}
						ref[refNum] = ptr->csAddr;
					}
				}
			} else if(*line == 'T'){
				sscanf(line + 1, "%6x%2x", &tmpAddr, &tLen);
				tmpAddr += csAddr;
				for(i = 0; i < tLen * 2; i += 2){
					sscanf(line + 9 + i, "%2x", &val);
					val &= (1 << 8) - 1;
					virtualMem[tmpAddr + i/2] = (unsigned char)val;
				}
			} else if(*line == 'M'){
				sscanf(line, "%*c%6x%2x%c%6s", &tmpAddr, &halfBytes, &sign, symbol);
				tmpAddr += csAddr;

				if(*ref){
					refNum = hexstrToInt(symbol);
					modAddr = ref[refNum];
				} else{
					idx = externHashFunction(symbol);
					ptr = externHead[idx];
					while(ptr && strcmp(ptr->symbol, symbol)) ptr = ptr->link;
					if(!ptr){	// when such symbol does not defined external or does not defined at all
						printf("Undefined symbol %s\n", symbol);
						return -1;
					}
					modAddr = ptr->csAddr;
				}
				modAddr *= sign == '+' ? 1 : -1;
				// get symbol value

				for(i = tmpAddr; i < tmpAddr + (halfBytes + 1) / 2; val |= virtualMem[i++]) val <<= 8;
				val += modAddr;
				val &= ((1 << (halfBytes * 4)) - 1);
				// add / subtract value with specific value

				if(halfBytes & 1) virtualMem[tmpAddr] = (virtualMem[tmpAddr] & ((1 << 8) - (1 << 4))) + (val >> --halfBytes * 4), ++tmpAddr;
				for(i = 0; i < halfBytes / 2; i++) virtualMem[tmpAddr + i] = (unsigned char)((val >> (halfBytes - (i + 1) * 2) * 4) & ((1 << 8) - 1));
			}
		}while(*line != 'E');

		csAddr += csLength;
		// starting address for next section

		fclose(fp);
	}

	lastExecAddr = progAddr;
	va_end(names);
}

void printLoadmap(){
	int i, idx;
	Extern *ptr, *arr;
	arr = (Extern *)malloc(sizeof(Extern) * estabSize);
	for(i = idx = 0; i < 18; i++){
		ptr = externHead[i];
		while(ptr){
			strcpy(arr[idx].symbol, ptr->symbol);
			arr[idx].csAddr = ptr->csAddr;
			arr[idx++].length = ptr->length;
			ptr = ptr->link;
		}
	}
	qsort(arr, estabSize, sizeof(Extern), cmpE);
	// sort by address where symbol located at

	printf("Control\t\tSymbol\n");
	printf("section\t\tname\t\tAddress\t\tLength\n");
	printf("------------------------------------------------------\n");
	for(i = 0; i < estabSize; i++){
		if(!arr[i].length) printf("\t\t%s\t\t%04X\n", arr[i].symbol, arr[i].csAddr);
		else printf("%s\t\t\t\t%04X\t\t%04X\n", arr[i].symbol, arr[i].csAddr, arr[i].length);
	}
	printf("------------------------------------------------------\n");

	free(arr);
}
