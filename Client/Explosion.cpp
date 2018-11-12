#include "Explosion.h"



Explosion::Explosion(sf::RenderWindow* w, sf::Vector2f pos, std::vector<S_sprite *> s, Explosion_Type *m) : IObject(w, pos, s)
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
	_sprites.at(0)->_sprite.setPosition(_pos);
	this->_w->draw(_sprites.at(0)->_sprite);
	return Action_Update::NOTHING;
}