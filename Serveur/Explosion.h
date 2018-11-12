#pragma once
#include "IObject.h"

struct Explosion_Type
{
	Explosion_Type( sf::Vector2f a)
	{
		this->_center = a;
	}
	sf::Vector2f _center;
};

class Explosion : public IObject
{
public:
	Explosion(sf::Vector2f pos, Explosion_Type *m);
	~Explosion();
	virtual Action_Update update(const float);
private:
	sf::Clock clock;
	Explosion_Type *_explosion_type;
};

