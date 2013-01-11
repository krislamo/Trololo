#include "main.h"

using namespace std;

namespace scrnfunk
{
	// Optimize image on load to reduce processing.
	SDL_Surface *load_image(const char *file)
	{
		SDL_Surface *image_format;
		SDL_Surface *screen_format;

		// Load image
		image_format = IMG_Load(file);

		/*
		    DOES THE FILE EVEN EXIST?
		*/

		if(image_format != 0)
		{
		    // Optimize
		    screen_format = SDL_DisplayFormat(image_format);
		
		    // Release old image
		    SDL_FreeSurface(image_format);
		}

		return screen_format;
	}
	
	void apply_image(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
	{
		SDL_Rect pos;

		pos.x = x;
		pos.y = y;

		SDL_BlitSurface(source, clip, destination, &pos);
	}

	// Removes color from an image.
	bool RemoveColor(SDL_Surface* image, Uint8 R, Uint8 G, Uint8 B)
	{
		int result;

		// Map color on the image
		Uint32 color = SDL_MapRGB(image->format, R, G, B);

		// Remove color from the image.
		result = SDL_SetColorKey(image, SDL_SRCCOLORKEY, color);

		if(!result)
		    return true;
		else 
		    return false;
	}
}


namespace scrns
{
	// Returns a pointer on success, NULL on an error
	SDL_Surface * initiateSDL(SDL_Surface *screen, int x, int y)
	{
		SDL_Surface *vid_init;
		int init, ttf_ret;
	
		// Initiate SDL and check
		init = SDL_Init(SDL_INIT_VIDEO);
		if(init < 0)
		{
			fprintf(stderr, "Unable to initiate SDL: %s\n", SDL_GetError());
			return NULL;
		}

		
		// Set video and check
		vid_init = SDL_SetVideoMode(x, y, 32, SDL_SWSURFACE);
		if(vid_init == 0)
		{
		    fprintf(stderr, "Video initialization failed: %s", SDL_GetError());
		    return NULL;
		}


		// Get video surface and check
		screen = SDL_GetVideoSurface();
		if(screen == NULL)
		{
		    fprintf(stderr, "Failed to get video surface: %s", SDL_GetError());
		    return NULL;
		}

		// Set window title
		SDL_WM_SetCaption(WINDOW_TITLE " " VERSION, 0);


		// Initialize SDL_ttf and check
		ttf_ret = TTF_Init();
		if(ttf_ret == -1)
		{
		    fprintf(stderr, "True Type Font initialization failed: %s", SDL_GetError());
		    return NULL;
		}
		
		SDL_FreeSurface(vid_init);

		// Success
		return screen;
	}

	SDL_Surface * MainMenu()
	{
		// Screen
		SDL_Surface *screen = NULL;

		SDL_Surface *background = NULL;
		SDL_Surface *characters = NULL;
		SDL_Surface *message = NULL;
		SDL_Surface *indicator = NULL;

		// Image filenames
		const char *background_image = "bin/stage1.bmp";
		const char *characters_image = "bin/characters.bmp";
		const char *indicator_image = "bin/indicator.bmp";

		// Char
		SDL_Rect clip[2];
		
		screen = initiateSDL(screen, 900, 675);

		if(screen == NULL)
		{
			fprintf(stderr, "Start up failure.\n");
			return NULL;
		}
		else
		{
			// Load images
			background = scrnfunk::load_image(background_image);
			characters = scrnfunk::load_image(characters_image);
			indicator = scrnfunk::load_image(indicator_image);
		
			// The troll's coordinates 
			clip[0].x = 0;
			clip[0].y = 0;
			clip[0].w = 200;
			clip[0].h = 160;
		
			// The codfish's coordinates
			clip[1].x = 0;
			clip[1].y = 160;
			clip[1].w = 200;
			clip[1].h = 100;
		
			// Remove the background from the characters
			scrnfunk::RemoveColor(characters, 0x00, 0xFF, 0x00); // Green
		
			// Blit background image
			scrnfunk::apply_image(0, 0, background, screen);
		
			// Blit troll image
			scrnfunk::apply_image(700, 40, characters, screen, &clip[0]);
		
			// Blit codfish image
			scrnfunk::apply_image(150, 300, characters, screen, &clip[1]);

			// Open font at title size.
			TTF_Font *font = TTF_OpenFont("bin/CaviarDreams.ttf", 80);
		
		
			if(!font)
			{
				fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
				return NULL;
			}

			// Setup Title text
			const char* title_text = "Welcome to Trololo";
			SDL_Color whitecolor = { 255, 255, 255 };
			
			// Get Title text render
			if(font != 0 && title_text != 0) // 0 into the text or font causes segfaults and undefined behavior
				message = TTF_RenderText_Solid(font, title_text, whitecolor);
			else
				fprintf(stderr, "TFF Render Text Solid Error: init error\n");
			
			// Apply Title text render
			if(message != 0)
				scrnfunk::apply_image(100, 200, message, screen);
			else
				fprintf(stderr, "TFF Render Text Solid Error: apply error\n");
			
			// Setup option 1 (New game)
			const char* option1_text = "New game";
			SDL_Color blackcolor = { 0,0,0 };
			font = TTF_OpenFont("bin/CaviarDreams.ttf", 55); // reopen to resize
			
			// Apply option 1 (New game)
			if(font != 0 && option1_text != 0)
				message = TTF_RenderText_Solid(font, option1_text, blackcolor);
			else
				fprintf(stderr, "TFF Render Text Solid Error: init error\n");
		
			// Apply message option 1 (New game)
			if(message != 0)
				scrnfunk::apply_image(620, 300, message, screen);
			else
				fprintf(stderr, "TFF Render Text Solid Error: apply error\n");
			
			// Setup exit option (Exit)
			const char* exit_text = "Exit";
			font = TTF_OpenFont("bin/CaviarDreams.ttf", 55); // reopen to resize
			
			// Apply exit option (Exit)
			if(font != 0 && option1_text != 0)
				message = TTF_RenderText_Solid(font, exit_text, blackcolor);
			else
				fprintf(stderr, "TFF Render Text Solid Error: init error\n");
		
			// Apply exit option (Exit)
			if(message != 0)
				scrnfunk::apply_image(620, 370, message, screen);
			else
				fprintf(stderr, "TFF Render Text Solid Error: apply error\n");
			
			// Remove white around the indicator image
			scrnfunk::RemoveColor(indicator, 0xFF, 0xFF, 0xFF); // white
			
			// Apply starting point of indicator
			scrnfunk::apply_image(550, 306, indicator, screen);
		
			// Update screen with all applied images
			if(screen != 0)
				SDL_Flip(screen);
			else
				fprintf(stderr, "Couldn't update screen.\n");
			
			// Free old surfaces
			SDL_FreeSurface(background);
			SDL_FreeSurface(characters);
			SDL_FreeSurface(message);
			SDL_FreeSurface(indicator);
		}
	
		return screen;
	}

	int MainMenu_survey()
	{
		// Return values
		// 1 -- User wants option 1 (start new game)
		// 2 -- User wants to quit

		SDL_Event event;
		
		int menu_option, current;
		menu_option = current = 1;

		bool option_wanted = false;
		
		
		while(!option_wanted)
		{
			while(SDL_PollEvent(&event))
			{
			  
				if(event.type == SDL_QUIT)
					return 2;
				
				else if(event.type == SDL_KEYDOWN)
				{
				  
					if(event.key.keysym.sym == 274) // down arrow key
					{
						if(menu_option < 2) // Less than the max amount of options
							menu_option++;
						
						//cout << "INPUT ::: DOWN KEY, menu_option: " << menu_option << "; current: " << current << endl; // debugline
					}
					
					else if(event.key.keysym.sym == 273) // up arrow key
					{
						if(menu_option > 1)
							menu_option--;
						
						//cout << "INPUT ::: UP KEY, menu_option: " << menu_option << "; current: " << current << endl; // debugline
					}
					
					else if(event.key.keysym.sym == 13) // enter key
						option_wanted = true;
				}

				
				
				if(menu_option != current)
				{
					current = menu_option;
					
					switch(menu_option)
					{
						case 1:
							//cout << "Case 1 ::: Menu should change.  menu_option: " << menu_option << "; current: " << current << endl; // debugline
							break;
						case 2:
							//cout << "Case 2 ::: Menu should change.  menu_option: " << menu_option << "; current: " << current << endl; // debugline
							break;
						default:
							cout << "Error: MainMenu_survey didn't update correctly." << endl;
							break;
					}
				}
			}
		}
		
		return menu_option;
	}


}
