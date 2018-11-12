#pragma once
#include "IObject.h"

struct Missile_Type
{
	Missile_Type(std::pair<sf::Vector2f, sf::Vector2f> c, std::vector<S_sprite *> s, std::pair<sf::Vector2f, sf::Vector2f> o, bool a)
	{ 
		this->_collision = c;
		this->_sprites = s;
		this->_off_map = o;
		this->_left = a;
	}
	std::vector<S_sprite *> _sprites;
	std::pair<sf::Vector2f, sf::Vector2f> _collision;
	std::pair<sf::Vector2f, sf::Vector2f> _off_map;
	bool _left;
};

class Missile : public IObject
{
public:
	Missile(sf::RenderWindow*, sf::Vector2f, std::vector<S_sprite *> , Missile_Type *);
	~Missile();
	virtual Action_Update update(const float);
	Missile_Type *getMissile_type() const { return this->_missile_type; }
private:
	Missile_Type *_missile_type;
};

