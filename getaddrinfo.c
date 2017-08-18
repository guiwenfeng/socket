#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    char ip[64], ip_len = 64;
    int port;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully bind(2).
        If socket(2) (or bind(2)) fails, we (close the socket
        and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family == AF_INET)
        {
            struct sockaddr_in *s = (struct sockaddr_in *)rp->ai_addr;
            inet_ntop(AF_INET,(void*)&(s->sin_addr),ip,ip_len);
            port = ntohs(s->sin_port);
        }
        else
        {
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)rp->ai_addr;
            inet_ntop(AF_INET6,(void*)&(s->sin6_addr),ip,ip_len);
            port = ntohs(s->sin6_port);
        }
        
        /*
         * * * * * * * * * * * * result * * * * * * * * * * *
         * AF_INET SOCK_STREAM TCP 0.0.0.0 9000
         * AF_INET6 SOCK_STREAM TCP :: 9000
         */
        printf("%d %d %d %s %d\n",rp->ai_family,rp->ai_socktype,rp->ai_protocol,ip,port);
    }
    
    if (rp == NULL)
    {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);
}
