#ifndef FONT_H
#define FONT_H

#include "Include.h"

void InitFont(Font* font, char* filename, int fontSize);

void InitTextWrapped(Font* font, char* text, int wrapWidth, SDL_Colour colour);

#endif