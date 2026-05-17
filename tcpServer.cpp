#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

/* You will to add includes here */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
// Included to get the support library
#include <calcLib.h>

#include "protocol.h"

using namespace std;
/* Needs to be global, to be rechable by callback and main */
int loopCount = 0;
int terminate = 0;

/* Call back function, will be called when the SIGALRM is raised when the timer expires. */
void checkJobbList(int signum)
{
    // As anybody can call the handler, its good coding to check the signal number that called it.

    printf("Let me be, I want to sleep, loopCount = %d.\n", loopCount);

    if (loopCount > 20)
    {
        printf("I had enough.\n");
        terminate = 1;
    }

    return;
}

int main(int argc, char *argv[])
{
    char *input = strdup(argv[1]);
    char *port = strrchr(input, ':');
    if (port == NULL)
    {
        fprintf(stderr, "Fel: Formatet måste vara adress:port\n");
        free(input);
        return 1;
    }
    *port = '\0';
    port++;
    char *address = input;
    printf("Adress: %s\n", address);
    printf("Port: %s\n", port);

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int server_fd = -1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int s = getaddrinfo(address, port, &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo fel: %s\n", gai_strerror(s));
        free(input);
        return 1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        server_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (server_fd == -1)
        {
            continue;
        }

        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        if (bind(server_fd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            printf("Servern har bundits framgångsrikt!\n");
            break;
        }

        close(server_fd);
    }
    freeaddrinfo(result);
    free(input);

    if (rp == NULL)
    {
        fprintf(stderr, "Kunde inte binda till någon adress\n");
        return 1;
    }
    /*
       Prepare to setup a reoccurring event every 10s. If it_interval, or it_value is omitted, it will be a single alarm 10s after it has been set.
    */
    struct itimerval alarmTime;
    alarmTime.it_interval.tv_sec = 10;
    alarmTime.it_interval.tv_usec = 10;
    alarmTime.it_value.tv_sec = 10;
    alarmTime.it_value.tv_usec = 10;

    /* Regiter a callback function, associated with the SIGALRM signal, which will be raised when the alarm goes of */
    // signal(SIGALRM, checkJobbList);
    // setitimer(ITIMER_REAL, &alarmTime, NULL); // Start/register the alarm.

#ifdef DEBUG
    printf("DEBUGGER LINE ");
#endif

    // while (terminate == 0)
    //{
    //     printf("This is the main loop, %d time.\n", loopCount);
    //     sleep(1);
    //     loopCount++;
    // }

    printf("done.\n");
    return (0);
}
