#include "cpu/intr.h"
#include "cpu/instr.h"

/*
Put the implementations of `int' instructions here.

Special note for `int': please use the instruction name `int_' instead of `int'.
*/
make_instr_func(int_){
    OPERAND imm;
    imm.sreg=SREG_CS;
    imm.data_size=8;
    imm.type=OPR_IMM;
    imm.addr=eip+1;
    operand_read(&imm);
    print_asm_1("int","",2,&imm);
    raise_sw_intr(imm.val);
    return 0;
}