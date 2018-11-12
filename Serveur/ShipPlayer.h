#pragma once
#include "IMainObject.h"
#include "Lobby.h"

class Foe;
class Bonus;

class ShipPlayer : public IMainObject
{
public:
	ShipPlayer(sf::Vector2f, Main_Object_type*, std::vector<Missile_Type *>, Explosion_Type *);
	~ShipPlayer();
	virtual Action_Update update(const float);
	//void setAction(std::map<sf::Keyboard::Key, int>);
	void setAction(sf::Vector2f, bool);
	void test_collision(std::vector<Foe *> &o, std::vector<Bonus *> &b);
	void addScore(int point) { this->_score += point; };
	int getScore() const { return this->_score; }
private:
	sf::Vector2f _v;
	bool _is_firing;
	int _score;
};

