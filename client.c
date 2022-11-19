#include <ncurses.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define PORT 9002
#define ADDRESS 0x7f000001

int main(void) {

    int pid = getpid();

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sfd) {
        perror("socket");
        return -1;
    }

    // define server address
    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr.s_addr = htonl(ADDRESS);

    socklen_t server_info_len = sizeof(server_info);

    // connect to server
    if (0 > connect(sfd, (struct sockaddr *) &server_info, server_info_len)) {
        perror("connect");
        return -1;
    }

    send(sfd, &pid, sizeof(pid), 0);


        // start ncurses

    initscr();
//    raw();
    noecho();
    cbreak();
    curs_set(0);
    timeout(-1);
    refresh();

    keypad(stdscr, TRUE);

    while (1) {
        flushinp();
        int key = getch();
        if(key == 'q') {
            break;
        }
        send(sfd, &key, sizeof(key), 0);
        int response;
        recv(sfd, &response, sizeof(response), 0);

    }



    endwin();

    close(sfd);

    printf("Disconnected\n");

    return 0;
}
