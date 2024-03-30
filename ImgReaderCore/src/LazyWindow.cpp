#include "LazyWindow.h"

#ifndef NOT_ADD_WINDOW_SHOW_MODULES

#include "SDL2/SDL.h"
#include "LazyImgConfig.h"


static int is_sdl_init = 0;

bool ImgShow(std::string window_name, lazyimg::Mat* mat, int width, int height)
{
	if (is_sdl_init == 0)
	{
		//SDL_初始化
		is_sdl_init = SDL_Init(SDL_INIT_VIDEO);
		if (is_sdl_init == -1)
		{
			printf("初始化SDL失败!\n");
			return false;
		}
	}

	//创建窗口

	SDL_Window* win = SDL_CreateWindow(window_name.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	//创建渲染器

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
	case pformat::U8_RGBA:
		pixformat = SDL_PIXELFORMAT_ABGR8888;
		break;
	default:
		SDL_DestroyRenderer(ren);
		return false;
	}

	//创建材质

	SDL_Texture* tex = nullptr;
	tex = SDL_CreateTexture(ren, pixformat, SDL_TEXTUREACCESS_STREAMING, mat->x, mat->y);

	//清空渲染器
	SDL_RenderClear(ren);

	//退出
	int quit = 0;

	//监听退出活动
	SDL_Event event;

	//将材质复制到渲染器
	int stride = (int)mat->x * mat->channel * mat->bit_depth;
	SDL_UpdateTexture(tex, nullptr, mat->at<uint8_t>(), stride);

	SDL_Rect rect = { .x = 0,.y = 0, .w = width,.h = height };

	// 将更新后的纹理拷贝到渲染器
	SDL_RenderCopy(ren, tex, nullptr, &rect);

	// 渲染器显示画面
	SDL_RenderPresent(ren);

	while (!quit)
	{
		//主消息循环
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			//用户从菜单要求退出程序
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}

	//清理资源
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	return true;
}

#endif // NOT_ADD_WINDOW_SHOW_MODULES IS END