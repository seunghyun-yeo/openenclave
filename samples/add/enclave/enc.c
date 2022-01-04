
#include <stdio.h>
#include <string.h>
#include <add_t.h>
#include <openenclave/internal/globals.h>
#include <openenclave/internal/sgx/td.h>

/* 
   Disalbe ASLR(Address Space layout randomization)
   by setting /proc/sys/kernel/randomizae_va_sapce as 0 (origin was 2)
*/

uint64_t get_stack_base(){
    oe_sgx_td_t* td = oe_sgx_get_td();
    void* tcs = td_to_tcs(td);
    return ((uint32_t)tcs -4096) | 0x7fffUL << 32;
}

void enclave_add(int* a, int* b, int* result_u){
    *result_u=*a+*b;
    fprintf(stdout, "This is add function for toy\n");

    // oe_result_t result = host_add();
}
void enclave_add2(int a, int b, int* result_u){

    *result_u=a+b;
}
void dump(char** result_u, size_t* sz){
    __asm__ volatile(
        "mov %%rbp, %0\n"
        "nop\n"
        "nop\n"
        :"=r" (rsp)
    );
    uint64_t stack_base= 0x1234567890UL;
    *sz=get_stack_base()-rsp;
    *result_u=oe_host_malloc(*sz);
    memcpy(*result_u,(void*) rsp,*sz);
    fprintf(stdout,"stack_base : %016p\nrsp : %016p\nsz : %016p\n",get_stack_base(),rsp,*sz);
}

void print_stack(uint8_t *stack, size_t sz){
    for(int i=0; i<sz/8; i++){
        for(int j=7; j>=0 ; j--)
            fprintf(stdout, "%02x", (uint8_t)stack[i*8+j]);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}


void restore(uint8_t** in, size_t in_sz){
    // uint64_t stack_base = get_stack_base();
    // rsp = stack_base - in_sz;
    // fprintf(stdout,"stack_base : %016p\nrsp : %016p\nsz : %016p\n",get_stack_base(),rsp,in_sz);
    __asm__ volatile(
        "mov %%rbp, %0\n"
        "nop\n"
        "nop\n"
        :"=r" (rsp)
    );
    // print_stack(*in,in_sz);
     for(size_t i=0; i<in_sz; i++){
        uint64_t input = *in[i];
        fprintf(stdout,"%zu\n",i);
        __asm__ volatile(
            "nop\n"
            "nop\n"
            "mov %0,  -0x10(%%rbp)\n"
            "nop\n"
            "nop\n"
            :
            :"r" (input)
            :
        );
    }
}
