#include "draw.h"

#include <stdio.h>
#include "SDL2_gfxPrimitives.h"

#define WIDTH 640
#define HEIGHT 480

int show_shape(shape *s) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf ("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("SDL2_gfx test", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		printf ("SDL_CreateWindow Error: %s", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		printf ("SDL_CreateRenderer Error: %s", SDL_GetError());
		SDL_Quit();
		return 3;
	}

	SDL_Event e;

	int quit = 0;
	while (!quit) {
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		for(int i = 0; i < s->nlines; i++) {
			float *p1 = s->pts[s->lines[i*2]];
			float *p2 = s->pts[s->lines[i*2+1]];
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

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
