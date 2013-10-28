#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"

void decode_instruction(Instruction instruction) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0 (SPECIAL)
            switch(instruction.rtype.funct) {
				case 0x0:
					printf("sll\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
					break;
				case 0x2:
					printf("srl\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
					break;
				case 0x3:
					printf("sra\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
					break;
				case 0x8:
					printf("jr\t$%d\n",instruction.rtype.rs);
					break;
				case 0x9:
					printf("jalr\t$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs);
					break;
                case 0xc: // funct == 0xc (SYSCALL)
                    printf("syscall\n");
                    break;
				case 0x10:
					printf("mfhi\t$%d\n",instruction.rtype.rd);
					break;
				case 0x12:
					printf("mflo\t$%d\n",instruction.rtype.rd);
					break;
				case 0x18:
					printf("mult\t$%d,$%d\n",instruction.rtype.rs,instruction.rtype.rt);
					break;
				case 0x19:
					printf("multu\t$%d,$%d\n",instruction.rtype.rs,instruction.rtype.rt);
					break;
				case 0x21:
                    printf("addu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
				case 0x23:
                    printf("subu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x24: // funct == 0x24 (AND)
                    printf("and\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
				case 0x25:
                    printf("or\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
				case 0x26:
                    printf("xor\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
				case 0x27:
                    printf("nor\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
				case 0x2a:
                    printf("slt\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
				case 0x2b:
                    printf("sltu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: illegal function: %08x\n",__FUNCTION__,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
		case 0x4:
            printf("beq\t$%d,$%d,%d\n",instruction.itype.rs,instruction.itype.rt,instruction.itype.imm*4);
            break;
		case 0x5:
            printf("bne\t$%d,$%d,%d\n",instruction.itype.rs,instruction.itype.rt,instruction.itype.imm*4);
            break;
		case 0x9:
            printf("addiu\t$%d,$%d,%d\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
		case 0xa:
            printf("slti\t$%d,$%d,%d\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
		case 0xb:
            printf("sltiu\t$%d,$%d,%d\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
		case 0xc:
            printf("andi\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
        case 0xd: // opcode == 0xd (ORI)
            printf("ori\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
		case 0xe:
            printf("xori\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
		case 0xf:
            printf("lui\t$%d,0x%x\n",instruction.itype.rt,instruction.itype.imm);
            break;
		case 0x20:
            printf("lb\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
		case 0x21:
            printf("lh\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
		case 0x23:
            printf("lw\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
		case 0x24:
            printf("lbu\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
		case 0x25:
            printf("lhu\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
		case 0x28:
            printf("sb\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
		case 0x29:
            printf("sh\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
		case 0x2b:
            printf("sw\t$%d,%d($%d)\n",instruction.itype.rt,instruction.itype.imm,instruction.itype.rs);
            break;
        case 0x2: // opcode == 0x2 (J)
            printf("j\t0x%x\n",instruction.jtype.addr*4);
            break;
		case 0x3:
            printf("jal\t0x%x\n",instruction.jtype.addr*4);
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: illegal instruction: %08x\n",__FUNCTION__,instruction.bits);
            exit(-1);
            break;
    }
}
