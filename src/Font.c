#include "Font.h"

void InitFont(Font* font, char* filename, int fontSize) {
	font->font = TTF_OpenFont(filename, fontSize);
}

void InitTextWrapped(Font* font, char* text, int wrapWidth, SDL_Colour colour) {
	SDL_Surface* surf = TTF_RenderUTF8_Blended_Wrapped(font->font, text, colour, wrapWidth);
	font->renderRect.w = surf->w;
	font->renderRect.h = surf->h;
	font->tex = SDL_CreateTextureFromSurface(globalWindow->renderer, surf);
	SDL_FreeSurface(surf);
}