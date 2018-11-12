#pragma once
#include <SFML\Graphics.hpp>
#include "IObject.h"
#include "pandaroux.h"
#include "MenuButton.h"
#include "MenuBlanc.h"
#include "Game.h"
#include "MenuLobby.h"
#include "GameState.h"
#include <vector>
#include <map>


class Menu : public GameState
{
public:
	Menu(Game *);
	~Menu();
	void menulobby();
	void play();
	void MenuUp();
	void MenuDown();
	void MenuEnter();
	virtual void Draw(const float dt);
	virtual void Update(const float dt);
	virtual void HandleInput();
	void ManagePopup();
private:
	bool isPopUp;
	std::vector<IObject *> _objects;
	IObject *blanc;
	std::map<sf::Keyboard::Key, int> _keys;
	sf::Event _event;
	sf::Vector2i _v;
	sf::Time t_bullet;
	int menupos;
	int maxmenuitem;
	float x;
	sf::Font font;
	std::string serverText;
	sf::Text serverInput;
	sf::Text portInput;
	std::string portText;
	sf::Text help;
};

