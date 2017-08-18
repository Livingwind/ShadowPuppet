
#ifndef MOVEABLE
#define MOVEABLE

#include "GameObject.h"

class Movable: public virtual GameObject{
	public:
		virtual void update() override {}
};

#endif
