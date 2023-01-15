#ifndef WINDOW_H
#define WINDOW_H

#include "Include.h"
#include "ChatClient.h"
#include "ChatServer.h"

extern char textInput[500];
extern Font prewrittenMsg;

void InitWindow(Window* window, int width, int height, char* name);

bool WindowIsOpen(ChatClient* client);

bool WindowIsOpenServer(ChatServer* server);

#endif