#include <iostream>
#include <string>
#include "SDL.h"
#include <SDL_opengl.h>
#include <SDL_image.h>

using namespace std

class Player{                       //explore x/y acceleration
	
	
	static int x, y, xvel, yvel;
	static bool falling = false;


	public Player(int x, int y){
	this->x = x;
	this->y = y;
    xvel = 5;
    yvel = 0;
	}

	// all jumping sets the 
	void jump(){
		if(!falling)
		    yvel = 12;
		    falling = true;
	}
	//move is given a char to indicate what direction the character is moving
	void move(char direction){
        
         switch(direction){
                //move right
                case 'r':
                    x += xVel;
                    break;
                //move left
                case 'l':
                    x -= xVel;
                    break;
                //jump
				case 'u':
					if(!falling){
						jump();
        
					}
					break;
            }
    }


	//border collision take into account that it looks at top left corner
	void border collision(){
        if(y>= 440)
        falling = false;
        yvel =0;

        if(falling){
            y += yvel;
            //terminal velocity
            if(yvel >= -15){
                yvel-= 1;
                y += yvel;
            }
        }
	}
}
