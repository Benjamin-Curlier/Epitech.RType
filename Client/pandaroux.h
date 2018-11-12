#pragma once

#include "IObject.h"

class pandaroux : public IObject
{
public:
	pandaroux(sf::RenderWindow*, sf::Vector2f, std::vector<S_sprite *>);
	~pandaroux();
	virtual Action_Update update(const float);
};
