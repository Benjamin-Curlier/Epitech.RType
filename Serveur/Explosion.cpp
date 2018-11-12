#include "Explosion.h"



Explosion::Explosion(sf::Vector2f pos, Explosion_Type *m) : IObject(pos)
{
	this->_explosion_type = m;
	this->clock.restart();
}


Explosion::~Explosion()
{
}

Action_Update Explosion::update(const float dt)
{
	if (this->clock.getElapsedTime().asSeconds() > 1)
		return Action_Update::TO_DELETE;
	return Action_Update::NOTHING;
}