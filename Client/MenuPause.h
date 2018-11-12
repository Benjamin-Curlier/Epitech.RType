#pragma once
#include <SFML\Graphics.hpp>
#include "pandaroux.h"
#include "MenuButton.h"
#include "MenuBlanc.h"
#include "Game.h"
#include "AlmostPopUp.h"
#include "GameState.h"
#include "Player.h"
#include <vector>
#include <map>

class MenuPause : public GameState
{
public:
	MenuPause(Game *);
	~MenuPause();
	void MenuEnter();
	void MenuUp();
	void MenuDown();
	void MenuSpace();
	virtual void Draw(const float dt);
	virtual void Update(const float dt);
	virtual void HandleInput();
	void UpdatePlayers();
private:
	std::vector<sf::Text*> texts;
	std::vector<Player> players;
	std::vector<IObject *> _objects;
	std::map<sf::Keyboard::Key, int> _keys;
	sf::Event _event;
	sf::Vector2i _v;
	sf::Time t_bullet;
	sf::Text text;
	sf::Text text1;
	sf::Text text2;
	sf::Text text3;
	sf::Text help;
	sf::Font font;
	std::string str;
	IObject *pop;
	IObject *blanc;
	bool inpopup;
	int menupos;
	int maxiteminmenu;
	std::string server;
	std::string passwd;
	float x;
	bool check;
};

