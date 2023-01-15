#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "Include.h"
#include "Font.h"

extern Font* messages[200];
extern Font srcFont;

int InitChatClient(ChatClient* client, ENetAddress* serverAddress);

void StoreMessage(ChatClient* client, char* text);

void HandleClientEvents(ChatClient* client);

void SendData(ChatClient* client, char* data);

#endif