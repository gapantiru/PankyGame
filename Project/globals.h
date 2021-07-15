#ifndef GLOBALS_H
#define GLOBALS_H


namespace globals {
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 608;
	
	const float SPRITE_SCALE = 2.0f;
}

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};
	inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};


struct Vector2 {
	int x, y;
	Vector2() :
		x(0), y(0)
	{}
	Vector2(int x, int y) :
		x(x), y(y)
	{}
	Vector2 zero() {
		return Vector2(0, 0);
	}
	bool operator >(const Vector2 &vec) {
		if (this->x > vec.x && this->y > vec.y)
			return true;
		else return false;
	}
};

struct Box {
	int x, y, w, h;

	Box():
		x(0),
		y(0),
		w(0),
		h(0)
	{}

	Box(int x,int y,int w,int h):
		x(x),
		y(y),
		w(w),
		h(h)
	{}

};



#endif