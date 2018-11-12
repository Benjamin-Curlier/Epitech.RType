#include "MenuButton.h"
#include <iostream>

MenuButton::MenuButton(sf::RenderWindow* w, sf::Vector2f pos, std::vector<S_sprite *> sprites) : IObject(w, pos, sprites)
{

}

MenuButton::~MenuButton()
{
}

Action_Update MenuButton::update(const float dt)
{
	_sprites[0]->_sprite.setPosition(this->_pos.x, this->_pos.y);
	this->_w->draw(_sprites[0]->_sprite);
	return Action_Update::NOTHING;
}
