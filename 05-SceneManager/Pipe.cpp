#include "Pipe.h"

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - weight / 2;
	r = l + width;
	b = t + weight;
}
