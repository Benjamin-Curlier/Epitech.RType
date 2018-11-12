#pragma once

#include "IObject.h"

class ScrollingBackground : public IObject
{
public:
	ScrollingBackground(sf::RenderWindow*, sf::Vector2f, std::vector<S_sprite *>);
	~ScrollingBackground();
	virtual Action_Update update(const float);
};

