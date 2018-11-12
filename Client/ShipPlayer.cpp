#include "ShipPlayer.h"
#include "Foe.h"
#include "Bonus.h"


ShipPlayer::ShipPlayer(sf::RenderWindow *w, sf::Vector2f pos, std::vector<S_sprite *> sprites, Main_Object_type *t, std::vector<Missile_Type *> m, Explosion_Type *e, std::vector<S_sound *> s) : IMainObject(w, pos, sprites, t, m, e)
{
	this->_sounds = s;
}


ShipPlayer::~ShipPlayer()
{
}

void ShipPlayer::test_collision(std::vector<Foe *> &o, std::vector<Bonus *> &b)
{
	std::vector<Missile *> missiles;

	if (this->_is_alive == false)
		return;

	for (int y = 0; y < b.size(); y++)
	{
		if (b[y]->getAlive() == true && this->collision(b[y]->getBonus_type()->_collision, b[y]->getPos()) == true)
		{
			if (b[y]->getBonus_type()->_bonus_action == Bonus_action::HP)
				this->_hp += 1;
			else
				this->_firing_rate += 0.2;
			b[y]->setAlive(false);
			this->_sounds[3]->_sound.play();
		}
	}

	for (int i = 0; i < o.size(); i++)
	{
		if (o[i]->getAlive() == true && this->collision(o[i]->getMain_type()->_collision, o[i]->getPos()) == true)
		{
			if (o[i]->testAlive() == false)
				this->addScore(o[i]->getPoint());
			if (this->testAlive() == false)
				this->_sounds[1]->_sound.play();
			else
				this->_sounds[2]->_sound.play();
			return;
		}
		missiles = o[i]->getMissiles();
		for (int y = 0; y < missiles.size(); y++)
		{
			if (missiles[y]->getAlive() == true && this->collision(missiles[y]->getMissile_type()->_collision, missiles[y]->getPos()) == true)
			{

				missiles[y]->setAlive(false);
				if (this->testAlive() == false)
					this->_sounds[1]->_sound.play();
				else
					this->_sounds[2]->_sound.play();
				return;
			}
		}
	}
}

void ShipPlayer::setAction(std::map<sf::Keyboard::Key, int> key)
{
	this->_v.x = -key[sf::Keyboard::Q] + key[sf::Keyboard::D];
	this->_v.y = -key[sf::Keyboard::Z] + key[sf::Keyboard::S];
	this->_is_firing = key[sf::Keyboard::Space];
}

void ShipPlayer::setAction(sf::Vector2f v, bool p)
{
	this->_v = v;
	this->_is_firing = p;
}

Action_Update ShipPlayer::update(const float dt)
{
	if (_is_alive == true)
	{
		this->_pos.x += dt * 1000 * this->_v.x;
		this->_pos.y += dt * 1000 * this->_v.y;
		if (this->_is_firing == 1 && this->_lastfire.getElapsedTime().asSeconds() >= this->_firing_rate)
		{
			this->fire(this->_missile_type.at(0));
			this->_lastfire.restart();
			this->_sounds[0]->_sound.play();
		}
		if (this->_pos.x > 1920 - this->_spec->_off_map.first.y)
			this->_pos.x = 1920 - this->_spec->_off_map.first.y;
		else if (this->_pos.x < this->_spec->_off_map.first.x)
			this->_pos.x = this->_spec->_off_map.first.x;
		if (this->_pos.y > 720 - this->_spec->_off_map.second.y)
			this->_pos.y = 720 - this->_spec->_off_map.second.y;
		else if (this->_pos.y < this->_spec->_off_map.second.x)
			this->_pos.y = this->_spec->_off_map.second.x;
		_sprites.at(0)->_sprite.setPosition(this->_pos);
		_w->draw(_sprites.at(0)->_sprite);
	}
	for (int index = this->_missiles.size() - 1; index >= 0; index--)
	{
		auto &it = this->_missiles.at(index);
		if (it->update(dt) == TO_DELETE)
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
