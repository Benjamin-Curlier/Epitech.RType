#pragma once
#include "IMainObject.h"

class Foe;
class Bonus;

class ShipPlayer : public IMainObject
{
public:
	ShipPlayer(sf::RenderWindow*, sf::Vector2f, std::vector<S_sprite *>, Main_Object_type*, std::vector<Missile_Type *>, Explosion_Type *, std::vector<S_sound *>);
	~ShipPlayer();
	virtual Action_Update update(const float);
	void setAction(std::map<sf::Keyboard::Key, int>);
	void setAction(sf::Vector2f, bool);
	void test_collision(std::vector<Foe *> &o, std::vector<Bonus *> &b);
	void addScore(int point) { this->_score += point; };
	int getScore() const { return this->_score; }
	void setScore(int point) { this->_score = point; }
private:
	std::vector<S_sound *> _sounds;
	sf::Vector2f _v;
	bool _is_firing;
	int _score;
};

