
#ifndef SOLID
#define SOLID

#include "GameObject.h"

class Solid: public virtual GameObject{
	public:
		virtual void collide( GameObject object )
			override {
		}
};

#endif
