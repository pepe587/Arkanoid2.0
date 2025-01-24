#pragma once

class C_Rectangle
{
	private:
		int x;
		int y;
		int width;
		int height;
	public:
		C_Rectangle();
		void draw();
		void move(int n);
		Vector2 getPos() const;
		Vector2 getSize() const;
		~C_Rectangle();
};
