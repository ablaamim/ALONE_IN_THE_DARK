#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    if (argc > 2 || argc == 1)
    {
        char *str = "Wrong number of arguments\n";
        write(2, str, strlen(str));
        return (EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        int port = atoi(argv[1]);
        char *ip = "127.0.0.1";
        printf("port : %d\nip : %s\n", port, ip);
        // create socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            char *str = "Fatal error\n";
            write(2, str, strlen(str));
            return (EXIT_FAILURE);
        }
        // bind socket
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
        if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        {
            char *str = "Fatal error\n";
            write(2, str, strlen(str));
            return (EXIT_FAILURE);
        }
        // listen socket
        if (listen(sock, 128) == -1)
        {
            char *str = "Fatal error\n";
            write(2, str, strlen(str));
            return (EXIT_FAILURE);
        }

        // accept socket
        int client_sock;
        struct sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);
        if ((client_sock = accept(sock, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_size)) < 0)
        {
            char *str = "Fatal error\n";
            write(2, str, strlen(str));
            return (EXIT_FAILURE);
        }

        while (1)
        {
            char buf[1024];
            int len = read(client_sock, buf, 1024);
            if (len == -1)
            {
                char *str = "Fatal error\n";
                write(2, str, strlen(str));
                return (EXIT_FAILURE);
            }
            else if (len == 0)
                break;
            buf[len] = '\0';
            printf("buf : %s\n", buf);
            write(client_sock, buf, strlen(buf));
        }
    }
    return (EXIT_SUCCESS);
}
