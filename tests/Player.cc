class Player{                       //explore x/y acceleration
	
	
	int x, y, xvel, yvel.
	bool falling = false;


	Player(int x, int y){
	this->x = x;
	this->y = y;
	}

	// all jumping/falling
	jumping(){
		if(!falling)
		    yvel = 12;
		    falling = true;
	}	


	//border collision take into account that it looks at top left corner
	border collision()
	if(y>= 440)
	   falling = false;

	   yvel =0;

	if(falling)
	   yvel-= 1;
	}