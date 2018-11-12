#include "AlmostPopUp.h"
#include <iostream>

AlmostPopUp::AlmostPopUp(sf::RenderWindow *w, sf::Vector2f pos, std::vector<S_sprite *> sprites) : IObject(w, pos, sprites)
{
}

AlmostPopUp::~AlmostPopUp()
{
}

Action_Update AlmostPopUp::update(const float)
{
	_sprites[0]->_sprite.setPosition(this->_pos.x, this->_pos.y);
	this->_w->draw(_sprites[0]->_sprite);
	return Action_Update::NOTHING;
}
