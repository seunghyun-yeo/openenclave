// oe headers
#include <openenclave/host.h>
// libcxx
#include <cstdio>
#include <deque>
#include <thread>
// edger8r generated file
#include "add_u.h"
// custom file
#include <host_custom.h>

using namespace std;

// global variable
mem_layout mem;
oe_enclave_t* enclave = NULL;
deque<thread> threads;

void ocall_memsetup(mem_layout** enclave_mem){
    *enclave_mem = &mem;
}

void server_bootup(oe_enclave_t* enclave)
{
    oe_result_t result;
    set_mem_pointer(enclave,&mem);
    result = mig_svr_boot(enclave);
}

int main(int argc, const char* argv[])
{
    oe_result_t result;
    int ret = 1;
    int result_u = 0;
    int a=1, b=2;

    uint32_t flags = OE_ENCLAVE_FLAG_DEBUG_AUTO;
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
    threads.push_back(thread(server_bootup, enclave));
    // Call into the enclave
    result = enclave_add(enclave, &a, &b, &result_u);
    result = enclave_add2(enclave, a, b, &result_u);
    fprintf(stdout, "result is : %d\n", result_u);

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
    //  Clean up migration thread
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
    // Clean up the enclave if we created one
    if (enclave)
        oe_terminate_enclave(enclave);

    return ret;
}
