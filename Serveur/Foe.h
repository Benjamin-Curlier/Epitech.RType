#pragma once
#include "IMainObject.h"
//#include "ShipPlayer.h"

class ShipPlayer;

class Foe : public IMainObject
{
public:
	Foe(sf::Vector2f, Main_Object_type*, std::vector<Missile_Type *>, Explosion_Type *, int, int);
	~Foe();
	virtual Action_Update update(const float);
	void test_collision(std::vector<ShipPlayer *> &o);
	int getPoint() const { return this->_point; }
private:
	bool _up = true;
	int _point;
};

