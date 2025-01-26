#pragma once

enum typeOfBricks{
	VOID = 0,
	INDESTRUCTIBLE = 1,
	SIMPLE = 2,
	RESISTANT = 3
};

class Brick
{
	private:
		short type;
		Vector2 pos;
		Vector2	size;
		short	hits;
		Color	color;
	public:
		Brick(short _type, Vector2 _pos, Vector2 _size);
		void Draw();
		bool Hit();
		bool ImBreakable();
		Vector2 getPos() const;
		Vector2 getSize() const;
		~Brick();
};


