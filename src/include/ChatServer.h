#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "Include.h"
#include "Font.h"

extern Font* serverMessages[200];
extern Font serverSrcFont;

int InitChatServer(ChatServer* server, int port);

void StoreMessageServer(ChatServer* server, char* text, SDL_Colour colour);

void HandleServerEvents(ChatServer* server);

#endif