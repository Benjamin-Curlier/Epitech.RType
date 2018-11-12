#include "Missile.h"



Missile::Missile(sf::RenderWindow* w, sf::Vector2f pos, std::vector<S_sprite *> s, Missile_Type *m) : IObject(w, pos, s)
{
	this->_missile_type = m;
}


Missile::~Missile()
{
}

Action_Update Missile::update(const float dt)
{
	if (this->_is_alive == false)
		return Action_Update::TO_DELETE;
	if (this->_missile_type->_left == true)
		this->_pos.x -= dt * 1200;
	else
		this->_pos.x += dt * 1200;
	if (this->_pos.x > 1920 + this->_missile_type->_off_map.first.y || this->_pos.x  < this->_missile_type->_off_map.first.x)
		return Action_Update::TO_DELETE;
	_sprites.at(0)->_sprite.setPosition(this->_pos);
	_w->draw(_sprites.at(0)->_sprite);
	return Action_Update::NOTHING;
}