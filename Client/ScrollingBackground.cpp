#include "ScrollingBackground.h"

ScrollingBackground::ScrollingBackground(sf::RenderWindow *w, sf::Vector2f pos, std::vector<S_sprite *> sprites) : IObject(w, pos, sprites)
{
}


ScrollingBackground::~ScrollingBackground()
{
}

Action_Update ScrollingBackground::update(const float dt)
{
	this->_sprites.at(0)->_sprite.setPosition(this->_pos);
	this->_w->draw(this->_sprites.at(0)->_sprite);
	this->_sprites.at(0)->_sprite.setPosition(-(1920 - this->_pos.x), this->_pos.y);
	this->_w->draw(this->_sprites.at(0)->_sprite);
	this->_pos.x -= dt * 1000 * 0.5;
	if (this->_pos.x < 0)
		this->_pos.x = 1920;
	return Action_Update::NOTHING;
}

