#include "draw.h"

#include <stdio.h>
#include "SDL2_gfxPrimitives.h"

#define WIDTH 640
#define HEIGHT 480

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Event e;

int init_window() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf ("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("SDL2_gfx test", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		printf ("SDL_CreateWindow Error: %s", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		printf ("SDL_CreateRenderer Error: %s", SDL_GetError());
		SDL_Quit();
		return 3;
	}
	return 0;
}

int get_key_event(char *k) {
	if(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
			return -1;
		case SDL_KEYDOWN:
			printf("%c\n", e.key.keysym.sym);
			*k = e.key.keysym.sym;
			return 1;
		default:
			return 0;
		}
	}
}

int show_shapes(int nshapes, shape **ss) {
	if(!renderer) init_window();


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	shape *s = NULL;
	for(int i = 0; i < nshapes; i++) {
		s = ss[i];
		//print_shape(s);
		for(int i = 0; i < s->proto->nlines; i++) {
			float *p1 = s->pts[s->proto->lines[i*2]];
			float *p2 = s->pts[s->proto->lines[i*2+1]];
			/*
			   printf("%f, %f, %f, %f\n\n",
			   p1[0] + WIDTH/2, p1[1] + HEIGHT/2,
			   p2[0] + WIDTH/2, p2[1] + HEIGHT/2);
			   */
			lineColor(renderer,
					p1[0] + WIDTH/2, p1[1] + HEIGHT/2,
					p2[0] + WIDTH/2, p2[1] + HEIGHT/2,
					0xF0F0F0FF);
		}
	}

	SDL_RenderPresent(renderer);
	//SDL_Delay(10);
	return 0;
}
