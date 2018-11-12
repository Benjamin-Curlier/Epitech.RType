#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>

enum Bonus_action
{
	HP,
	FIRE_RATE
};

enum Action_Update
{
	NOTHING,
	TO_DELETE,
};

class IObject
{
public:
	IObject(sf::Vector2f pos)
	{
		this->_pos = pos;
	}
	sf::Vector2f getPos() const { return _pos; }
	void setPos(sf::Vector2f pos) { this->_pos = pos; }
	void setAlive(bool a) { this->_is_alive = a; }
	virtual bool getAlive() const { return this->_is_alive; }
	virtual Action_Update update(const float) = 0;
protected:
	bool _is_alive = true;
	sf::Vector2f _pos;
};

