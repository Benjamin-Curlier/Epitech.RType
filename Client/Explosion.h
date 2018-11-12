#pragma once
#include "IObject.h"

struct Explosion_Type
{
	Explosion_Type(std::vector<S_sprite *> s, sf::Vector2f a)
	{
		this->_center = a;
		this->_sprites = s;
	}
	std::vector<S_sprite *> _sprites;
	sf::Vector2f _center;
};

class Explosion : public IObject
{
public:
	Explosion(sf::RenderWindow* w, sf::Vector2f pos, std::vector<S_sprite *> s, Explosion_Type *m);
	~Explosion();
	virtual Action_Update update(const float);
private:
	sf::Clock clock;
	Explosion_Type *_explosion_type;
};

