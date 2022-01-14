void print_stack(void* contents, size_t sz)
{
    uint8_t* p_contents=(uint8_t*)contents;
    for (int i = 0; i < sz / 8; i++)
    {
        for (int j = 7; j >= 0; j--)
            fprintf(stdout, "%02x", p_contents[i * 8 + j]);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}