#include "socket.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int create_socket(int port) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server_info = {0};
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = INADDR_ANY;
    server_info.sin_port = htons(port);

    int opt = 1;
    if(0 > setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }

    if (bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0) {
        perror("bind");
        return -1;
    }

    if (listen(sfd, 5) < 0) {
        perror("listen");
        return -1;
    }

    return sfd;
}
