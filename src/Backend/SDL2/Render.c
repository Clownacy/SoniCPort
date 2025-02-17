#include "SDL_render.h"
#include "SDL_timer.h"

#include "../VDP.h"

#include <stdio.h>

//Render compile options
//#define DISPLAY_PADDING //Displays the internal VDP padding

#ifdef DISPLAY_PADDING
	#define TEXTURE_WIDTH  (SCREEN_WIDTH  + (VDP_INTERNAL_PAD * 2))
	#define TEXTURE_HEIGHT SCREEN_HEIGHT
#else
	#define TEXTURE_WIDTH  SCREEN_WIDTH
	#define TEXTURE_HEIGHT SCREEN_HEIGHT
#endif

//Icon
static uint8_t icon_data[] = {
	#include <Resource/Icon.h>
};

//Window and renderer
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

//Render state
static int vsync;

//Backend render interface
int Render_Init(const MD_Header *header)
{
	//Create window
	if ((window = SDL_CreateWindow(header->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TEXTURE_WIDTH * SCREEN_SCALE, TEXTURE_HEIGHT * SCREEN_SCALE, SDL_WINDOW_HIDDEN)) == NULL)
	{
		printf("Render_Init: %s\n", SDL_GetError());
		return -1;
	}
	
	//Load icon
	SDL_Surface *icon_surface;
	if ((icon_surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)icon_data, 16, 16, 24, 16*3, SDL_PIXELFORMAT_RGB24)) == NULL)
	{
		printf("Render_Init: %s\n", SDL_GetError());
	}
	else
	{
		SDL_SetWindowIcon(window, icon_surface);
		SDL_FreeSurface(icon_surface);
	}
	
	//Show window now that the icon's been loaded
	SDL_ShowWindow(window);
	
	//Check if VSync should be used
	SDL_DisplayMode display_mode;
	SDL_GetWindowDisplayMode(window, &display_mode);
	if (display_mode.refresh_rate > 0 && (display_mode.refresh_rate % 60) == 0)
		vsync = display_mode.refresh_rate / 60;
	else
		vsync = 0;
	
	//Create renderer
	if ((renderer = SDL_CreateRenderer(window, -1, vsync ? SDL_RENDERER_PRESENTVSYNC : 0)) == NULL)
	{
		printf("Render_Init: %s\n", SDL_GetError());
		return -1;
	}
	
	//Create screen texture
	if ((texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT)) == NULL)
	{
		printf("Render_Init: %s\n", SDL_GetError());
		return -1;
	}
	
	return 0;
}

void Render_Quit()
{
	//Destroy screen texture
	if (texture != NULL)
		SDL_DestroyTexture(texture);
	
	//Destroy window and renderer
	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	if (window != NULL)
		SDL_DestroyWindow(window);
}

//This takes in the internal VDP screen buffer positioned after the padding
void Render_Screen(const uint32_t *screen)
{
	//Framerate limiter (when VSync is unavailable)
	if (!vsync)
	{
		static const unsigned int delays[3] = {17, 16, 17};
		static unsigned int counter;
		
		static uint32_t time_prev;
		const uint32_t time_now = SDL_GetTicks();
		const uint32_t time_next = time_prev + delays[counter % 3];
		
		if (time_now >= time_prev + 100)
		{
			time_prev = time_now;
		}
		else
		{
			if (time_now < time_next)
				SDL_Delay(time_next - time_now);
			time_prev += delays[counter % 3];
		}
		
		counter++;
	}
	
	//Lock screen texture
	uint8_t *to;
	int pitch;
	SDL_LockTexture(texture, NULL, (void**)&to, &pitch);
	
	//Copy screen
	#ifdef DISPLAY_PADDING
		screen -= VDP_INTERNAL_PAD;
	#endif
	for (size_t i = 0; i < TEXTURE_HEIGHT; i++)
	{
		memcpy(to, screen, TEXTURE_WIDTH << 2);
		to += pitch;
		screen += SCREEN_WIDTH + (VDP_INTERNAL_PAD * 2);
	}
	
	//Unlock screen texture and draw to window
	SDL_UnlockTexture(texture);
	
	for (int i = 0; i < (vsync == 0 ? 1 : vsync); i++)
	{
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}
