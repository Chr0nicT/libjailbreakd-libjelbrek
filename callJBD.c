//
//  callJBD.c
//  pspaw_payload
//
//  Created by Tanay Findley on 5/14/19.
//  Copyright © 2019 Slice Team. All rights reserved.
//

#include "callJBD.h"
#include <mach/mach.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <spawn.h>
#include <sys/wait.h>
#include <syslog.h>
#include <malloc/malloc.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <libkern/OSByteOrder.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <mach/kern_return.h>
#include <sys/types.h>

int sockfd;

struct __attribute__((__packed__)) JAILBREAKD_FIXUP_EXECUTABLE {
    uint8_t Command;
    char exec[1024];
};

struct __attribute__((__packed__)) JAILBREAKD_FIXUP_DYLIB {
    uint8_t Command;
    char dylib[1024];
};

struct __attribute__((__packed__)) JAILBREAKD_UNSANDBOX {
    uint8_t Command;
    int32_t Pid;
};

struct __attribute__((__packed__)) JAILBREAKD_FIXUP_SETUID {
    uint8_t Command;
    int32_t Pid;
};

struct __attribute__((__packed__)) JAILBREAKD_ENTITLE_PID {
    uint8_t Command;
    int32_t Pid;
};

struct __attribute__((__packed__)) JAILBREAKD_ENTITLE_PID_AND_SIGCONT {
    uint8_t Command;
    int32_t PID;
};

void ENTITLE_NOW(pid_t PID)
{
    #define BUFSIZE 1024
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    
    struct JAILBREAKD_ENTITLE_PID entitlePacket;
    entitlePacket.Command = JAILBREAKD_COMMAND_ENTITLE;
    entitlePacket.Pid = PID;
    
    memcpy(buf, &entitlePacket, sizeof(struct JAILBREAKD_ENTITLE_PID));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_ENTITLE_PID), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");
}

void ENTITLE_AND_SIGCONT(pid_t PID)
{
    #define BUFSIZE 1024
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    
    struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT entitleSIGCONTPacket;
    entitleSIGCONTPacket.Command = JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT;
    entitleSIGCONTPacket.PID = PID;
    
    memcpy(buf, &entitleSIGCONTPacket, sizeof(struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");
}

void ENTITLE_PID_AND_SIGCONT_AFTER_DELAY(pid_t PID)
{
    #define BUFSIZE 1024
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    
    struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT entitlePacket;
    entitlePacket.Command = JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT_AFTER_DELAY;
    entitlePacket.PID = PID;
    
    memcpy(buf, &entitlePacket, sizeof(struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");
}

void ENTITLE_SIGCONT_XPCPROXY(pid_t PID)
{
    #define BUFSIZE 1024
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    
    struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT entitleSIGCONTPacket;
    entitleSIGCONTPacket.Command = JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT_FROM_XPCPROXY;
    entitleSIGCONTPacket.PID = PID;
    
    memcpy(buf, &entitleSIGCONTPacket, sizeof(struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_ENTITLE_PID_AND_SIGCONT), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");
}

void FIXUPSETUID(pid_t PID)
{
    #define BUFSIZE 1024
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    
    struct JAILBREAKD_FIXUP_SETUID setuidPacket;
    setuidPacket.Command = JAILBREAKD_COMMAND_FIXUP_SETUID;
    setuidPacket.Pid = PID;
    
    memcpy(buf, &setuidPacket, sizeof(struct JAILBREAKD_FIXUP_SETUID));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_FIXUP_SETUID), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");

}

void UNSANDBOX(pid_t PID)
{
    #define BUFSIZE 1024
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    
    struct JAILBREAKD_UNSANDBOX entitlePacket;
    entitlePacket.Command = JAILBREAKD_COMMAND_UNSANDBOX;
    entitlePacket.Pid = PID;
    
    memcpy(buf, &entitlePacket, sizeof(struct JAILBREAKD_UNSANDBOX));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_UNSANDBOX), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");
}

void FIXUP_DYLIB(pid_t PID, char *dylib)
{
    #define BUFSIZE_DYLIB 2000
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE_DYLIB];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE_DYLIB);
    
    struct JAILBREAKD_FIXUP_DYLIB dylibPacket;
    dylibPacket.Command = JAILBREAKD_COMMAND_FIXUP_DYLIB;
    strcpy(dylibPacket.dylib, dylib);
    
    memcpy(buf, &dylibPacket, sizeof(struct JAILBREAKD_FIXUP_DYLIB));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_FIXUP_DYLIB), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");
}

void FIXUP_EXEC(char *exec)
{
    #define BUFSIZE_EXEC 2000
    
    int portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE_EXEC];
    
    hostname = "127.0.0.1";
    portno = 5;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket\n");
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* get a message from the user */
    bzero(buf, BUFSIZE_EXEC);
    
    struct JAILBREAKD_FIXUP_EXECUTABLE execPacket;
    execPacket.Command = JAILBREAKD_COMMAND_FIXUP_DYLIB;
    strcpy(execPacket.exec, exec);
    
    memcpy(buf, &execPacket, sizeof(struct JAILBREAKD_FIXUP_EXECUTABLE));
    
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, sizeof(struct JAILBREAKD_FIXUP_EXECUTABLE), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        printf("Error in sendto\n");
}

void closejailbreakfd() {
    close(sockfd);
    sockfd = -1;
}



kern_return_t calljailbreakd(int command, pid_t PID, char *file)
{
    if (command == JAILBREAKD_COMMAND_ENTITLE)
    {
        ENTITLE_NOW(PID);
    }
    
    if (command == JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT)
    {
        ENTITLE_AND_SIGCONT(PID);
    }
    
    if (command == JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT_AFTER_DELAY)
    {
        ENTITLE_PID_AND_SIGCONT_AFTER_DELAY(PID);
    }
    
    if (command == JAILBREAKD_COMMAND_ENTITLE_AND_SIGCONT_FROM_XPCPROXY)
    {
        ENTITLE_SIGCONT_XPCPROXY(PID);
    }
    
    if (command == JAILBREAKD_COMMAND_FIXUP_SETUID)
    {
        FIXUPSETUID(PID);
    }
    
    if (command == JAILBREAKD_COMMAND_UNSANDBOX)
    {
        UNSANDBOX(PID);
    }
    
    if (command == JAILBREAKD_COMMAND_FIXUP_DYLIB)
    {
        FIXUP_DYLIB(PID, file);
    }
    
    if (command == JAILBREAKD_COMMAND_FIXUP_EXECUTABLE)
    {
        FIXUP_EXEC(file);
    }
    
    closejailbreakfd();
    
    return KERN_SUCCESS;
}

