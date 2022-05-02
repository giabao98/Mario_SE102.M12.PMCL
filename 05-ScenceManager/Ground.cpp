#include "Ground.h"
Ground::Ground(float width, float height) : Block(width, height){
	blockType = BlockType::GROUND;
}