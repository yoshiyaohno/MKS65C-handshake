#include "pipe_networking.h"

#define PUBLIC_PIPE "jeff"
#define PIPELEN     16
#define CONFIRM     "YAHOO"
    // yes I know this was defined for us

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    int read_result;

    if(mkfifo("jeff", 0755)) {
        printf("[SERVER]\tError creating public pipe:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }

    int public_pipe = open(PUBLIC_PIPE, O_RDONLY);
    // block here until the pipe is opened at the other end
    if(public_pipe == -1) {
        printf("[SERVER]\tError opening public pipe:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }

    // part 1: client sends downstream pipe
    char down_name[PIPELEN];
    read_result = read(public_pipe, down_name, PIPELEN - 1);
    if(read_result == -1) {
        printf("[SERVER]\tError reading downstream pipe name:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }
    down_name[read_result + 1] = '\0';

    // part 2: open downstream pipe and send upstream pipe
    *to_client = open(down_name, O_WRONLY);
    char up_name[PIPELEN];

    // part 3: confirm upstream pipe
    return 0;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    return 0;
}
