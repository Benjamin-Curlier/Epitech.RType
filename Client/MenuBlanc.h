#pragma once

#include "IObject.h"

class MenuBlanc : public IObject
{
public:
	MenuBlanc(sf::RenderWindow*, sf::Vector2f, std::vector<S_sprite *>);
	~MenuBlanc();
	virtual Action_Update update(const float);
};
