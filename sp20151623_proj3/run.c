#include "20151623.h"

int getTargetAddr(int curr, int flags, int reg[]){
	int targetAddr, disp;

	reg[8] += 3 + (flags & 1);

	if(flags & 1) targetAddr = ADDRESS(virtualMem + curr + 1);	// extended format
	else if(flags & 6){ // PC, Base relative
		disp = DISP(virtualMem + curr + 1);
		if(disp & (1 << 11)) disp = -((~disp + 1) & ((1 << 12) - 1));
		targetAddr = (flags & 2 ? reg[8] : reg[3]) + disp;
	}
	else targetAddr = DISP(virtualMem + curr + 1);
	// immediate addressing
	// calculate target address

	if(flags & 8) targetAddr += reg[1];
	// indexed addressing

	return targetAddr;
}

int getVal(int targetAddr, int flags){
	int ret = 0;
	flags >>= 4;

	if(flags == 1) ret = targetAddr;
	// immediate
	else if(flags == 2){	// indirect
		targetAddr = (virtualMem[targetAddr] << 16) | (virtualMem[targetAddr + 1] << 8) | virtualMem[targetAddr + 2];
		ret = (virtualMem[targetAddr] << 16) | (virtualMem[targetAddr + 1] << 8) | virtualMem[targetAddr + 2];
	} else if(flags == 3) ret = (virtualMem[targetAddr] << 16) | (virtualMem[targetAddr + 1] << 8) | virtualMem[targetAddr + 2];
	// simple

	return ret;
}

void printReg(int reg[]){
	printf("A : %06X X : %06X\nL : %06X PC: %06X\nB : %06X S : %06X\nT : %06X\n", reg[0], reg[1], reg[2], reg[8], reg[3], reg[4], reg[5]);
	return;
}

void run(){
	int i, opcode, curr, endAddr, idx, flags, targetAddr, val, r1, r2;

	curr = reg[8] = lastExecAddr;
	endAddr = progAddr + fileLen;
	reg[2] = endAddr;
	while(reg[8] != endAddr){
		if(bptab[curr]){	// when break point is located at current location
			if(bpLast != -1) bpLast = -1;
			else{
				bpLast = lastExecAddr = curr;
				printReg(reg);
				printf("Stop at checkpoint[%04X]\n", curr);
				return;
			}
		}
		opcode = virtualMem[curr] & 0xFC;
		if(opcode == 0x00 || opcode == 0x68 || opcode == 0x74 || opcode == 0x08 || opcode == 0x6C || opcode == 0x04 || opcode == 0x50){	// LDA, LDB, LDL, LDS, LDT, LDX, LDCH
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			val = getVal(targetAddr, flags);

			r1 = opcode == 0x00 || opcode == 0x50 ? 0 : opcode == 0x68 ? 3 : opcode == 0x08 ? 2 : opcode == 0x6C ? 4 : opcode == 0x74 ? 5 : 1;
			reg[r1] = opcode == 0x50 ? ((reg[0] & 0xFFFF00) | (val >> 16)) : val;
			// store value in reg
		} else if(opcode == 0x0C || opcode == 0x10 || opcode == 0x14 || opcode == 0x78 || opcode == 0x7C || opcode == 0x84){	// STA, STB, STL, STS, STT, STX, STL
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			if((flags & 0x30) == 0x20) targetAddr = getVal(targetAddr, 0x30);

			r1 = opcode == 0x0C ? 0 : opcode == 0x10 ? 1 : opcode == 0x14 ? 2 : opcode == 0x78 ? 3 : opcode == 0x7C ? 4 : 5;
			for(i = 0; i < 3; i++) virtualMem[targetAddr + i] = (unsigned char)((reg[r1] >> (2 - i) * 8) & ((1 << 8) - 1));
			// set memory as reg
		} else if(opcode == 0x18 || opcode == 0x1C){	// ADD, SUB
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			val = getVal(targetAddr, flags);

			reg[0] = reg[0] + (opcode == 0x18 ? val : -val);
			reg[0] &= (1 << 24) - 1;
			// add (or substract) (m..m+2) to reg A
		} else if(opcode == 0x28){	// COMP
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			val = getVal(targetAddr, flags);
			
			reg[9] = reg[0] > val ? '>' : reg[0] < val ? '<' : '=';
			// save the result in SW (CC)
		} else if(opcode == 0x2C){	// TIX
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			val = getVal(targetAddr, flags);

			++reg[1];
			reg[9] = reg[1] > val ? '>' : reg[1] < val ? '<' : '=';
		} else if(opcode >= 0x30 && opcode <= 0x3C){	// JEQ, JGT, JLT, J
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			if((flags & 0x30) == 0x20) targetAddr = getVal(targetAddr, 0x30);

			if(opcode == 0x30 && reg[9] == '=') reg[8] = targetAddr;
			if(opcode == 0x38 && reg[9] == '<') reg[8] = targetAddr;
			if(opcode == 0x34 && reg[9] == '>') reg[8] = targetAddr;
			if(opcode == 0x3C) reg[8] = targetAddr;
			// set PC as value when CC satisfies condition
		} else if(opcode == 0x40 || opcode == 0x44){	// AND, OR
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			val = getVal(targetAddr, flags);

			if(opcode == 0x40) reg[0] &= val;
			else reg[0] |= val;
			// and (or or) reg A with value

			reg[0] &= (1 << 24) - 1;
		} else if(opcode == 0x48){	// JSUB
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			if((flags & 0x30) == 0x20) targetAddr = getVal(targetAddr, 0x30);

			reg[2] = reg[8];
			reg[8] = targetAddr;
			// save return address in reg L, set PC as value just read
		} else if(opcode == 0x4C) reg[8] = reg[2];	// RSUB
		else if(opcode == 0x54){	// STCH
			flags = ((virtualMem[curr] & 0x03) << 4) | ((virtualMem[curr + 1] & 0xF0) >> 4);

			targetAddr = getTargetAddr(curr, flags, reg);
			if((flags & 0x30) == 0x20) targetAddr = getVal(targetAddr, 0x30);

			virtualMem[targetAddr] = (unsigned char)(reg[0] & 0xF);
		} else if(opcode == 0x90 || opcode == 0x94){	// ADDR, SUBR
			reg[8] += 2;	// format 2
			r1 = (virtualMem[curr + 1] & 0xF0) >> 4;
			r2 = virtualMem[curr + 1] & 0x0F;
			reg[r2] = reg[r2] + (opcode == 0x90 ? reg[r1] : -reg[r1]);
			reg[r2] &= (1 << 24) - 1;
			// add (or subtract) r1, r2 and store the result in r2
		} else if(opcode == 0xA0){	// COMPR
			reg[8] += 2;	// format 2
			r1 = (virtualMem[curr + 1] & 0xF0) >> 4;
			r2 = virtualMem[curr + 1] & 0x0F;
			reg[9] = reg[r1] > reg[r2] ? '>' : reg[r1] < reg[r2] ? '<' : '=';
			// save the result in SW (CC)
		} else if(opcode == 0xA4 || opcode == 0xA8){	// SHIFTR, SHIFTL
			reg[8] += 2;	// format 2
			r1 = (virtualMem[curr + 1] & 0xF0) >> 4;
			r2 = virtualMem[curr + 1] & 0x0F;
			while(r2--){
				if(opcode == 0xA4) reg[r1] = ((reg[r1] << 1) & ((1 << 24) - 1)) | (((reg[r1] << 1) & (1 << 24)) ? 1 : 0);
				// SHIFTR - circular shift
				else reg[r1] = (reg[r1] >> 1) | (reg[r1] & (1 << 23));
				// SHIFTL - copy left-most bit to vacated bit
			}
		} else if(opcode == 0xAC){	// RMO
			reg[8] += 2;	// format 2
			r1 = (virtualMem[curr + 1] & 0xF0) >> 4;
			r2 = virtualMem[curr + 1] & 0x0F;
			reg[r2] = reg[r1];
			// r2 <- (r1)
		} else if(opcode == 0xB4){	// CLEAR
			reg[8] += 2;	// format 2
			r1 = (virtualMem[curr + 1] & 0xF0) >> 4;
			reg[r1] = 0;	// clear
		} else if(opcode == 0xB8){	// TIXR
			reg[8] += 2;	// format 2
			r1 = (virtualMem[curr + 1] & 0xF0) >> 4;
			++reg[1];
			// increase reg X
			reg[9] = reg[1] > reg[r1] ? '>' : reg[1] < reg[r1] ? '<' : '=';
			// set CC
		} else if(opcode == 0xE0 || opcode == 0xDC || opcode == 0xD8){	// TD, WD, RD
			reg[8] += 3 + ((virtualMem[curr + 1] & 16) >> 4);
			if(opcode == 0xE0) reg[9] = '<';
		}
		curr = reg[8];
		// move PC to LOCCTR
	}
	printReg(reg);
	puts("End program");
	memset(reg, 0, sizeof(reg));
	lastExecAddr = progAddr;	// when run ends, need to initialize lastExecAddr
	bpLast = -1;

	return;
}
