
#ifndef PLATFORM
#define PLATFORM

#include "Visible.h"
#include "Solid.h"

class Platform: public Visible,
		public Solid
{
	public:
		Platform( SDL_Rect hbox ): GameObject( hbox )
		{
			loadTexture( "images/Platform.png" );
		}
};

#endif
