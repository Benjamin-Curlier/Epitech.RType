#include "pandaroux.h"
#include <iostream>

pandaroux::pandaroux(sf::RenderWindow *w, sf::Vector2f pos, std::vector<S_sprite *> sprites) : IObject(w, pos, sprites)
{
}

pandaroux::~pandaroux()
{
}

Action_Update pandaroux::update(const float dt)
{
	_sprites[0]->_sprite.setPosition(0, 0);
	this->_w->draw(_sprites[0]->_sprite);
	return Action_Update::NOTHING;
}