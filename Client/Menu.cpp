#include "Menu.h"
#include <iostream>
#include <sstream>

Menu::Menu(Game *game)
{
	this->game = game;

	S_sprite *s = new S_sprite("Textures/menu.png", sf::Vector2f(1000, 625));
	std::vector<S_sprite *> vs;
	vs.push_back(s);
	this->_objects.push_back(new pandaroux(this->game->_rw, sf::Vector2f(0, 0), vs));

	S_sprite *s1 = new S_sprite("Textures/play.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs1;
	vs1.push_back(s1);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(750, 200), vs1));

	S_sprite *s2 = new S_sprite("Textures/options.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs2;
	vs2.push_back(s2);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(50, 290), vs2));

	S_sprite *s3 = new S_sprite("Textures/exit.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs3;
	vs3.push_back(s3);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(630, 358), vs3));

	S_sprite *s4 = new S_sprite("Textures/blanc.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs4;
	vs4.push_back(s4);
	blanc = new MenuBlanc(this->game->_rw, sf::Vector2f(1000, 625), vs4);

	font.loadFromFile("Textures/helvetica.ttf");

	serverInput.setFont(font);
	serverInput.setCharacterSize(25);
	serverInput.setColor(sf::Color::White);
	serverInput.setPosition(1000, 1000);

	portInput.setFont(font);
	portInput.setCharacterSize(25);
	portInput.setColor(sf::Color::White);
	portInput.setPosition(1000, 1000);

	help.setFont(font);
	help.setCharacterSize(30);
	help.setColor(sf::Color::White);
	help.setString("Adresse serveur\n\n\nPort\n\n\n\n  Join                   Cancel");
	help.setPosition(1000, 1000);

	menupos = 1;
	isPopUp = false;
	maxmenuitem = 3;
}

Menu::~Menu()
{
}

void Menu::MenuUp()
{
	if (menupos == maxmenuitem)
		menupos = 1;
	else
		menupos++;
}

void Menu::MenuDown()
{
	if (menupos == 1)
		menupos = maxmenuitem;
	else
		menupos--;
}

void Menu::MenuEnter()
{
	if (!isPopUp)
	{
		if (menupos == 1)
		{
			isPopUp = true;
		}
		else if (menupos == 2)
			play();
		else if (menupos == 3)
			this->game->_rw->close();
	}
	else
	{
		if (menupos == 3)
		{
			std::cout << "LobbyName : " << serverText << " / " << "Password : " << portText << std::endl;

			std::istringstream buffer(portText);
			unsigned short port;
			buffer >> port;
			sf::Socket::Status status = this->game->tcp.connect(serverText, port);
			if (status != sf::Socket::Done)
			{
				std::cout << status << std::endl;
				return;
			}
			this->game->serverAdress = serverText;
			menulobby();
		}
		else if (menupos == 4)
		{
			isPopUp = false;
			maxmenuitem = 3;
			menupos = 1;
			help.setPosition(sf::Vector2f(1000, 1000));
			serverInput.setPosition(sf::Vector2f(1000, 1000));
			portInput.setPosition(sf::Vector2f(1000, 1000));
			serverText = "";
			portText = "";
		}
	}
}

void Menu::Draw(const float dt)
{
	this->game->_rw->draw(help);
	this->game->_rw->draw(serverInput);
	this->game->_rw->draw(portInput);
}

void Menu::Update(const float dt)
{
	if (!isPopUp)
	{
		if (menupos == 1)
			blanc->setPos(sf::Vector2f(750, 200));
		else if (menupos == 2)
			blanc->setPos(sf::Vector2f(50, 290));
		else if (menupos == 3)
			blanc->setPos(sf::Vector2f(630, 358));
		for (auto &it : this->_objects)
			it->update(dt);
	}
	else
	{
		if (menupos == 1)
			blanc->setPos(sf::Vector2f(350, 150));
		else if (menupos == 2)
			blanc->setPos(sf::Vector2f(350, 250));
		else if (menupos == 3)
			blanc->setPos(sf::Vector2f(350, 350));
		else if (menupos == 4)
			blanc->setPos(sf::Vector2f(550, 350));
		this->_objects[0]->update(dt);
	}

	this->blanc->update(dt);
}

void Menu::HandleInput()
{
	while (this->game->_rw->pollEvent(_event))
	{
		if (this->_event.type == sf::Event::Closed)
			this->game->_rw->close();
		else if (this->_event.type == sf::Event::KeyPressed)
		{
			switch (this->_event.key.code)
			{
			case sf::Keyboard::Space:
				this->_keys[sf::Keyboard::Space] = 1;
				break;
			case sf::Keyboard::Up:
				this->_keys[sf::Keyboard::Up] = 1;
				break;
			case sf::Keyboard::Down:
				this->_keys[sf::Keyboard::Down] = 1;
				break;
			case sf::Keyboard::Return:
				this->_keys[sf::Keyboard::Return] = 1;
				break;
			}
		}
		if (isPopUp)
			ManagePopup();
	}
	if (this->_keys[sf::Keyboard::Up] == 1)
	{
		this->_keys[sf::Keyboard::Up] = 0;
		MenuDown();
	}
	if (this->_keys[sf::Keyboard::Down] == 1)
	{
		this->_keys[sf::Keyboard::Down] = 0;
		MenuUp();
	}
	if (this->_keys[sf::Keyboard::Return] == 1)
	{
		this->_keys[sf::Keyboard::Return] = 0;
		MenuEnter();
	}
}

void Menu::ManagePopup()
{
	maxmenuitem = 4;
	help.setPosition(360, 100);
	serverInput.setPosition(355, 150);
	portInput.setPosition(355, 250);

	if (this->_event.type == sf::Event::TextEntered)
	{
		if (menupos == 1)
		{
			if (this->_event.key.code == 8 && serverText.size() != 0) {
				serverText.pop_back();
			}
			else if (isprint(this->_event.text.unicode)) {
				serverText.push_back((char)this->_event.text.unicode);
			}
			serverInput.setString(serverText);
		}
		else if (menupos == 2)
		{
			if (this->_event.key.code == 8 && portText.size() != 0) {
				portText.pop_back();
			}
			else if (isprint(this->_event.text.unicode)) {
				portText.push_back((char)this->_event.text.unicode);
			}
			portInput.setString(portText);
		}
	}
}

void Menu::menulobby()
{
	this->game->PushState(new MenuLobby(game));
	//MenuLobby *m = new MenuLobby();
	//m->run();
}

void Menu::play()
{
	//this->_rw->close();
	//Game *g = new Game();
	//g->run();
}