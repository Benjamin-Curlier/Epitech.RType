#include "MenuPause.h"
#include "Player.h"
#include "Header.h"
#include "InGame.h"
#include <sstream>
#include <iostream>

MenuPause::MenuPause(Game *game)
{
	this->game = game;

	texts.push_back(&text);
	texts.push_back(&text1);
	texts.push_back(&text2);
	texts.push_back(&text3);

	S_sprite *s = new S_sprite("Textures/menulobby.png", sf::Vector2f(1000, 625));
	std::vector<S_sprite *> vs;
	vs.push_back(s);
	this->_objects.push_back(new pandaroux(this->game->_rw, sf::Vector2f(0, 0), vs));

	S_sprite *s1 = new S_sprite("Textures/ready.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs1;
	vs1.push_back(s1);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(400, 150), vs1));

	S_sprite *s2 = new S_sprite("Textures/ready.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs2;
	vs2.push_back(s2);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(400, 210), vs2));

	S_sprite *s3 = new S_sprite("Textures/ready.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs3;
	vs3.push_back(s3);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(400, 270), vs3));

	S_sprite *s4 = new S_sprite("Textures/ready.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs4;
	vs4.push_back(s4);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(400, 330), vs4));

	S_sprite *s5 = new S_sprite("Textures/square.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs5;
	vs5.push_back(s5);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(620, 150), vs5));

	S_sprite *s6 = new S_sprite("Textures/square.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs6;
	vs6.push_back(s6);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(620, 210), vs6));

	S_sprite *s7 = new S_sprite("Textures/square.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs7;
	vs7.push_back(s7);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(620, 270), vs7));

	S_sprite *s8 = new S_sprite("Textures/square.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs8;
	vs8.push_back(s8);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(620, 330), vs8));

	S_sprite *s9 = new S_sprite("Textures/check.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs9;
	vs9.push_back(s9);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(1000, 250), vs9));

	S_sprite *s10 = new S_sprite("Textures/playpause.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs10;
	vs10.push_back(s10);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(100, 420), vs10));

	S_sprite *s11 = new S_sprite("Textures/cancel.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs11;
	vs11.push_back(s11);
	this->_objects.push_back(new MenuButton(this->game->_rw, sf::Vector2f(400, 420), vs11));

	S_sprite *s12 = new S_sprite("Textures/blanc.png", sf::Vector2f(210, 55));
	std::vector<S_sprite *> vs12;
	vs12.push_back(s12);
	blanc = new MenuBlanc(this->game->_rw, sf::Vector2f(100, 420), vs12);

	font.loadFromFile("Textures/helvetica.ttf");
	text.setFont(font);
	text.setCharacterSize(50);
	text.setColor(sf::Color::White);
	text.setPosition(633, 147);

	text1.setFont(font);
	text1.setCharacterSize(50);
	text1.setColor(sf::Color::White);
	text1.setPosition(633, 207);

	text2.setFont(font);
	text2.setCharacterSize(50);
	text2.setColor(sf::Color::White);
	text2.setPosition(633, 267);

	text3.setFont(font);
	text3.setCharacterSize(50);
	text3.setColor(sf::Color::White);
	text3.setPosition(633, 327);

	help.setFont(font);
	help.setCharacterSize(30);
	help.setColor(sf::Color::White);
	help.setString("Player                                (Press space to be ready)");
	help.setPosition(50, 100);

	this->_keys[sf::Keyboard::Space] = 0;
	this->_keys[sf::Keyboard::Right] = 0;
	this->_keys[sf::Keyboard::Left] = 0;
	this->_keys[sf::Keyboard::Return] = 0;

	check = false;
	menupos = 1;
	maxiteminmenu = 2;
}

MenuPause::~MenuPause()
{
}

void MenuPause::MenuUp()
{
	if (menupos == maxiteminmenu)
		menupos = 1;
	else
		menupos++;
}

void MenuPause::MenuDown()
{
	if (menupos == 1)
		menupos = maxiteminmenu;
	else
		menupos--;
}

void MenuPause::MenuSpace()
{
	Header *header = new Header();

	header->argsSize = sizeof(LobbyInfo);
	header->command = "IS_READY";
	this->game->tcp.send(header, sizeof(Header));

	Header *head = new Header();
	std::size_t received;
	this->game->tcp.receive(head, sizeof(Header), received);
	std::cout << head->command << std::endl;
}

void MenuPause::Draw(const float dt)
{
	this->game->_rw->draw(help);
	this->game->_rw->draw(text);
}

void MenuPause::Update(const float dt)
{
	UpdatePlayers();

	if (menupos == 1)
		blanc->setPos(sf::Vector2f(100, 420));
	else if (menupos == 2)
		blanc->setPos(sf::Vector2f(400, 420));
	for (auto &it : this->_objects)
		it->update(dt);
	blanc->update(dt);
}

void MenuPause::HandleInput()
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
			case sf::Keyboard::Right:
				this->_keys[sf::Keyboard::Right] = 1;
				break;
			case sf::Keyboard::Left:
				this->_keys[sf::Keyboard::Left] = 1;
				break;
			case sf::Keyboard::Return:
				this->_keys[sf::Keyboard::Return] = 1;
				break;
			}
		}
	}
	if (this->_keys[sf::Keyboard::Right] == 1)
	{
		this->_keys[sf::Keyboard::Right] = 0;
		MenuDown();
	}
	if (this->_keys[sf::Keyboard::Left] == 1)
	{
		this->_keys[sf::Keyboard::Left] = 0;
		MenuUp();
	}
	if (this->_keys[sf::Keyboard::Space] == 1)
	{
		this->_keys[sf::Keyboard::Space] = 0;
		MenuSpace();
	}
	if (this->_keys[sf::Keyboard::Return] == 1)
	{
		this->_keys[sf::Keyboard::Return] = 0;
		MenuEnter();
	}
}

void MenuPause::UpdatePlayers()
{
	Header *header = new Header();
	std::size_t received;

	header->command = "LIST_PLAYERS";
	header->argsSize = -1;

	this->game->tcp.send(header, sizeof(Header));

	this->game->tcp.receive(header, sizeof(Header), received);

	int i;
	for (i = 0; i < header->argsSize; i++)
	{
		std::stringstream ss;
		std::string tmp;
		Player player1;

		size_t size;
		this->game->tcp.receive(&size, sizeof(size_t), received);
		char *data = new char[size];
		this->game->tcp.receive(data, size, received);

		data[size] = 0;
		tmp.assign(data);
		ss << tmp;
		ss >> player1;
		this->players.push_back(player1);
	}
	std::cout << std::endl;

	this->game->idplayer = this->players.size();

	for (i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].IsReady())
			this->texts[i]->setString("X");
		else
			this->texts[i]->setString(" ");
	}

	for (i = 0; i < this->players.size(); i++)
		this->players.pop_back();
}

void MenuPause::MenuEnter()
{
	if (menupos == 1)
	{
		std::size_t received;
		Header *header = new Header();

		header->argsSize = sizeof(LobbyInfo);
		header->command = "START_GAME";
		this->game->tcp.send(header, sizeof(Header));

		int port;
		this->game->tcp.receive(&port, sizeof(int), received);

		this->game->port = port;
		this->game->udp.bind(sf::UdpSocket::AnyPort);

		this->game->PushState(new InGame(this->game));
	}
	else if (menupos == 2)
	{
		Header *header = new Header();

		header->argsSize = sizeof(LobbyInfo);
		header->command = "EXIT_LOBBY";
		this->game->tcp.send(header, sizeof(Header));

		Header *head = new Header();
		std::size_t received;
		this->game->tcp.receive(head, sizeof(Header), received);
		std::cout << head->command << std::endl;

		this->game->PopState();
	}
}