#pragma once
#include "TetrisShape.h"
 class OShape : public TetrisShape
{
public:
	OShape();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void Rotate();
	void ScaleUp();
	void ScaleDown();
private:

};