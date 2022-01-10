
#include <stdio.h>
#include <string.h>
#include <add_t.h>
#include <openenclave/internal/globals.h>
#include <openenclave/internal/sgx/td.h>

/* 
   Disalbe ASLR(Address Space layout randomization)
   by setting /proc/sys/kernel/randomizae_va_sapce as 0 (origin was 2)
*/
__oe_get_heap_base()
__oe_get_heap_size()

uint64_t get_stack_base(){
    oe_sgx_td_t* td = oe_sgx_get_td();
    void* tcs = td_to_tcs(td);
    return ((uint32_t)tcs -4096) | 0x7fffUL << 32;
}

void enclave_add(int* a, int* b, int* result_u){
    *result_u=*a+*b;
    fprintf(stdout, "This is add function for toy\n");

    // oe_result_t result = host_add();ca
}
void enclave_add2(int a, int b, int* result_u){

    *result_u=a+b;
}
void dump(char** result_u, size_t* sz){
    __asm__ volatile(
        "mov %%rbp, %0\n"
        :"=r" (rsp)
    );
    *sz=get_stack_base()-rsp;
    *result_u=oe_host_malloc(*sz);
    memcpy(*result_u,(void*) rsp,*sz);
    // fprintf(stdout,"stack_base : %016p\nrsp : %016p\nsz : %016p\n",rbp,rsp,*sz);
}

void print_stack(uint8_t *stack, size_t sz){
    for(int i=0; i<sz/8; i++){
        for(int j=7; j>=0 ; j--)
            fprintf(stdout, "%02x", (uint8_t)stack[i*8+j]);
        fprintf(stdout, "\n");
    }
    // fprintf(stdout, "\n");
}


void restore(char* in, size_t in_sz){
    
    // uint64_t stack_base = get_stack_base();
    rsp = 0x007ffff69a5760;
    // fprintf(stdout,"stack_base : %016p\nrsp : %016p\nsz : %016p\n",rbp,rsp,in_sz);
    uint64_t stuff;
    fprintf(stderr,"stack_base : %016p rsp : %016p sz : %016p\n",&stuff,(&stuff) + 1, (&stuff)+8);

    for(int i=0; i<in_sz/8; i++){
        memcpy(&stuff,&in[i*8],8);
        // print_stack(&stuff,8);
        __asm__ volatile(
            "mov %0, (%1)\n"
            ::"r" (stuff), "r" (rsp):
        );
        rsp+=0x8;
    }
}
