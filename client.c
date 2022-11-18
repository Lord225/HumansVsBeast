#include <ncurses.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 9002
#define ADDRESS 0x7f000001

int main(void) {



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

    // start ncurses

    initscr();

    noecho();
    cbreak();
    curs_set(0);
    refresh();

    keypad(stdscr, TRUE);

    int key = ' ';

    while (tolower(key) != 'q') {
        key = getch();
        send(sfd, &key, sizeof(key), 0);
    }


    endwin();



    // close the socket
    close(sfd);

    return 0;
}