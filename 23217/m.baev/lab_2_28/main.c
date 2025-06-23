#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <termios.h>

#define BUF_SZ 4096
#define PAGE_LINES 25
#define PORT_HTTP 80

void raw_mode_on() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void raw_mode_off() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int do_connect(const char *hname) {
    struct hostent *hptr = gethostbyname(hname);
    if (hptr == NULL) {
        fprintf(stderr, "[!] Failed to resolve host: %s\n", hname);
        exit(111);
    }

    int socky = socket(AF_INET, SOCK_STREAM, 0);
    if (socky < 0) {
        perror("[!] socket()");
        exit(112);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr.s_addr, hptr->h_addr, hptr->h_length);
    addr.sin_port = htons(PORT_HTTP);

    if (connect(socky, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("[!] connect()");
        close(socky);
        exit(113);
    }

    return socky;
}

void make_request(int fd, const char *h, const char *p) {
    char out[1024] = {0};
    snprintf(out, sizeof(out),
             "GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: weird-client\r\n\r\n",
             p, h);
    ssize_t sent = send(fd, out, strlen(out), 0);
    if (sent < (ssize_t)strlen(out)) {
        fprintf(stderr, "[!] send() failed partially\n");
    }
}

int main(int argc, char **argv) {

    char hst[256], pth[1024];
    memset(hst, 0, sizeof(hst));
    memset(pth, 0, sizeof(pth));

    const char *s_ptr = strchr(argv[1], '/');
    if (s_ptr != NULL) {
        int len = s_ptr - argv[1];
        strncpy(hst, argv[1], len);
        hst[len] = '\0';
        strcpy(pth, s_ptr);
    } else {
        strcpy(hst, argv[1]);
        strcpy(pth, "/");
    }

    int netfd = do_connect(hst);
    make_request(netfd, hst, pth);

    raw_mode_on();

    char buf[BUF_SZ];
    int l_count = 0;
    fd_set set;

    while (1) {
        FD_ZERO(&set);
        FD_SET(netfd, &set);
        FD_SET(STDIN_FILENO, &set);
        int biggest = (netfd > STDIN_FILENO ? netfd : STDIN_FILENO) + 1;

        if (select(biggest, &set, NULL, NULL, NULL) < 0) {
            perror("select()");
            break;
        }

        if (FD_ISSET(netfd, &set)) {
            ssize_t got = read(netfd, buf, sizeof(buf) - 1);
            if (got <= 0) break;
            buf[got] = '\0';

            char *ln = strtok(buf, "\n");
            while (ln != NULL) {
                printf("%s\n", ln);
                fflush(stdout);
                l_count++;
                if (l_count >= PAGE_LINES) {
                    printf("--press SPACE to continue--\n");
                    fflush(stdout);
                    char ch = 0;
                    while (read(STDIN_FILENO, &ch, 1) > 0 && ch != ' ');
                    l_count = 0;
                }
                ln = strtok(NULL, "\n");
            }
        }
    }

    raw_mode_off();
    close(netfd);
    return 0;
}
