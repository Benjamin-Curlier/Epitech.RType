#include "MenuLobby.h"
#include "InGame.h"
#include "Header.h"
#include "MenuPause.h"
#include <ctype.h>
#include <iostream>

MenuLobby::MenuLobby(Game *game)
{
	this->game = game;

	S_sprite *s = new S_sprite("Textures/menulobby.png", sf::Vector2f(1000, 625));
	std::vector<S_sprite *> vs;
	vs.push_back(s);
	this->_objects.push_back(new pandaroux(this->game->_rw, sf::Vector2f(0, 0), vs));

	S_sprite *s1 = new S_sprite("Textures/create.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs1;
	vs1.push_back(s1);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(100, 150), vs1));

	S_sprite *s2 = new S_sprite("Textures/join.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs2;
	vs2.push_back(s2);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(100, 250), vs2));

	S_sprite *s3 = new S_sprite("Textures/return.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs3;
	vs3.push_back(s3);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(100, 350), vs3));

	S_sprite *s4 = new S_sprite("Textures/blanc.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs4;
	vs4.push_back(s4);
	blanc = new MenuBlanc(this->game->_rw, sf::Vector2f(1000, 625), vs4);
	
	font.loadFromFile("Textures/helvetica.ttf");

	lobbyInput.setFont(font);
	lobbyInput.setCharacterSize(25);
	lobbyInput.setColor(sf::Color::White);
	lobbyInput.setPosition(1000, 1000);

	passwdInput.setFont(font);
	passwdInput.setCharacterSize(25);
	passwdInput.setColor(sf::Color::White);
	passwdInput.setPosition(1000, 1000);

	help.setFont(font);
	help.setCharacterSize(30);
	help.setColor(sf::Color::White);
	help.setString("Lobby\n\n\nPassword\n\n\n\n   Create                Cancel");
	help.setPosition(1000, 1000);

	this->_keys[sf::Keyboard::Space] = 0;
	this->_keys[sf::Keyboard::Up] = 0;
	this->_keys[sf::Keyboard::Down] = 0;
	this->_keys[sf::Keyboard::Return] = 0;

	inpopup = false;
	isCreating = false;
	menupos = 1;
	maxiteminmenu = 3;
}

MenuLobby::~MenuLobby()
{
}

void MenuLobby::managepopup()
{
	inpopup = true;

	if (isCreating)
		help.setString("Lobby\n\n\nPassword\n\n\n\n   Create                Cancel");
	else
		help.setString("Lobby\n\n\nPassword\n\n\n\n   Join                   Cancel");

	maxiteminmenu = 4;
	help.setPosition(360, 100);
	lobbyInput.setPosition(355, 150);
	passwdInput.setPosition(355, 250);

	if (this->_event.type == sf::Event::TextEntered)
	{
		if (menupos == 1)
		{
			if (this->_event.key.code == 8 && lobbyText.size() != 0) {
				lobbyText.pop_back();
			}
			else if (isprint(this->_event.text.unicode)) {
				lobbyText.push_back((char)this->_event.text.unicode);
			}
			lobbyInput.setString(lobbyText);
		}
		else if (menupos == 2)
		{
			if (this->_event.key.code == 8 && passwdText.size() != 0) {
				passwdText.pop_back();
			}
			else if (isprint(this->_event.text.unicode)) {
				passwdText.push_back((char)this->_event.text.unicode);
			}
			passwdInput.setString(passwdText);
		}
	}
}

void MenuLobby::MenuUp()
{
	if (menupos == maxiteminmenu)
		menupos = 1;
	else
		menupos++;
}

void MenuLobby::MenuDown()
{
	if (menupos == 1)
		menupos = maxiteminmenu;
	else
		menupos--;
}

void MenuLobby::MenuEnter()
{
	if (!inpopup)
	{
		if (menupos == 1)
		{
			inpopup = true;
			isCreating = true;
		}
		else if (menupos == 2)
		{
			inpopup = true;
			isCreating = false;
			menupos = 1;
		}
		else if (menupos == 3)
			this->game->PopState();
	}
	else
	{
		if (menupos == 3 && isCreating)
		{
			CreateLobby();
		}
		else if (menupos == 3 && !isCreating)
		{
			JoinLobby();
		}
		else if (menupos == 4)
		{
			inpopup = false;
			maxiteminmenu = 3;
			menupos = 1;
			blanc->setPos(sf::Vector2f(100, 150));
			help.setPosition(sf::Vector2f(1000, 1000));
			lobbyInput.setPosition(sf::Vector2f(1000, 1000));
			passwdInput.setPosition(sf::Vector2f(1000, 1000));
			lobbyText = "";
			passwdText = "";
		}
	}
}

void MenuLobby::Draw(const float dt)
{
	this->game->_rw->draw(help);
	this->game->_rw->draw(lobbyInput);
	this->game->_rw->draw(passwdInput);
}

void MenuLobby::Update(const float dt)
{
	if (!inpopup)
	{
		if (menupos == 1)
			blanc->setPos(sf::Vector2f(100, 150));
		else if (menupos == 2)
			blanc->setPos(sf::Vector2f(100, 250));
		else if (menupos == 3)
			blanc->setPos(sf::Vector2f(100, 350));
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
	}
	for (auto &it : this->_objects)
		it->update(dt);
	blanc->update(dt);
}

void MenuLobby::HandleInput()
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
		if (inpopup)
			managepopup();
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

void MenuLobby::CreateLobby()
{
	Header *header = new Header();

	header->argsSize = sizeof(LobbyInfo);
	header->command = "CREATE_LOBBY";
	this->game->tcp.send(header, sizeof(Header));

	LobbyInfo *lobbyinfo = new LobbyInfo();

	lobbyinfo->LobbyName = lobbyText;
	lobbyinfo->PasswdHash = passwdText;

	this->game->tcp.send(lobbyinfo, sizeof(LobbyInfo));

	Header *head = new Header();
	std::size_t received;

	this->game->tcp.receive(head, sizeof(Header), received);

	if (head->command == "OK")
		play();
}

void MenuLobby::JoinLobby()
{
	Header *header = new Header();

	header->argsSize = sizeof(LobbyInfo);
	header->command = "JOIN_LOBBY";
	this->game->tcp.send(header, sizeof(Header));

	LobbyInfo *lobbyinfo = new LobbyInfo();

	lobbyinfo->LobbyName = lobbyText;
	lobbyinfo->PasswdHash = passwdText;

	this->game->tcp.send(lobbyinfo, sizeof(LobbyInfo));

	Header *head = new Header();
	std::size_t received;
	this->game->tcp.receive(head, sizeof(Header), received);

	if (head->command == "OK")
		play();
}

void MenuLobby::play()
{
	this->game->PushState(new MenuPause(this->game));
}