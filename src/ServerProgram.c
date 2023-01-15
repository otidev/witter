#include "Include.h"
#include "Window.h"
#include "ChatServer.h"
#include "ChatClient.h"

Window window;
ChatServer server;
SDL_Texture* renderTexture;
int scrolled;

int main(int argc, char** argv) {
	// If there is no second argument, die
	if (argc == 1) {
		fprintf(stderr, "Error: You need a username to use this program! (That can be your second argument)\n");
		return 1;
	}

	// Init our window and our main font
	InitWindow(&window, 1024, 720, "Witter (server host)");
	InitFont(&serverSrcFont, "assets/PublicSans-VariableFont_wght.ttf", 18);
	
	// Init ENet
	if (enet_initialize() != 0) {
		fprintf(stderr, "Error: Couldn't init ENet!");
        return EXIT_FAILURE;
    }

	// Init a server on a host
	if (InitChatServer(&server, 7777))
		return 1;
	
	// Set the client name to second argument
	server.name = argv[1];

	// Server note
	StoreMessageServer(&server, "NOTE: This program is in server mode (messages can't be sent). Only messages in red can be seen by the server.", COLOUR_SERVER);

	renderTexture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, window.width, 5000);

	while (WindowIsOpenServer(&server)) {
		SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
		SDL_RenderClear(window.renderer);

		// Render to the target
		SDL_SetRenderTarget(window.renderer, renderTexture);
		SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
		SDL_RenderClear(window.renderer);
		for (int i = 0; i < 200; i++) {
			if (serverMessages[i] != NULL) {
				SDL_RenderCopy(window.renderer, serverMessages[i]->tex, NULL, &serverMessages[i]->renderRect);
			}
		}
		// Stop rendering to target
		SDL_SetRenderTarget(window.renderer, NULL);

		// Draw *the* target
		SDL_RenderCopy(window.renderer, renderTexture, &(SDL_Rect){0, scrolled, 944, window.height}, &(SDL_Rect){0, 0, 944, window.height});

		// Handle scrolling
		if (scrolled != 2000 - window.height && window.mouseScroll[0] < 0)
			scrolled -= 10 * window.mouseScroll[0];
		if (scrolled != 0 && window.mouseScroll[0] > 0)
			scrolled -= 10 * window.mouseScroll[0];

		SDL_RenderPresent(window.renderer);
	}

	enet_deinitialize();

	return 0;
}