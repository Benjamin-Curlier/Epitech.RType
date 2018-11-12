#include "Bonus.h"



Bonus::Bonus(sf::Vector2f pos, Bonus_Type *m) : IObject(pos)
{
	this->_bonus_type = m;
}


Bonus::~Bonus()
{
}

Action_Update Bonus::update(const float dt)
{
	if (this->_is_alive == false)
		return Action_Update::TO_DELETE;
	this->_pos.x -= dt * 400;
	if (this->_pos.x > 1920 + this->_bonus_type->_off_map.first.y || this->_pos.x  < this->_bonus_type->_off_map.first.x)
		return Action_Update::TO_DELETE;
	if (this->_clock.getElapsedTime().asSeconds()>1)
	{
		if (this->idsprite == 0)
			this->idsprite = 1;
		else
			this->idsprite = 0;
		_clock.restart();
	}
	return Action_Update::NOTHING;
}