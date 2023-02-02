#include "cpu/cpu.h"

void set_CF_add(uint32_t result,uint32_t src,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.CF=(result<src);
}

void set_ZF(uint32_t result,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.ZF=(result==0);
}

void set_SF(uint32_t result,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.SF=sign(result);
}

void set_PF(uint32_t result){
    int i=0;
    if((result&0x00000001)==0x00000001) i++;
    if((result&0x00000002)==0x00000002) i++;
    if((result&0x00000004)==0x00000004) i++;
    if((result&0x00000008)==0x00000008) i++;
    if((result&0x00000010)==0x00000010) i++;
    if((result&0x00000020)==0x00000020) i++;
    if((result&0x00000040)==0x00000040) i++;
    if((result&0x00000080)==0x00000080) i++;
    if(i%2)
        cpu.eflags.PF=0;
    else
        cpu.eflags.PF=1;
}

void set_OF_add(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    /*switch(data_size){
        case 8:
            result=sign_ext(result&0xFF,8);
            src=sign_ext(src&0xFF,8);
            dest=sign_ext(dest&0xFF,8);
            break;
        case 16:
            result=sign_ext(result&0xFFFF,16);
            src=sign_ext(src&0xFFFF,16);
            dest=sign_ext(dest&0xFFFF,16);
            break;
        default:
            break;
    }*/
    if(sign(src)==sign(dest)){
        if(sign(result)!=sign(dest))
            cpu.eflags.OF=1;
        else
            cpu.eflags.OF=0;
    }else{
        cpu.eflags.OF=0;
    }
}
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    res=dest+src;
    set_CF_add(res,src,data_size);
    set_PF(res);
    //set_AF();
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));

}

void set_CF_adc(uint32_t result,uint32_t src,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    if(cpu.eflags.CF==0){
       cpu.eflags.CF=(result<src); 
    }
    else{
        cpu.eflags.CF=(result<=src);
    }
    
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    res=src+dest+cpu.eflags.CF;
    set_CF_adc(res,src,data_size);
    set_PF(res);
    //set_AF();
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
}

void set_CF_sub(uint32_t dest,uint32_t src,size_t data_size){
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.CF=dest<src;
}
void set_OF_sub(uint32_t result,uint32_t src,uint32_t dest,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    if(sign(src)!=sign(dest)){
        if(sign(result)==sign(src))
            cpu.eflags.OF=1;
        else
            cpu.eflags.OF=0;
    }else{
        cpu.eflags.OF=0;
    }
}
uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
        uint32_t res=0;
        uint32_t temp=(src^0xFFFFFFFF)+1;
        res=dest+temp;
        set_CF_sub(dest,src,data_size);
        set_PF(res);
        //set_AF();
        set_ZF(res,data_size);
        set_SF(res,data_size);
        set_OF_sub(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
}
void set_CF_sbb(uint32_t dest,uint32_t src,size_t data_size){
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    if(cpu.eflags.CF==0){
    cpu.eflags.CF=dest<src;
    }
    else{
        cpu.eflags.CF=(dest<=src);
    }
}
uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    uint32_t temp=(src^0xFFFFFFFF)+1;
    res=dest+temp-cpu.eflags.CF;
    set_CF_sbb(dest,src,data_size);
    set_PF(res);
    //set_AF();
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_sub(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));

}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
    uint64_t res=0;
    res=(uint64_t)src*(uint64_t)dest;
    uint32_t flag=res>>data_size;
    if(flag&0xFFFFFFFF){
        cpu.eflags.CF=1;
        cpu.eflags.OF=1;
    }else{
        cpu.eflags.CF=0;
        cpu.eflags.OF=0;
    }
    return res&0xFFFFFFFFFFFFFFFF;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
    int64_t res=0;
    res=(int64_t)src*(int64_t)dest;
    /*uint32_t flag=res>>data_size;
    if(flag&0xFFFFFFFF){
        cpu.eflags.CF=1;
        cpu.eflags.OF=1;
    }else{
        cpu.eflags.CF=0;
        cpu.eflags.OF=0;
    }*/
    return res&0xFFFFFFFFFFFFFFFF;
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
    uint32_t res=0;
    //src=src&(0xFFFFFFFFFFFFFFFF>>(64-data_size));
    //dest=dest&(0xFFFFFFFFFFFFFFFF>>(64-data_size));
    if(src==0){
        assert(0);
    }else{
    res=dest/src;
    
    }
    return res;
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
 int32_t res=0;
 if(src==0){
     assert(0);
 }else{
     res=dest/src;
 }
 return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
    uint32_t res=0;
    if(src==0){
        assert(0);
    }
    res=dest%src;
    return res&0xFFFFFFFF;
}

int32_t alu_imod(int64_t src, int64_t dest)
{
    int32_t res=0;
    if(src==0){
        assert(0);
    }else{
     res=dest%src;   
    }
    return res;
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    res=src&dest;
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    set_SF(res,data_size);
    set_ZF(res,data_size);
    set_PF(res);
    return res&(0xFFFFFFFF>>(32-data_size));
    
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    res=src^dest;
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    set_SF(res,data_size);
    set_ZF(res,data_size);
    set_PF(res);
    return res&(0xFFFFFFFF>>(32-data_size));
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    res=src|dest;
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    set_SF(res,data_size);
    set_ZF(res,data_size);
    set_PF(res);
    return res&(0xFFFFFFFF>>(32-data_size));
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    if(src==0){
        return dest&(0xFFFFFFFF>>(32-data_size));
    }
    uint32_t temp=dest<<(src-1);
    res=temp<<1;
    temp=sign_ext(temp,data_size);
    cpu.eflags.CF=sign(temp);
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
    
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    if(src==0){
        return dest&(0xFFFFFFFF>>(32-data_size));
    }
    dest=dest&(0xFFFFFFFF>>(32-data_size));
    uint32_t temp=dest>>(src-1);
    res=temp>>1;
    temp=temp&1;
    //temp=sign_ext(temp,data_size);
    cpu.eflags.CF=temp;
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    if(src==0){
        return dest&(0xFFFFFFFF>>(32-data_size));
    }
    switch(data_size){
        case 32:{
            int32_t temp=(int32_t)dest>>(src-1);
            res=temp>>1;
            cpu.eflags.CF=temp&0x1;
        break;
        }
        case 16:{
            int16_t temp=(int16_t)dest>>(src-1);
            res=temp>>1;
            cpu.eflags.CF=temp&0x1;
          break;  
        }
        case 8:{
            int8_t temp=(int8_t)dest>>(src-1);
            res=temp>>1;
            cpu.eflags.CF=temp&0x1;
           break; 
        }
        default:
            break;
    }
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res=0;
    if(src==0){
        return dest&(0xFFFFFFFF>>(32-data_size));
    }
    uint32_t temp=dest<<(src-1);
    res=temp<<1;
    temp=sign_ext(temp,data_size);
    cpu.eflags.CF=sign(temp);
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
}
