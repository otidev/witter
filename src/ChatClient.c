#include "ChatClient.h"

Font* messages[200] = {NULL};
Font srcFont;
char messageBuf[1000];

int InitChatClient(ChatClient* client, ENetAddress* serverAddress) {
	client->client = enet_host_create(NULL, 1, 1, 0, 0);
	if (client->client == NULL) {
		fprintf(stderr, "Error: Couldn't init an ENet client.\n");
		return 1;
	}
	
	client->peer = enet_host_connect(client->client, serverAddress, 1, 0);
	if (client->peer == NULL) {
		fprintf(stderr, "Error: No peers available.\n");
		return 1;
	}

	if (enet_host_service(client->client, &client->event, 5000) > 0 && client->event.type == ENET_EVENT_TYPE_CONNECT) {
		fprintf(stderr, "Connection established!");
	} else {
		enet_peer_reset(client->peer);
		fprintf(stderr, "Connection failed!");
		return 1;
	}

	return 0;
}

void StoreMessage(ChatClient* client, char* text) {
	for (int i = 0; i < 200; i++) {
		if (messages[i] == NULL) {
			messages[i] = malloc(sizeof(Font));
			*(messages[i]) = srcFont;
			InitTextWrapped(messages[i], text, globalWindow->width - 2, (SDL_Colour){0, 0, 0, 255});
			
			messages[i]->renderRect.x = 2;
			if (messages[i - 1] != NULL)
				messages[i]->renderRect.y = messages[i - 1]->renderRect.y + messages[i - 1]->renderRect.h + 2;
			else
				messages[i]->renderRect.y = (i * 18) + 2;
			break;
		}
	}
}

void HandleClientEvents(ChatClient* client) {
	while (enet_host_service(client->client, &client->event, 10) > 0) {
		switch (client->event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n", client->event.peer->address.host, client->event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n", client->event.packet->dataLength, client->event.packet->data, (char*)client->event.peer->data, client->event.channelID);
				StoreMessage(client, (char*)client->event.packet->data);
				enet_packet_destroy(client->event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", (char*)client->event.peer->data);
				StoreMessage(client, "Server disconnected!");
				break;
			case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}

void SendData(ChatClient* client, char* data) {
	snprintf(messageBuf, 1000, "<%s@%x:%u>: %s", client->name, client->peer->address.host, client->peer->address.port, data);
	client->packet = enet_packet_create(messageBuf, strlen(messageBuf) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(client->peer, 0, client->packet);
}