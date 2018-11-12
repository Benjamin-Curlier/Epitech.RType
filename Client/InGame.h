#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "IObject.h"
#include "ScrollingBackground.h"
#include "Missile.h"
#include "ShipPlayer.h"
#include "Foe.h"
#include "Explosion.h"
#include "Bonus.h"
#include "GameState.h"
#include <stack>
#include <map>
#include <thread>
#include <mutex>

enum unique_object
{
	SCROLLINGBACKGROUND = 0,
	X_WING,
	BLUE_BULLET,
	RED_BULLET,
	FIGHTER,
	EXPLOSION,
	BONUS,
};

class InGame : public GameState
{
public:
	InGame(Game *);
	~InGame() { t->join(); };
	virtual void Draw(const float dt);
	virtual void Update(const float dt);
	virtual void HandleInput();
	void WaitUpdate();

private:
	std::map<unique_object, std::vector<S_sound *> > _sounds;
	std::map<unique_object, std::vector<S_sprite*> > _sprites;
	std::map<unique_object, Main_Object_type*> _main_types;
	std::map<unique_object, Missile_Type*> _missile_type;
	std::map<unique_object, Explosion_Type*> _explosion_type;
	std::map<unique_object, Bonus_Type*> _bonus_types;

	std::thread *t;

	std::mutex _mutex;

	std::vector<ShipPlayer *> _shipplayers;
	std::vector<Foe *> _foes;
	std::vector<Bonus *> _bonus;


	ScrollingBackground *_scrollingbackground;
	std::map<sf::Keyboard::Key, int> _keys;

	sf::Event _event;
	sf::Clock _clock;
	sf::Music music;
};