#include "Include.h"
#include "Window.h"
#include "ChatServer.h"
#include "ChatClient.h"

Window window;
ChatClient client;
ENetAddress address;
SDL_Rect messageRect;
SDL_Texture* renderTexture;
int scrolled;

int main(int argc, char** argv) {
	// If there is no second argument, die
	if (argc == 2) {
		fprintf(stderr, "Error: You need a username and dest IP to use this program! (Username: second argument, dest IP: third argument)\n");
		return 1;
	}

	// Init our window and our main font
	InitWindow(&window, 1024, 720, "Witter");
	InitFont(&srcFont, "assets/PublicSans-VariableFont_wght.ttf", 18);
	prewrittenMsg = srcFont;
	
	// Init ENet
	if (enet_initialize() != 0) {
		fprintf(stderr, "Error: Couldn't init ENet!");
        return EXIT_FAILURE;
    }

	// Set the IP and port we will connect to
	enet_address_set_host_ip(&address, argv[2]);
	address.port = 7777;

	// Set the text input rect
	messageRect = (SDL_Rect){40, 500, 944, 100};

	// Init the chat client
	if (InitChatClient(&client, &address))
		return 1;
	
	// Set the client name to second argument
	client.name = argv[1];

	prewrittenMsg.renderRect.x = messageRect.x + 4;
	prewrittenMsg.renderRect.y = messageRect.y + 4;
	
	// Start SDL Input
	SDL_StartTextInput();

	renderTexture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, window.width, 5000);

	while (WindowIsOpen(&client)) {
		SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
		SDL_RenderClear(window.renderer);

		// Render to the target
		SDL_SetRenderTarget(window.renderer, renderTexture);
		SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
		SDL_RenderClear(window.renderer);
		for (int i = 0; i < 200; i++) {
			if (messages[i] != NULL) {
				SDL_RenderCopy(window.renderer, messages[i]->tex, NULL, &messages[i]->renderRect);
			}
		}
		// Stop rendering to target
		SDL_SetRenderTarget(window.renderer, NULL);

		// Draw *the* target
		SDL_RenderCopy(window.renderer, renderTexture, &(SDL_Rect){0, scrolled, 944, 400}, &(SDL_Rect){40, 0, 944, 400});

		// Draw messages box
		SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(window.renderer, &(SDL_Rect){40, 0, 944, 400});
		SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);

		// Handle scrolling
		if (scrolled != 2000 - 400 && window.mouseScroll[0] < 0)
			scrolled -= 10 * window.mouseScroll[0];
		if (scrolled != 0 && window.mouseScroll[0] > 0)
			scrolled -= 10 * window.mouseScroll[0];

		// Draw the typing box and its outline
		SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(window.renderer, 128, 128, 128, 128);
		SDL_RenderDrawRect(window.renderer, &messageRect);
		SDL_SetRenderDrawColor(window.renderer, 128, 128, 128, 64);
		SDL_RenderFillRect(window.renderer, &messageRect);
		SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_NONE);

		// Send a message
		if (window.keys[SDL_SCANCODE_RETURN]) {
			if (textInput[0] != 0)
				SendData(&client, &textInput[0]);
			for (int i = 0; i < 500; i++)
				textInput[i] = 0;

			// Destroy texture so SDL2_TTF doesn't explode
			SDL_DestroyTexture(prewrittenMsg.tex);
			prewrittenMsg.tex = NULL;
		}

		// Delete a character
		if (window.keys[SDL_SCANCODE_BACKSPACE]) {
			for (int i = 0; i < 500; i++) {
				if (textInput[i] != 0 && textInput[i + 1] == 0) {
					printf("%c", textInput[i]);

					textInput[i] = 0;

					// Render texture if there is a remaining character, destroy texture if there isn't.
					if (i != 0)
						InitTextWrapped(&prewrittenMsg, textInput, 940, COLOUR_CLIENT);
					else {
						SDL_DestroyTexture(prewrittenMsg.tex);
						prewrittenMsg.tex = NULL;
					}
					
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(window.renderer, 255, 255, 255, 255);
		if (prewrittenMsg.tex) SDL_RenderCopy(window.renderer, prewrittenMsg.tex, NULL, &prewrittenMsg.renderRect);
		SDL_RenderPresent(window.renderer);
	}

	enet_peer_disconnect(client.peer, 0);
	enet_host_destroy(client.client);

	SDL_StopTextInput();
	enet_deinitialize();
	SDL_Quit();

	return 0;
}