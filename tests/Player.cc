#include "Player.h"


	
	
	Player::Player(int x, int y){
	this.x = x;
	this.y = y;
	xvel = 5;
	falling = false;
	}

	// all jumping/falling
	void Player::jump(){
		if(!falling){
		    yvel = -12;
		    falling = true;
		}
	}	
	//moves in the respective direction
	void Player::moveUp(){
		jump();
		y+= yvel;
	}
	void Player::movedown(){
		yvel += 1;
		y+= yvel;
	}
	void Player::moveLeft(){
		x=+ xvel;
	}
	void Player::moveRight(){
		x+= xvel;
	}
	//border collision take into account that it looks at top left corner
	void Player::borderCollision(){
		//this case accounts for hitting the ground
		if(y >= 440){
		   falling = false;
		   yvel = 0;
		   y = 440;
		}
		// if it the player tries to go past the roof
		if(y <= 0){
		   falling = true;
		   y = 0;
		}
		//the left side...
		if(x <= 0){
		   x = 0;
		}
		//finally the right
		if(x >= 600){	
		   x = 600;
		}
	
	}
	

int main(){
	Player p = new Player(0,440);
	//testing the starting coordinates
	cout << "should be 0: " << p.x << " should be 440: "<< p.y << "\n";

}
