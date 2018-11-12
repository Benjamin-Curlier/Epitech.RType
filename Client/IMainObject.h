#pragma once

#include "IObject.h"
#include "Missile.h"
#include "Explosion.h"

struct Main_Object_type
{
	Main_Object_type(std::pair<sf::Vector2f, sf::Vector2f> c, std::vector<sf::Vector2f> &f, std::pair<sf::Vector2f, sf::Vector2f> o, sf::Vector2f a)
	{
		this->_fire_pos = f;
		this->_collision = c;
		this->_off_map = o;
		this->_center = a;
	}
	sf::Vector2f _center;
	std::vector<sf::Vector2f> _fire_pos;
	std::pair<sf::Vector2f, sf::Vector2f> _collision;
	std::pair<sf::Vector2f, sf::Vector2f> _off_map;
};

class IMainObject : public IObject
{
public:
	IMainObject(sf::RenderWindow *w, sf::Vector2f pos, std::vector<S_sprite *> sprites, Main_Object_type *t, std::vector<Missile_Type *> m, Explosion_Type *e) : IObject(w, pos, sprites)
	{
		this->_spec = t;
		this->_missile_type = m;
		this->_lastfire.restart();
		this->_is_alive = true;
		this->_hp = 1;
		this->_firing_rate = 0.5;
		this->_explosion_type = e;
	}
	bool collision(std::pair<sf::Vector2f, sf::Vector2f> o, sf::Vector2f pos)
	{
		if (this->_spec->_collision.first.x + this->_pos.x < o.second.x + pos.x && this->_spec->_collision.second.x + this->_pos.x > o.first.x + pos.x && this->_spec->_collision.first.y + this->_pos.y < o.second.y + pos.y && this->_spec->_collision.second.y + this->_pos.y > o.first.y + pos.y)
			return true;
		return false;
	}
	bool testAlive() 
	{ 
		this->_hp -= 1; 
		if (this->_hp == 0)
		{
			this->explode();
			this->setAlive(false);
			return false;
		}
		return this->_is_alive;
	}
	Main_Object_type *getMain_type() const { return _spec; };
	void fire(Missile_Type *m) 
	{
		for (int i = 0; i < this->_spec->_fire_pos.size(); i++)
			this->_missiles.push_back(new Missile(this->_w, this->_pos + this->_spec->_fire_pos[i], m->_sprites, m));
	}

	void setHp(int hp)
	{
		this->_hp = hp;
	}

	void explode() { this->_explosion = new Explosion(this->_w, this->_pos + this->_spec->_center  - this->_explosion_type->_center, this->_explosion_type->_sprites, this->_explosion_type); }
	std::vector<Missile *> &getMissiles() { return _missiles; }
	virtual Action_Update update(const float) = 0;
protected:
	Explosion_Type *_explosion_type;
	Explosion *_explosion;
	Main_Object_type *_spec;
	std::vector<Missile_Type *> _missile_type;
	std::vector<Missile *> _missiles;
	int _hp;
	float _firing_rate;
	sf::Clock _lastfire;
};

