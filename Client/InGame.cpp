#include "InGame.h"
#include "Header.h"
#include "Foe.h"
#include <iostream>

InGame::InGame(Game *game)
{
	this->game = game;

	this->_keys[sf::Keyboard::Z] = 0;
	this->_keys[sf::Keyboard::S] = 0;
	this->_keys[sf::Keyboard::Q] = 0;
	this->_keys[sf::Keyboard::D] = 0;
	this->_keys[sf::Keyboard::Space] = 0;

	S_sprite *s = new S_sprite("Textures/PrimaryBackground.png", sf::Vector2f(1920, 720));
	this->_sprites[unique_object::SCROLLINGBACKGROUND].push_back(s);

	S_sprite *s1 = new S_sprite("Textures/explosion.png", sf::Vector2f(150, 150));
	this->_sprites[unique_object::EXPLOSION].push_back(s1);

	S_sprite *s2 = new S_sprite("Textures/bullet2.png", sf::Vector2f(40, 12));
	this->_sprites[unique_object::RED_BULLET].push_back(s2);

	S_sprite *s3 = new S_sprite("Textures/enemi1.png", sf::Vector2f(60, 54));
	this->_sprites[unique_object::FIGHTER].push_back(s3);

	S_sprite *s4 = new S_sprite("Textures/spaceship1.png", sf::Vector2f(62, 80));
	this->_sprites[unique_object::X_WING].push_back(s4);

	S_sprite *s5 = new S_sprite("Textures/bullet1.png", sf::Vector2f(40, 12));
	this->_sprites[unique_object::BLUE_BULLET].push_back(s5);

	S_sprite *s6 = new S_sprite("Textures/heal.png", sf::Vector2f(40, 40));
	this->_sprites[unique_object::BONUS].push_back(s6);

	S_sprite *s7 = new S_sprite("Textures/heal2.png", sf::Vector2f(40, 40));
	this->_sprites[unique_object::BONUS].push_back(s7);

	S_sound *d = new S_sound("Music/PewPew.wav"); // tire
	this->_sounds[unique_object::X_WING].push_back(d);

	S_sound *d1 = new S_sound("Music/explosion.wav");
	this->_sounds[unique_object::X_WING].push_back(d1); // explosion

	S_sound *d2 = new S_sound("Music/explosion.wav");
	this->_sounds[unique_object::X_WING].push_back(d2); // impact

	S_sound *d3 = new S_sound("Music/explosion.wav");
	this->_sounds[unique_object::X_WING].push_back(d3); // bonus


	std::vector<sf::Vector2f> p;
	p.push_back(sf::Vector2f(46, 13));
	p.push_back(sf::Vector2f(46, 68));
	Main_Object_type *m = new Main_Object_type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(2, 32), sf::Vector2f(60, 49)), p, std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(2, 60), sf::Vector2f(6, 74)), sf::Vector2f(26, 40));
	this->_main_types[unique_object::X_WING] = m;

	std::vector<sf::Vector2f> p1;
	p1.push_back(sf::Vector2f(10, 27));
	Main_Object_type *t1 = new Main_Object_type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(11, 9), sf::Vector2f(48, 48)), p1, std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-49, 9), sf::Vector2f(-48, 9)), sf::Vector2f(29, 27));
	this->_main_types[unique_object::FIGHTER] = t1;

	Missile_Type *t2 = new Missile_Type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(3, 4), sf::Vector2f(36, 8)), this->_sprites[unique_object::RED_BULLET], std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-40, 2), sf::Vector2f(-12, 2)), true);
	this->_missile_type[unique_object::RED_BULLET] = t2;

	Missile_Type *t3 = new Missile_Type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(3, 4), sf::Vector2f(36, 8)), this->_sprites[unique_object::BLUE_BULLET], std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-40, 2), sf::Vector2f(-12, 2)), false);
	this->_missile_type[unique_object::BLUE_BULLET] = t3;

	Explosion_Type *t4 = new Explosion_Type(this->_sprites[unique_object::EXPLOSION], sf::Vector2f(73, 73));
	this->_explosion_type[unique_object::EXPLOSION] = t4;

	Bonus_Type *t5 = new Bonus_Type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(1, 0), sf::Vector2f(38, 38)), this->_sprites[unique_object::BONUS], std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-38, 0), sf::Vector2f(-38, 1)), Bonus_action::HP);
	this->_bonus_types[unique_object::BONUS] = t5;

	this->_scrollingbackground = new ScrollingBackground(this->game->_rw, sf::Vector2f(0, 0), this->_sprites[unique_object::SCROLLINGBACKGROUND]);

	music.openFromFile("Music/Duel_Of_Fates_8-Bit.ogg");
	music.setLoop(true);
	music.play();

	CreateObject *co = new CreateObject();
	Input *entry = new Input(this->_keys, -1);
	std::size_t received;
	sf::IpAddress sender;
	unsigned short port;
	int nbPlayer;

	this->game->udp.send(entry, sizeof(Input), sf::IpAddress(this->game->serverAdress), this->game->port);

	this->game->udp.receive(&nbPlayer, sizeof(int), received, sender, port);

	std::vector<Missile_Type *> vm[4];

	int i;
	for (i = 0; i < nbPlayer; i++)
	{
		this->game->udp.receive(co, sizeof(CreateObject), received, sender, port);

		vm[i].push_back(this->_missile_type[(unique_object)co->typebullet]);
		this->_shipplayers.push_back(new ShipPlayer(this->game->_rw, sf::Vector2f(co->posX, co->posY), this->_sprites[(unique_object)co->type], this->_main_types[(unique_object)co->type], vm[i], this->_explosion_type[(unique_object)co->typeexplosion], this->_sounds[(unique_object)co->type]));
	}

	t = new std::thread(&InGame::WaitUpdate, this);
}

void InGame::Draw(const float dt)
{

}

void InGame::Update(const float dt)
{
	this->_scrollingbackground->update(dt);

	std::cout << std::boolalpha << this->_shipplayers[0]->getAlive() << std::endl;
	std::cout << std::boolalpha << this->_shipplayers[0]->getPos().x << " " << this->_shipplayers[0]->getPos().y << std::endl;

	this->_mutex.lock();
	for (int index = this->_shipplayers.size() - 1; index >= 0; index--) {
		this->_shipplayers[index]->test_collision(_foes, _bonus);
		this->_shipplayers[index]->update(dt);
	}


	for (int index = this->_foes.size() - 1; index >= 0; index--) {
		if (this->_foes[index] != NULL)
			this->_foes[index]->test_collision(_shipplayers);
		if (this->_foes[index] != NULL && this->_foes[index]->update(dt) == Action_Update::TO_DELETE) {
			auto &it = _foes.at(index);
			delete this->_foes[index];
			_foes.erase(std::find(this->_foes.begin(), this->_foes.end(), it));
		}
	}

	for (int index = this->_bonus.size() - 1; index >= 0; index--) {
		if (this->_bonus[index] != NULL && this->_bonus[index]->update(dt) == Action_Update::TO_DELETE) {
			auto &it = _bonus.at(index);
			delete this->_bonus[index];
			_bonus.erase(std::find(_bonus.begin(), _bonus.end(), it));
		}
	}
	this->_mutex.unlock();
}

void InGame::HandleInput()
{
	bool thereIsEvent = false;

	while (this->game->_rw->pollEvent(_event))
	{
		if (this->_event.type == sf::Event::Closed)
			this->game->_rw->close();
		else if (this->_event.type == sf::Event::KeyPressed)
		{
			switch (this->_event.key.code)
			{
			case sf::Keyboard::S:
				this->_keys[sf::Keyboard::S] = 1;
				thereIsEvent = true;
				break;
			case sf::Keyboard::Z:
				this->_keys[sf::Keyboard::Z] = 1;
				thereIsEvent = true;
				break;
			case sf::Keyboard::D:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::D] = 1;
				break;
			case sf::Keyboard::Q:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::Q] = 1;
				break;
			case sf::Keyboard::Space:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::Space] = 1;
				break;
			default:
				break;
			}
		}
		else if (this->_event.type == sf::Event::KeyReleased)
		{
			switch (this->_event.key.code)
			{
			case sf::Keyboard::S:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::S] = 0;
				break;
			case sf::Keyboard::Z:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::Z] = 0;
				break;
			case sf::Keyboard::D:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::D] = 0;
				break;
			case sf::Keyboard::Q:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::Q] = 0;
				break;
			case sf::Keyboard::Space:
				thereIsEvent = true;
				this->_keys[sf::Keyboard::Space] = 0;
				break;
			default:
				break;
			}
		}
	}

	if (thereIsEvent)
	{
		Input *entry = new Input(this->_keys, this->game->idplayer);

		this->game->udp.send(entry, sizeof(Input), sf::IpAddress(this->game->serverAdress), this->game->port);
	}

	if (this->_shipplayers[this->game->idplayer - 1] != NULL)
		this->_shipplayers[this->game->idplayer - 1]->setAction(this->_keys);
	//RECEIVE DATA FROM SERVER
}

void InGame::WaitUpdate()
{
	while (1)
	{
		CreateObject *co = new CreateObject();
		Input *entry = new Input(this->_keys, -1);
		std::size_t received;
		sf::IpAddress sender;
		unsigned short port;
		std::size_t vectorSize;

		this->game->udp.receive(&vectorSize, sizeof(size_t), received, sender, port);

		this->_mutex.lock();
		int i;
		for (i = 0; i < vectorSize; i++)
		{
			this->game->udp.receive(co, sizeof(CreateObject), received, sender, port);
			if (i < this->_shipplayers.size())
			{
				this->_shipplayers[i]->setPos(sf::Vector2f(co->posX, co->posY));
				this->_shipplayers[i]->setHp(co->hp);
				this->_shipplayers[i]->setAlive(co->alive);
				this->_shipplayers[i]->setScore(co->score);
			}
		}

		this->game->udp.receive(&vectorSize, sizeof(size_t), received, sender, port);

		for (i = 0; i < vectorSize; i++)
		{
			this->game->udp.receive(co, sizeof(CreateObject), received, sender, port);
			if (i < this->_foes.size())
			{
				this->_foes[i]->setPos(sf::Vector2f(co->posX, co->posY));
				this->_foes[i]->setHp(co->hp);
				this->_foes[i]->setAlive(co->alive);
			}
			else
			{
				std::vector<Missile_Type *> vm;

				vm.push_back(this->_missile_type[(unique_object)co->typebullet]);
				this->_foes.push_back(new Foe(this->game->_rw, sf::Vector2f(co->posX, co->posY), this->_sprites[(unique_object)co->type], this->_main_types[(unique_object)co->type], vm, this->_explosion_type[(unique_object)co->typeexplosion], 100, 1));
			}
		}

		this->game->udp.receive(&vectorSize, sizeof(size_t), received, sender, port);

		for (i = 0; i < vectorSize; i++)
		{
			this->game->udp.receive(co, sizeof(CreateObject), received, sender, port);
			if (i < this->_bonus.size())
			{
				this->_bonus[i]->setPos(sf::Vector2f(co->posX, co->posY));
				this->_bonus[i]->setAlive(co->alive);
			}
			else
			{
				this->_bonus.push_back(new Bonus(this->game->_rw, sf::Vector2f(co->posX, co->posY), this->_sprites[(unique_object)co->type], this->_bonus_types[(unique_object)co->type]));
			}
		}

		this->_mutex.unlock();
	}
}

