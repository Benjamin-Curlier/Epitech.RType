#pragma once

#include "IObject.h"

class MenuButton : public IObject
{
public:
	MenuButton(sf::RenderWindow*, sf::Vector2f, std::vector<S_sprite *>);
	~MenuButton();
	virtual Action_Update update(const float);
};

