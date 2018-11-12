#pragma once
#include "IObject.h"


struct Bonus_Type
{
	Bonus_Type(std::pair<sf::Vector2f, sf::Vector2f> c, std::pair<sf::Vector2f, sf::Vector2f> o, Bonus_action a)
	{
		this->_collision = c;
		this->_off_map = o;
		this->_bonus_action = a;
	}
	std::pair<sf::Vector2f, sf::Vector2f> _collision;
	std::pair<sf::Vector2f, sf::Vector2f> _off_map;
	Bonus_action _bonus_action;
};

class Bonus : public IObject
{
public:
	Bonus(sf::Vector2f, Bonus_Type *);
	~Bonus();
	virtual Action_Update update(const float);
	Bonus_Type *getBonus_type() const { return this->_bonus_type; }
private:
	Bonus_Type *_bonus_type;
	sf::Clock _clock;
	int idsprite = 0;
};

