#include "Pipe.h"

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - Width / 2;
	t = y - Height / 2;
	r = l + Width;
	b = t + Height;
}
