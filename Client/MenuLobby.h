#pragma once
#include <SFML\Graphics.hpp>
#include "pandaroux.h"
#include "MenuButton.h"
#include "MenuBlanc.h"
#include "Game.h"
#include "AlmostPopUp.h"
#include "GameState.h"
#include <vector>
#include <map>


class MenuLobby : public GameState
{
public:
	MenuLobby(Game *);
	~MenuLobby();
	void play();
	void managepopup();
	void MenuDown();
	void MenuUp();
	void MenuEnter();
	virtual void Draw(const float dt);
	virtual void Update(const float dt);
	virtual void HandleInput();
	void CreateLobby();
	void JoinLobby();
private:
	std::vector<IObject *> _objects;
	std::map<sf::Keyboard::Key, int> _keys;
	sf::Event _event;
	sf::Vector2i _v;
	sf::Time t_bullet;
	sf::Text help;
	sf::Font font;

	std::string lobbyText;
	sf::Text lobbyInput;
	std::string passwdText;
	sf::Text passwdInput;

	IObject *pop;
	IObject *blanc;
	bool inpopup;
	bool isCreating;
	int menupos;
	int maxiteminmenu;
	std::string server;
	std::string passwd;
	float x;
};

