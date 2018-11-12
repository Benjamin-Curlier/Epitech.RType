#pragma once

#include "IObject.h"

class AlmostPopUp : public IObject
{
public:
	AlmostPopUp(sf::RenderWindow*, sf::Vector2f, std::vector<S_sprite *>);
	~AlmostPopUp();
	virtual Action_Update update(const float);
};