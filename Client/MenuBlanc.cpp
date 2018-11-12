#include "MenuBlanc.h"
#include <iostream>

MenuBlanc::MenuBlanc(sf::RenderWindow *w, sf::Vector2f pos, std::vector<S_sprite *> sprites) : IObject(w, pos, sprites)
{

}

MenuBlanc::~MenuBlanc()
{
}

Action_Update MenuBlanc::update(const float)
{
	_sprites[0]->_sprite.setPosition(this->_pos.x, this->_pos.y);
	this->_w->draw(_sprites[0]->_sprite);
	return Action_Update::NOTHING;
}
