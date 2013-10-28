#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    Double a;
    Word b;

    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct) {
		case 0x0:
			processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
			processor->PC += 4;
			break;
		case 0x2:
			processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
			processor->PC += 4;
			break;
		case 0x3:
			processor->R[instruction.rtype.rd] = (sWord)processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
			processor->PC += 4;
			break;
		case 0x8:
			processor->PC = processor->R[instruction.rtype.rs];
			break;
		case 0x9:
			b = processor->PC + 4;
			processor->PC = processor->R[instruction.rtype.rs];
			processor->R[instruction.rtype.rd] = b;
			break;
                case 0xc: // funct == 0xc (SYSCALL)
                    	execute_syscall(processor);
                    	processor->PC += 4;
                    	break;
		case 0x10:
			processor->R[instruction.rtype.rd] = processor->RHI;
			processor->PC += 4;
			break;
		case 0x12:
			processor->R[instruction.rtype.rd] = processor->RLO;
			processor->PC += 4;
			break;
		case 0x18:
			a = (sDouble)((sWord)processor->R[instruction.rtype.rs]) * (sDouble)((sWord)processor->R[instruction.rtype.rt]);
			processor->RLO = (sWord)a;
			processor->RHI = (sWord)((a & 0xffffffff00000000) >> 32);
			processor->PC += 4;
			break;
		case 0x19:
			a = (Double)(processor->R[instruction.rtype.rs]) * (Double)(processor->R[instruction.rtype.rt]);
			processor->RLO = (sWord)a;
			processor->RHI = (sWord)((a & 0xffffffff00000000) >> 32);
			processor->PC += 4;
			break;
		case 0x21: 
			processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
			processor->PC += 4;
			break;
		case 0x23:
			processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
			processor->PC += 4;
			break;
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
		case 0x25: 
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
		case 0x26:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
		case 0x27: 
                    processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
                    processor->PC += 4;
                    break;
		case 0x2a: 
                    processor->R[instruction.rtype.rd] = (sWord)processor->R[instruction.rtype.rs] < (sWord)processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
		case 0x2b:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
	case 0x3: 
	    processor->R[31] = processor->PC + 4;
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
		case 0x4:
			if(processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt]){
				processor->PC = processor->PC + 4 + (sHalf)(instruction.itype.imm << 2);
			} else {
				processor->PC += 4;
			}
			break;
		case 0x5:
			if(processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt]){
				processor->PC = processor->PC + 4 + (sHalf)(instruction.itype.imm << 2);
			} else {
				processor->PC += 4;
			}
			break;
		case 0x9:
			processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + (sHalf)instruction.itype.imm;
			processor->PC += 4;
			break;
		case 0xa:
			processor->R[instruction.itype.rt] = (sWord)processor->R[instruction.itype.rs] < (sHalf)instruction.itype.imm;
			processor->PC += 4;
			break;
		case 0xb:
			processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] < (sHalf)instruction.itype.imm;
			processor->PC += 4;
			break;
        case 0xc:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xd: // opcode == 0xd (ORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xe:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xf: 
            processor->R[instruction.itype.rt] = instruction.itype.imm << 16;
            processor->PC += 4;
            break;
		case 0x20:
			processor->R[instruction.itype.rt] = (sHalf)(load(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_BYTE));
			processor->PC += 4;
			break;
		case 0x21:
			processor->R[instruction.itype.rt] = (sHalf)(load(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_HALF_WORD));
			processor->PC += 4;
			break;
		case 0x23:
			processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_WORD);
			processor->PC += 4;
			break;
		case 0x24:
			processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_BYTE);
			processor->PC += 4;
			break;
		case 0x25:
			processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_HALF_WORD);
			processor->PC += 4;
			break;
		case 0x28:
			store(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_BYTE, processor->R[instruction.itype.rt]);
			processor->PC += 4;
			break;
		case 0x29:
			store(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_HALF_WORD, processor->R[instruction.itype.rt]);
			processor->PC += 4;
			break;
		case 0x2b:
			store(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_WORD, processor->R[instruction.itype.rt]);
			processor->PC += 4;
			break;
        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}


int check(Address address,Alignment alignment) {
    /* YOUR CODE HERE */
    if (address < 1 || address >= MEMORY_SPACE || address % alignment != 0) {
    	return 0;
    }
    return 1;
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    /* YOUR CODE HERE */
    if (alignment == LENGTH_WORD) {
    	*(Word*)(memory+address) = (Word)value;
    }
    else if (alignment == LENGTH_HALF_WORD) {
	*(Half*)(memory+address) = (Half)value;
    }
    else if (alignment == LENGTH_BYTE){
    	*(Byte*)(memory+address) = (Byte)value;
    }
    else {
	*(Double*)(memory+address) = (Double)value;
    }
}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    /* YOUR CODE HERE */
    // incomplete stub to let "simple" execute
    // (only handles size == SIZE_WORD correctly)
    // feel free to delete and implement your own way
    if (alignment == LENGTH_WORD) {
    	return *(Word*)(memory+address);
    }
    else if (alignment == LENGTH_HALF_WORD) {
    	return *(Half*)(memory+address);
    }
    else if (alignment == LENGTH_BYTE){
    	return *(Byte*)(memory+address);
    }
    else {
	return *(Double*)(memory+address);
    }

}
