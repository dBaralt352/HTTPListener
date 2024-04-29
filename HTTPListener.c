#include "HTTPListener.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

const char* HEADERS = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
int sock;
int client;

char *getResponseBuffer(const char *buffer) {
    const size_t headers_len = strlen(HEADERS);
    const size_t buffer_len = strlen(buffer);

    char *r = malloc(headers_len + buffer_len + 1);
    if (r == NULL) {
        perror("Failed to allocate memory for response");
        exit(1);
    }

    strncpy(r, HEADERS, headers_len);
    strncat(r, buffer, buffer_len);
    return r;
}

void response(const char *text) {
    char *responseBuffer = getResponseBuffer(text);
    send(client, responseBuffer, strlen(responseBuffer), 0);
    free(responseBuffer);
}

char *serve(const int port, const char *exitMessage) {
    char buffer[256] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("Failed to create socket");
        exit(1);
    }
    const struct sockaddr_in addr = {
        AF_INET,
        htons(port),
        0
    };

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Failed to bind socket");
        exit(1);
    }
    if(listen(sock, 5) < 0) {
        perror("Faailed to listen on socket");
        exit(1);
    }

    client = accept(sock, NULL, NULL);
    if(client < 0) {
        perror("Failed to accept connection");
        exit(1);
    }
    if(recv(client, buffer, sizeof(buffer), 0) < 0) {
        perror("Failed to receive data from client");
        exit(1);
    }

    char *f = buffer + 5;
    *strchr(f, ' ') = 0;

    response(exitMessage);
    close(client);
    close(sock);

    char *result = malloc(strlen(f) + 1);
    if (result == NULL) {
        perror("Failed to allocate memory for result");
        exit(1);
    }
    strcpy(result, f);
    return result;
}
