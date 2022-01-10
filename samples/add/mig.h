typedef struct _mem_layout{
    void* stack_contents;
    size_t stack_sz;
    void* heap_contents;
    size_t heap_sz;
} mem_layout;