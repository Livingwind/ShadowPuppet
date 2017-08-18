
#ifndef VISABLE
#define VISABLE

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "GameObject.h"

class Visible: public virtual GameObject{
	public:
		virtual void draw() override {
			cout << hbox.x << ' ' << hbox.y << ' ' << hbox.w << ' ' << hbox.h << endl;
			glBindTexture( GL_TEXTURE_2D, texture );
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(hbox.x, hbox.y);
        		glTexCoord2f(1, 0); glVertex2f(hbox.x + hbox.w, hbox.y);
        		glTexCoord2f(1, 1); glVertex2f(hbox.x + hbox.w, hbox.y + hbox.h);
        		glTexCoord2f(0, 1); glVertex2f(hbox.x, hbox.y+hbox.h);
    			glEnd();
		}
	protected:
		GLuint texture;

		void loadTexture( string filename ){
			SDL_Surface *surface = IMG_Load( filename.data() );
			if( !surface ){
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					     "Couldn't load %s.", filename.data() );
			}
			else return;

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w,
				     surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			SDL_FreeSurface(surface);
		}
};

#endif
