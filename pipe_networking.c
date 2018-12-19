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
    int result, from_client;

    printf("[SERVER]\tCreating public pipe....\n");

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

    // BEGIN HANDSHAKE //

    // part 1: client sends downstream pipe
    char down_name[PIPELEN];
    result = read(public_pipe, down_name, PIPELEN - 1);
    if(result == -1) {
        printf("[SERVER]\tError reading downstream pipe name:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }
    down_name[result + 1] = '\0';

    // part 2: close public pipe, open downstream pipe and send upstream pipe
    close(public_pipe);
    result = (*to_client = open(down_name, O_WRONLY));
    if(result == -1) {
        printf("[SERVER]\tError opening downstream pipe:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }

    char up_name[PIPELEN] = "upstroof";
    if(mkfifo(up_name, 0755)) {
        printf("[SERVER]\tError creating upstream pipe:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }

    result = (from_client = open(up_name, 0755));
    if(result == -1) {
        printf("[SERVER]\tError opening upstream pipe:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }

    result = write(*to_client, up_name, PIPELEN - 1);
    if(result == -1) {
        printf("[SERVER]\tError sending upstream pipe name:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }

    // part 3: confirm upstream pipe
    char confirm[PIPELEN];
    result = read(up_pipe, confirm, PIPELEN - 1);
    if(result == -1) {
        printf("[SERVER]\tError reading confirmation:\n\t%s\n\n",
                    strerror(errno));
        exit(1);
    }

    if(!strcmp(confirm, CONFIRM)) {
        printf("[SERVER]\tConfirmation successful. Handshake complete.\n");
    }
    

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
