#include "LazyWindow.h"

#ifndef NOT_ADD_WINDOW_SHOW_MODULES

#include "SDL2/SDL.h"
#include "LazyImgConfig.h"


static int is_sdl_init = 0;

bool ImgShow(std::string window_name, lazyimg::Mat* mat, int width, int height)
{
	if (is_sdl_init == 0)
	{
		// Init SDL
		is_sdl_init = SDL_Init(SDL_INIT_VIDEO);
		if (is_sdl_init == -1)
		{
			printf("Init SDL Falided!\n");
			return false;
		}
	}

	// Create Window

	SDL_Window* win = SDL_CreateWindow(window_name.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// Create Render

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

	if (win == nullptr || ren == nullptr)
		printf("%s\n", SDL_GetError());

	Uint32 pixformat = 0;
	switch (mat->color_type)
	{
	case pformat::U8_YUV:
		pixformat = SDL_PIXELFORMAT_YV12;
		break;
	case pformat::U8_RGB:
		pixformat = SDL_PIXELFORMAT_RGB24;
		break;
	case pformat::U8_ABGR:
		pixformat = SDL_PIXELFORMAT_ABGR8888;
		break;
	case pformat::U8_RGBA:
		pixformat = SDL_PIXELFORMAT_RGBA8888;
		break;
	default:
		SDL_DestroyRenderer(ren);
		return false;
	}

	// Create Texture

	SDL_Texture* tex = nullptr;
	tex = SDL_CreateTexture(ren, pixformat, SDL_TEXTUREACCESS_STREAMING, mat->x, mat->y);

	//clear
	SDL_RenderClear(ren);

	// exit number
	int quit = 0;

	SDL_Event event;

	int stride = (int)mat->x * mat->channel * mat->bit_depth;
	SDL_UpdateTexture(tex, nullptr, mat->at<uint8_t>(), stride);

	SDL_Rect rect = { .x = 0,.y = 0, .w = width,.h = height };

	// ReSize IMG Texture
	SDL_RenderCopy(ren, tex, nullptr, &rect);

	// Show
	SDL_RenderPresent(ren);

	while (!quit)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			// EXIT COMMAND
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}

	// Release
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	return true;
}

#endif // NOT_ADD_WINDOW_SHOW_MODULES IS END