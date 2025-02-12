#pragma once

class C_Rectangle
{
	private:
		double x;
		int y;
		int width;
		int height;
		Vector2 MonitorProps;
	public:
		C_Rectangle();
		void draw();
		void move(double n);
		Vector2 getPos() const;
		Vector2 getSize() const;
		~C_Rectangle();
};
