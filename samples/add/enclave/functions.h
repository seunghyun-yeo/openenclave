#include <openenclave/internal/globals.h>
#include <openenclave/internal/sgx/td.h>

extern "C"
{
    void* td_to_tcs(const oe_sgx_td_t* td);
    const void* __oe_get_heap_base();
    size_t __oe_get_heap_size();
    const void* __oe_get_heap_end();
    oe_result_t oe_load_module_host_socket_interface();
}

uint64_t oe_get_heap_base()
{
    return (uint64_t)__oe_get_heap_base();
}

uint64_t oe_get_heap_end()
{
    return (uint64_t)__oe_get_heap_end();
}

uint64_t get_stack_base()
{
    oe_sgx_td_t* td = oe_sgx_get_td();
    void* tcs = td_to_tcs(td);
    return ((uint64_t)tcs - 4096) | 0x7fffUL << 32;
}

void print_stack(uint8_t* stack, size_t sz)
{
    for (int i = 0; i < sz / 8; i++)
    {
        for (int j = 7; j >= 0; j--)
            fprintf(stdout, "%02x", (uint8_t)stack[i * 8 + j]);
        fprintf(stdout, "\n");
    }
}
void _dump(mem_layout* mem)
{
    uint64_t return_addr = 0;
    mem->heap_sz = __oe_get_heap_size();
    mem->heap_contents = (void*)oe_host_malloc(mem->heap_sz);
    memcpy(mem->heap_contents, __oe_get_heap_base(), mem->heap_sz);

    __asm__ volatile("nop\n nop\n nop\n");
    __asm__ volatile("mov %%rbp, %0\n" : "=r"(rsp)::);
    __asm__ volatile("mov %%ss:8(%%rbp), %0\n" : "=r"(return_addr)::);
    __asm__ volatile("nop\n nop\n nop\n");
    fprintf(stdout, "   rbp %p\nreturn %p\n", (long)rsp, (long)return_addr);
    mem->stack_sz = get_stack_base() - rsp;
    // mem->stack_sz = 0 ;
    mem->stack_contents = (void*)oe_host_malloc(mem->stack_sz);
    std::cout << "host malloc done" << std::endl;
    memcpy(mem->stack_contents, (void*)rsp, mem->stack_sz);
}

void __restore_stack(mem_layout mem)
{
    rsp = get_stack_base() - mem.stack_sz;
    memcpy((void*)rsp, mem.stack_contents, mem.stack_sz);
}
void __restore_heap(mem_layout mem)
{
    uint64_t return_addr = 0;
    __asm__ volatile("mov %%ss:8(%%rbp), %0\n" : "=r"(return_addr)::);
    fprintf(stdout, "   rbp %p\nreturn %x\n", rsp, return_addr);
    uint64_t end = (uint64_t)__oe_get_heap_base();
    memcpy((void*)__oe_get_heap_base(), mem.heap_contents, mem.heap_sz);
}
void __restore_regs(void)
{
}

void _restore(mem_layout mem)
{
    __restore_heap(mem);
    __restore_stack(mem);
}
