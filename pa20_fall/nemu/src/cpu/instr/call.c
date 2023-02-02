#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
make_instr_func(call_near){
    OPERAND rel,mem;
    rel.data_size=data_size;
    rel.type=OPR_IMM;
    rel.sreg=SREG_CS;
    rel.addr=cpu.eip+1;
    operand_read(&rel);
    print_asm_1("call","",1+data_size/8,&rel);
    cpu.esp-=data_size/8;
    mem.data_size=data_size;
    mem.type=OPR_MEM;
    mem.sreg=SREG_DS;
    mem.addr=cpu.esp;
    mem.val=cpu.eip+1+data_size/8;
    operand_write(&mem);
    int offset=sign_ext(rel.val,data_size);
    cpu.eip+=offset+1+data_size/8;
    return 0;
}
make_instr_func(call_near_indirect){
    int length=1;
    OPERAND rel,mem;
    rel.data_size=data_size;
    length+=modrm_rm(eip+1,&rel);
    operand_read(&rel);
    print_asm_1("call","",length,&rel);
    cpu.esp-=data_size/8;
    mem.data_size=data_size;
    mem.type=OPR_MEM;
    mem.sreg=SREG_DS;
    mem.addr=cpu.esp;
    mem.val=cpu.eip+length;
    operand_write(&mem);
    if(data_size==16){
        cpu.eip=rel.val&0xFFFF;
    }else{
        cpu.eip=rel.val;
    }
    return 0;
}
