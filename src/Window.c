#include "Include.h"
#include "Window.h"

Window* globalWindow;
Font prewrittenMsg;
char textInput[500];
float timeEnd, timeStart, timeElapsed;

void InitWindow(Window* window, int width, int height, char* name) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error: Couldn't init SDL: %s!", SDL_GetError());
		return;
	}

	if (TTF_Init() < 0) {
		fprintf(stderr, "Error: Couldn't init SDL_TTF: %s!", TTF_GetError());
		return;
	}

	// Assign window stuff
	window->width = width;
	window->height = height;
	window->running = true;

	for (int i = 0; i < 512; i++)
		window->keys[i] = false;

	window->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window->window == NULL) {
		fprintf(stderr, "Error: Couldn't init a window: %s!", SDL_GetError());
		return;
	}

	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (window->renderer == NULL) {
		fprintf(stderr, "Error: Couldn't init a renderer: %s!", SDL_GetError());
		return;
	}

	globalWindow = window;
}

bool WindowIsOpen(ChatClient* client) {
	globalWindow->mouseScroll[0] = globalWindow->mouseScroll[1] = 0;
	
	while (SDL_PollEvent(&globalWindow->event) != 0) {
		switch (globalWindow->event.type) {
			case SDL_QUIT:
				globalWindow->running = false;
				break;
			case SDL_TEXTINPUT:
				if (textInput[499] == 0)
					strcat(&textInput[0], globalWindow->event.text.text);
				else
					fprintf(stderr, "No more space in message buffer!\n");
				InitTextWrapped(&prewrittenMsg, textInput, 940, COLOUR_CLIENT);
			case SDL_KEYDOWN:
				if (globalWindow->event.key.repeat == 0) globalWindow->keys[globalWindow->event.key.keysym.scancode] = true;
				break;
			case SDL_KEYUP:
				globalWindow->keys[globalWindow->event.key.keysym.scancode] = false;
				break;
			case SDL_MOUSEWHEEL:
				globalWindow->mouseScroll[0] = globalWindow->event.wheel.y;
				globalWindow->mouseScroll[1] = globalWindow->event.wheel.x;
				break;
			default:
				break;
		}
	}

	HandleClientEvents(client);

	return globalWindow->running;
}

bool WindowIsOpenServer(ChatServer* server) {
	globalWindow->mouseScroll[0] = globalWindow->mouseScroll[1] = 0;

	while (SDL_PollEvent(&globalWindow->event) != 0) {
		switch (globalWindow->event.type) {
			case SDL_QUIT:
				globalWindow->running = false;
				break;
			case SDL_KEYDOWN:
				globalWindow->keys[globalWindow->event.key.keysym.scancode] = true;
				break;
			case SDL_KEYUP:
				globalWindow->keys[globalWindow->event.key.keysym.scancode] = false;
				break;
			case SDL_MOUSEWHEEL:
				globalWindow->mouseScroll[0] = globalWindow->event.wheel.y;
				globalWindow->mouseScroll[1] = globalWindow->event.wheel.x;
				break;
		}
	}

	HandleServerEvents(server);

	return globalWindow->running;
}