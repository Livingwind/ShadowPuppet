#ifndef PLAYER_HEADER
#define PLAYER_HEADER


class Player{

	public: 
	int x, y, xvel, yvel;
	bool falling;
	Player(int, int);
	void jump();
	void moveUp();
	void movedown();
	void moveLeft();
	void moveRight();
	void borderCollision();
};

#endif
