/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

//
// MACRO: Check sign bit (sb) of (v) to see if negative
//   if no, just give number
//   if yes, sign extend (e.g., 0x80_0000 -> 0xFF80_0000)
//INCORRECT, FIX!!!
//#define SIGNEXT(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1<<  (sb))-1) : 0))
#define SIGNEXT(v, sb) ( (v & (1<<(sb-1))) ? (v | (~((1 << (sb - 1))-1))) : (v & (((1 << (sb - 1))-1))) )
//v = number
//sb = number length


// I Instructions
//Add Immediate
int ADDI (int Rd, int Rs1, int Imm) {
  if(Rd != 0x0) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  }
  return 0;
}

//Load Byte
int LB (int Rd, int Rs1, int Imm) {
  int mask = 0x3; //2-bit mask
  //Effective address: address at input with sign extended immediate
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12); 
  int alignedAdr = (effAdr) & ~mask; //assign all data but bytePos
  int read = mem_read_32(alignedAdr); //read from memory with the aligned address
  int bytePos = (effAdr) & mask; //holds the byte position of the memory address
  int shiftRead = read >> (bytePos * 0x8); //aligns data to first 8 bits
  NEXT_STATE.REGS[Rd] = SIGNEXT(shiftRead, 0x8); //writes extended data to register
  return 0;
}

//Load Half-Word
int LH (int Rd, int Rs1, int Imm) {
  int mask = 0x3;
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  int alignedAdr = (effAdr) & ~mask;
  int read = mem_read_32(alignedAdr);
  int bytePos = (effAdr) & mask;
  int shiftRead = read >> (bytePos * 0x8);
  NEXT_STATE.REGS[Rd] = SIGNEXT(shiftRead, 0x10);
  return 0;
}

//BAD?
//Load Word
int LW (int Rd, int Rs1, int Imm) {
  if( Rd != 0) {
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  int read = mem_read_32(effAdr);
  NEXT_STATE.REGS[Rd] = read;
  }
  return 0;
}

//Load Byte Unsigned
int LBU (int Rd, int Rs1, int Imm) {
  int mask = 0x3;
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  int alignedAdr = (effAdr) & ~mask;
  int read = mem_read_32(alignedAdr);
  int bytePos = (effAdr) & mask;
  int shiftRead = read >> (bytePos * 0x8);
  int uMask = 0xff;

  NEXT_STATE.REGS[Rd] = shiftRead & uMask;
  return 0;
}

int LHU (int Rd, int Rs1, int Imm) {
  int mask = 0x3;
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  int alignedAdr = (effAdr) & ~mask;
  int read = mem_read_32(alignedAdr);
  int bytePos = (effAdr) & mask;
  int shiftRead = read >> (bytePos * 0x8);
  int uMask = 0xffff;

  NEXT_STATE.REGS[Rd] = shiftRead & uMask;
  return 0;
}

//BAD?
int SLLI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  int mask = 31;
  cur = CURRENT_STATE.REGS[Rs1] << (SIGNEXT(Imm,12) & mask);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLTI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  cur = (signed int)CURRENT_STATE.REGS[Rs1] < (signed int)(SIGNEXT(Imm,12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLTIU (int Rd, int Rs1, int Imm) {
  int cur = 0;
  cur = (unsigned int)CURRENT_STATE.REGS[Rs1] < (unsigned int)(SIGNEXT(Imm,12));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

//BAD?
int XORI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] ^ SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SRLI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  int mask = 31;
  cur = CURRENT_STATE.REGS[Rs1] >> (SIGNEXT(Imm,12) & mask);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SRAI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  int mask = 31;
  cur = (signed int)CURRENT_STATE.REGS[Rs1] >> (signed int)(SIGNEXT(Imm, 12) & mask);
  NEXT_STATE.REGS[Rd] = SIGNEXT(cur, (32 - (SIGNEXT(Imm, 12))));
  return 0;
}

//BAD?
int ORI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] | SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

//BAD?
int ANDI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] & SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

// U Instructions
//Add upper immediate to program counter
int AUIPC (int Rd, int UpImm) {
  if(Rd != 0x0) {
  NEXT_STATE.REGS[Rd] = (UpImm << 0xC) + CURRENT_STATE.PC;
  }
  return 0;
}

int LUI (int Rd, int UpImm) {
  if(Rd != 0x0) {
  NEXT_STATE.REGS[Rd] = UpImm << 0xC;
  return 0;
  }
}

// S Instructions
int SB (int Rs1, int Rs2, int Imm) { //Rs1 destination, Rs2 source
  //loading
  int mask = 0x3; //2-bit mask
  int byteMask = 0xff; //8-bit mask
  //Effective address: address at input with sign extended immediate
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12); 
  int alignedAdr = (effAdr) & ~mask; //assign all data but bytePos
  int read = mem_read_32(alignedAdr); //read from memory with the aligned address
  int bytePos = (effAdr) & mask; //holds the byte position of the memory address

//writting to mem
  read = read & ~(byteMask << (bytePos * 0x8));
  read = read | ((CURRENT_STATE.REGS[Rs2] & byteMask) << (bytePos * 0x8));
  mem_write_32(alignedAdr, read);
}
int SH (int Rs1, int Rs2, int Imm) {
  //loading
  int mask = 0x3; //2-bit mask
  int byteMask = 0xffff; //16-bit mask
  //Effective address: address at input with sign extended immediate
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12); 
  int alignedAdr = (effAdr) & ~mask; //assign all data but bytePos
  int read = mem_read_32(alignedAdr); //read from memory with the aligned address
  int bytePos = (effAdr) & mask; //holds the byte position of the memory address

//writting to mem
  read = read & ~(byteMask << (bytePos * 0x8));
  read = read | ((CURRENT_STATE.REGS[Rs2] & byteMask) << (bytePos * 0x8));
  mem_write_32(alignedAdr, read);
  return 0;
}

int SW (int Rs1, int Rs2, int Imm) {
  //Effective address: address at input with sign extended immediate
  int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12); //assume effective is aligned
  int regData = CURRENT_STATE.REGS[Rs2]; //read from memory with the aligned address
  mem_write_32(effAdr, regData);
  return 0;
}

// R Instructions
int ADD (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SUB (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] - CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLL (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] << CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLT (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = ((signed int)CURRENT_STATE.REGS[Rs1] < (signed int)(CURRENT_STATE.REGS[Rs2]));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLTU (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = ((unsigned int)CURRENT_STATE.REGS[Rs1] < (unsigned int)(CURRENT_STATE.REGS[Rs2]));
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int XOR (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] ^ CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SRL (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  int mask = 31;
  cur = CURRENT_STATE.REGS[Rs1] >> (CURRENT_STATE.REGS[Rs2] & mask);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SRA (int Rd, int Rs1, int Rs2) { //C arr doesn't work. Do by hand!!!
  int cur = 0;
  int mask = 31;
  cur = (CURRENT_STATE.REGS[Rs1]) >> (CURRENT_STATE.REGS[Rs2] & mask);
  NEXT_STATE.REGS[Rd] = SIGNEXT(cur, (32 - (CURRENT_STATE.REGS[Rs2] & mask)));
  return 0;
}

int OR (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] | CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int AND (int Rd, int Rs1, int Rs2) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] & CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}



// B instructions
int BNE (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] != CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
}

int BEQ (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] == CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
}
int BLT (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if ((signed int)CURRENT_STATE.REGS[Rs1] < (signed int)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
}
int BGE (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if ((signed int)CURRENT_STATE.REGS[Rs1] >= (signed int)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
}
int BLTU (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if ((unsigned int)CURRENT_STATE.REGS[Rs1] < (unsigned int)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
}
int BGEU (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if ((unsigned int)CURRENT_STATE.REGS[Rs1] >= (unsigned int)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
}



// I instruction
//Jump and Link Register
int JALR (int Rd, int Rs1, int Imm) {
  NEXT_STATE.PC = (CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12)) - 4;
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
  return 0;
}



// J instruction
//Jump and Link
int JAL (int Rd, int Imm) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
  NEXT_STATE.PC = CURRENT_STATE.PC + SIGNEXT(Imm, 12) - 4;
  return 0;
}

int ECALL (char* i_){return 0;}

#endif