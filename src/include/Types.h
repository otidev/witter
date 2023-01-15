#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdbool.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <enet/enet.h>

#define COLOUR_CLIENT (SDL_Colour){0, 0, 0, 255}
#define COLOUR_SERVER (SDL_Colour){255, 0, 0, 255}

typedef struct Window {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	int width, height;
	bool running;

	bool keys[512];

	int32_t mouseScroll[2];
} Window;

extern Window* globalWindow;

typedef struct ChatServer {
	char* name;
	ENetAddress address;
	ENetEvent event;
	ENetHost* server;
	ENetPeer* peer;
} ChatServer;

typedef struct ChatClient {
	char* name;
	ENetHost* client;
	ENetEvent event;
	ENetPeer* peer;
	ENetPacket* packet;
} ChatClient;

typedef struct Font {
	SDL_Texture* tex;
	TTF_Font* font;
	SDL_Rect renderRect;
} Font;


#endif