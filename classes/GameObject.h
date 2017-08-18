
#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <SDL.h>
#include <iostream>
#include <string>

using namespace std;

class GameObject{
	public:
		GameObject( SDL_Rect hbox ){
			this->hbox.x = hbox.x - hbox.w / 2 / (640.0/1080);
			this->hbox.y = hbox.y - hbox.h / 2 / (480.0/720);
			this->hbox.w = hbox.w;
			this->hbox.h = hbox.h;
		}
		GameObject(){}
		virtual ~GameObject(){}
		virtual void update(){}
		virtual void draw(){}
		virtual void collide( GameObject obj ){}

		SDL_Rect hbox;
};

#endif
