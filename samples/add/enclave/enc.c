
#include <stdio.h>
#include <string.h>
#include <add_t.h>
// #include <openenclave/internal/globals.h>
// #include <openenclave/internal/sgx/td.h>

// uint64_t get_stack_base(){
//     oe_sgx_td_t* td = oe_sgx_get_td();
//     void* tcs = td_to_tcs(td);
//     return ((uint64_t)tcs -4096);
// }

void enclave_add(int* a, int* b, int* result_u){
    *result_u=*a+*b;
    fprintf(stdout, "This is add function for toy\n");

    // oe_result_t result = host_add();
}
void enclave_add2(int a, int b, int* result_u){

    *result_u=a+b;
}
void migrate(char** result_u, size_t* sz){
    __asm__ volatile(
        "mov %%rsp, %0\n"
        "nop\n"
        "nop\n"
        :"=r" (rsp)
    );
    *sz=rbp-rsp;
    uint64_t stack_base= 0x1234567890UL;
    *result_u=oe_host_malloc(*sz);
    memcpy(*result_u,(void*) rsp,*sz);
}