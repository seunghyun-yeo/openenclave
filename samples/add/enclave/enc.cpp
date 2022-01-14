#include <openenclave/enclave.h>
#include <add_t.h>
// system
#include <sys/socket.h>
#include <arpa/inet.h>
// libcxx
#include <cstdio>
#include <cstring>
// custom
#include <functions.h>

#include <netinet/in.h>
#include <unistd.h>

using namespace std;

void enclave_add(int* a, int* b, int* result_u)
{
    *result_u = *a + *b;
    fprintf(stdout, "This is add function for toy\n");

    // oe_result_t result = host_add();ca
}
void enclave_add2(int a, int b, int* result_u)
{
    *result_u = a + b;
}
void mig_svr_boot()
{
    oe_load_module_host_socket_interface();
    struct sockaddr_in addr, in_addr;
    uint32_t in_addr_len = sizeof(in_addr);
    int server_socket, client_socket;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        fprintf(stdout, "socket creation failed\n");
    }
    if (bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        fprintf(stdout, "port binding failed\n");
    }
    if (listen(server_socket, 1) < 0)
    {
        fprintf(stdout, "listen error\n");
    }
    while (1)
    {
        close(client_socket);
        client_socket =
            accept(server_socket, (struct sockaddr*)&in_addr, &in_addr_len);
        if (client_socket < 0)
        {
            fprintf(stdout, "accept error\n");
        }
    }
}