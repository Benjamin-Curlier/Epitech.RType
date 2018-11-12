#pragma once

#include "Game.h"

class GameState
{
public:
	Game *game;
	virtual void Draw(const float dt) = 0;
	virtual void Update(const float dt) = 0;
	virtual void HandleInput() = 0;
};