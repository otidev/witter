#include "ChatServer.h"

Font* serverMessages[200] = {NULL};
Font serverSrcFont;
char* serverMessageBuf;

int InitChatServer(ChatServer* server, int port) {
	// Set address and port of server
	enet_address_set_host_ip(&server->address, "127.0.0.1");
	server->address.port = port;

	// Init server
	server->server = enet_host_create(&server->address, 32, 1, 0, 0);
	if (server->server == NULL) {
		fprintf(stderr, "Error: Couldn't init an ENet server.\n");
		return 1;
	}

	return 0;
}

void StoreMessageServer(ChatServer* server, char* text, SDL_Colour colour) {
	for (int i = 0; i < 200; i++) {
		if (serverMessages[i] == NULL) {
			serverMessages[i] = malloc(sizeof(Font));
			*(serverMessages[i]) = serverSrcFont;
			InitTextWrapped(serverMessages[i], text, globalWindow->width - 2, colour);

			serverMessages[i]->renderRect.x = 2;
			if (serverMessages[i - 1] != NULL)
				serverMessages[i]->renderRect.y = serverMessages[i - 1]->renderRect.y + serverMessages[i - 1]->renderRect.h + 2;
			else
				serverMessages[i]->renderRect.y = (i * 18) + 2;
			
			break;
		}

	}
}

static void SendMessageToPeers(ChatServer* server, char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->server, 0, packet);
}

void HandleServerEvents(ChatServer* server) {
	while (enet_host_service(server->server, &server->event, 10) > 0) {
		switch (server->event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n", server->event.peer->address.host, server->event.peer->address.port);
				server->event.peer->data = malloc(500);
				sprintf(server->event.peer->data, "%x:%u has joined!", server->event.peer->address.host, server->event.peer->address.port);
				
				StoreMessageServer(server, server->event.peer->data, COLOUR_CLIENT);
				SendMessageToPeers(server, server->event.peer->data);
				
				sprintf(server->event.peer->data, "%x:%u", server->event.peer->address.host, server->event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n", server->event.packet->dataLength, server->event.packet->data, (char*)server->event.peer->data, server->event.channelID);
				StoreMessageServer(server, (char*)server->event.packet->data, COLOUR_CLIENT);
				SendMessageToPeers(server, (char*)server->event.packet->data);
				enet_packet_destroy(server->event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", (char*)server->event.peer->data);
				sprintf(server->event.peer->data, "See ya, %x:%u!", server->event.peer->address.host, server->event.peer->address.port);
				
				StoreMessageServer(server, server->event.peer->data, COLOUR_CLIENT);
				SendMessageToPeers(server, server->event.peer->data);
				
				free(server->event.peer->data);
				server->event.peer->data = NULL;
			case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}
