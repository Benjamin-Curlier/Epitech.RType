#include "Foe.h"
#include "ShipPlayer.h"

Foe::Foe(sf::Vector2f pos, Main_Object_type *t, std::vector<Missile_Type *> m, Explosion_Type *e, int hp, int point) : IMainObject(pos, t, m, e)
{
	this->_hp = hp;
	this->_point = point;
}


Foe::~Foe()
{
}

void Foe::test_collision(std::vector<ShipPlayer *> &o)
{
	std::vector<Missile *> missiles;

	if (this->_is_alive == false)
		return;

	for (int i = 0; i < o.size(); i++)
	{
		missiles = o[i]->getMissiles();
		for (int y = 0; y < missiles.size(); y++)
		{
			if (missiles[y]->getAlive() == true && this->collision(missiles[y]->getMissile_type()->_collision, missiles[y]->getPos()) == true)
			{
				missiles[y]->setAlive(false);
				if (this->testAlive() == false)
					o[i]->addScore(this->getPoint());
				return;
			}
		}
	}
}

Action_Update Foe::update(const float dt)
{
	if (this->_is_alive == true)
	{
		if (this->_lastfire.getElapsedTime().asSeconds() >= this->_firing_rate)
		{
			this->fire(this->_missile_type.at(0));
			this->_lastfire.restart();
		}
		if (this->_up == true)
		{
			if (this->_pos.y > 720 - this->_spec->_off_map.second.y)
				this->_up = false;
			else
				this->_pos.y += dt * 300;
		}
		else
		{
			if (this->_pos.y < this->_spec->_off_map.second.x)
				this->_up = true;
			else
				this->_pos.y -= dt * 300;
		}
	}
	else if (this->_missiles.empty() == true && this->_explosion == NULL)
	{
		return Action_Update::TO_DELETE;
	}
	for (int index = this->_missiles.size() - 1; index >= 0; index--)
	{
		auto &it = this->_missiles.at(index);
		if (it->update(dt) == Action_Update::TO_DELETE)
		{
			delete this->_missiles[index];
			this->_missiles.erase(std::find(this->_missiles.begin(), this->_missiles.end(), it));
		}
	}
	if (this->_explosion != NULL)
	{
		if (this->_explosion->update(dt) == Action_Update::TO_DELETE)
		{ 
			this->_explosion = NULL;
			delete this->_explosion;
		}
	}
	return Action_Update::NOTHING;
}
