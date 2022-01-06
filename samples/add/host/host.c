#include <openenclave/host.h>
#include <stdio.h>

#include "add_u.h"

void host_add()
{
    fprintf(stdout, "Enclave called into host to print: add!\n");
}
void print_stack(char* stack, size_t sz){
    for(int i=0; i<sz/8; i++){
        for(int j=7; j>=0 ; j--)
            fprintf(stdout, "%02x", (uint8_t)stack[i*8+j]);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

int main(int argc, const char* argv[])
{
    oe_result_t result;
    int ret = 1;
    oe_enclave_t* enclave = NULL;

    uint32_t flags = OE_ENCLAVE_FLAG_DEBUG;
    // if (check_simulate_opt(&argc, argv))
    // {
    //     flags |= OE_ENCLAVE_FLAG_SIMULATE;
    // }

    if (argc != 2)
    {
        fprintf(
            stderr, "Usage: %s enclave_image_path [ --simulate  ]\n", argv[0]);
        goto exit;
    }

    // Create the enclave
    result = oe_create_add_enclave(
        argv[1], OE_ENCLAVE_TYPE_AUTO, flags, NULL, 0, &enclave);
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "oe_cre_helloworld_enclave(): result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
    }

    // Call into the enclave
    int a=1;
    int b=2;
    int result_u=0;
    result = enclave_add(enclave,&a,&b,&result_u);
    // fprintf(stdout,"result is : %d\n",result_u);
    result=enclave_add2(enclave,a,b,&result_u);
    // fprintf(stdout,"result is : %d\n",result_u);
    char* stack;
    size_t sz=0;
    result=dump(enclave,&stack,&sz);
    
    if(sz!=0) print_stack(stack,sz);
    
    result=restore(enclave,stack,sz);
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "calling into enclave_helloworld failed: result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
    }

    ret = 0;

exit:
    // Clean up the enclave if we created one
    if (enclave)
        oe_terminate_enclave(enclave);

    return ret;
}
