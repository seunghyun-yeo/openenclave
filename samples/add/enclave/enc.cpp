#include <add_t.h>
#include <openenclave/enclave.h>
// system
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
// libc
#include <cstdio>
// libcxx
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <iostream>
// custom
#include <functions.h>

// using namespace std;

void restore();
void dump();

std::map<std::string, std::string> m = {
    {"dump", "dump done"},
    {"restore", "restore done"},
    {"exit", "bye bye"}};

std::map<std::string,std::function<void()>> map_to_func = {
    {"dump", dump},
    {"restore", restore}};

mem_layout* memsite_setup(){
    mem_layout* mem;
    ocall_memsetup(&mem);
    oe_in
    return mem;
}
void dump(){
    mem_layout* mem = memsite_setup();
    _dump(mem);
}

void restore(){
    mem_layout* mem = memsite_setup();
    _restore(*mem);
}
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
    fprintf(stdout, "server is in ready state\n");
    while (1)
    {
        close(client_socket);
        client_socket =
            accept(server_socket, (struct sockaddr*)&in_addr, &in_addr_len);
        if (client_socket < 0)
        {
            fprintf(stdout, "accept error\n");
        }
        char buffer[100];
        int bytes_read = 0;
        memset(buffer, 0, sizeof(buffer));
        bytes_read = read(client_socket, buffer, sizeof(buffer));
        std::string input(buffer, bytes_read), response;
        std::int32_t a = 0;
        if (m.find(input) != m.end())
        {
            if (map_to_func.find(input) != map_to_func.end())
                map_to_func[input]();
            response = m[input];
        }
        else
        {
            response = "list of command : dump, restore, exit";
        }
        send(client_socket, response.c_str(), response.size(), 0);
        if (input == "exit")
        {
            close(client_socket);
            break;
        }
    }
}