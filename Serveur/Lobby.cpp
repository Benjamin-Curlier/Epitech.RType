#include <thread>
#include "Lobby.h"

#include "ServeurTCP.h"

void Lobby::SendClient(Player player, void *ptr, size_t argsSize)
{
	if (sendto(sock, (char*)ptr, argsSize, 0, (SOCKADDR *)player.GetSin(), sizeof(SOCKADDR)) < 0)
		throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
}

void Lobby::ReadClient()
{
	SOCKADDR_IN sin = { 0 };
	int sinsize = sizeof(sin);
	Input *entry = new Input();

	if ((recvfrom(sock, (char*)entry, sizeof(Input), 0, (SOCKADDR *)&sin, &sinsize)) < 0)
		throw new std::runtime_error("Recvfrom failed with error code " + WSAGetLastError());

	if (!this->isClientExist(&sin))
		this->players.push_back(Player(sin));

	this->_shipplayers[entry->idplayer - 1]->setAction(sf::Vector2f(-(int)(entry->input[1]) + (int)entry->input[3], -(int)entry->input[0] + (int)entry->input[2]), entry->input[4]);

	Update(this->_clock.getElapsedTime().asSeconds());
	this->_clock.restart();
}

void Lobby::Start()
{
	int actual = 0;
	fd_set rdfs;

	this->nbPlayer = this->players.size();
	for (; actual < this->players.size(); actual++)
		this->players.pop_back();

	InitGame();

	while (1)
	{
		FD_ZERO(&rdfs);
		FD_SET(sock, &rdfs);

		timeval *timeout = new timeval();
		timeout->tv_usec = 1000;

		if (select((int)sock + 1, &rdfs, NULL, NULL, timeout) == -1)
			throw new std::runtime_error("Select failed with error code " + WSAGetLastError());

		if (FD_ISSET(sock, &rdfs))
			ReadClient();

		Update(this->_clock.getElapsedTime().asSeconds());
		this->_clock.restart();
		SendAllTheClient();
		//Put timeout on select and send serialize object
	}
}

void Lobby::SendAllTheClient()
{
	size_t size;

	size = this->_shipplayers.size();
	for (auto &player : players)
	{
		if (sendto(sock, (char*)&size, sizeof(size_t), 0, (SOCKADDR *)player.GetSin(), sizeof(SOCKADDR)) < 0)
			throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
	}

	for (auto ship : this->_shipplayers)
	{
		CreateObject *co = new CreateObject(ship->getPos().x, ship->getPos().y, unique_object::X_WING, unique_object::BLUE_BULLET, unique_object::EXPLOSION, ship->getScore(), ship->GetHp(), ship->getAlive());

		for (auto &player : players)
		{
			if (sendto(sock, (char*)co, sizeof(CreateObject), 0, (SOCKADDR *)player.GetSin(), sizeof(SOCKADDR)) < 0)
				throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
		}
	}

	size = this->_foes.size();
	for (auto &player : players)
	{
		if (sendto(sock, (char*)&size, sizeof(size_t), 0, (SOCKADDR *)player.GetSin(), sizeof(SOCKADDR)) < 0)
			throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
	}

	for (auto foe : this->_foes)
	{
		CreateObject *co = new CreateObject(foe->getPos().x, foe->getPos().y, unique_object::FIGHTER, unique_object::RED_BULLET, unique_object::EXPLOSION, 0, foe->GetHp(), foe->getAlive());

		for (auto &player : players)
		{
			if (sendto(sock, (char*)co, sizeof(CreateObject), 0, (SOCKADDR *)player.GetSin(), sizeof(SOCKADDR)) < 0)
				throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
		}
	}

	size = this->_bonus.size();
	for (auto &player : players)
	{
		if (sendto(sock, (char*)&size, sizeof(size_t), 0, (SOCKADDR *)player.GetSin(), sizeof(SOCKADDR)) < 0)
			throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
	}

	for (auto bonus : this->_bonus)
	{
		CreateObject *co = new CreateObject(bonus->getPos().x, bonus->getPos().y, unique_object::BONUS, 0, 0, 0, 0, bonus->getAlive());

		for (auto &player : players)
		{
			if (sendto(sock, (char*)co, sizeof(CreateObject), 0, (SOCKADDR *)player.GetSin(), sizeof(SOCKADDR)) < 0)
				throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
		}
	}
}


void Lobby::Init()
{
	int error;

#ifdef _WIN64
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
		throw new std::runtime_error("WSAStartup failed with error code : " + WSAGetLastError());
#endif
	this->ssin = { 0 };

	if ((this->sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		throw new std::runtime_error("Socket failed with error code " + WSAGetLastError());

	this->ssin.sin_addr.s_addr = htonl(INADDR_ANY);
	this->ssin.sin_family = AF_INET;

	for (port = 9001; port != 9015; port++)
	{
		this->ssin.sin_port = htons(port);
		if ((error = bind(sock, (SOCKADDR *)&ssin, sizeof ssin)) != SOCKET_ERROR)
			break;
		else if (error == SOCKET_ERROR && port == 9015)
			throw new std::runtime_error("Bind failed with error code " + WSAGetLastError());
	}
}

void Lobby::End()
{
#ifdef _WIN64
	WSACleanup();
#endif
}

Lobby::Lobby(const std::string passwd)
{
	this->PasswdHash = passwd;
}

Lobby::~Lobby()
{
	closesocket(sock);
}

bool Lobby::isClientExist(SOCKADDR_IN* sin)
{
	for (auto player : this->players)
	{
		if (player.GetSin() != NULL)
		{
			if (player.GetSin()->sin_addr.s_addr == sin->sin_addr.s_addr && player.GetSin()->sin_port == sin->sin_port)
				return true;
		}
	}
	return false;
}

const std::string Lobby::GetPassword()
{
	return this->PasswdHash;
}

void Lobby::StartGame()
{
	this->Init();

	Header *header = new Header();
	header->argsSize = sizeof(this->ssin);
	header->command = "GAME_STARTED";

	for (auto player : this->players)
		ServeurTCP::Instance->WritePlayer(player.GetFd(), &(this->port), sizeof(int));

	this->Start();
	this->End();
}

std::vector<Player> & Lobby::GetPlayers()
{
	return this->players;
}

void Lobby::popStuff()
{
	std::vector<Missile_Type *> vm2;
	vm2.push_back(this->_missile_type[unique_object::RED_BULLET]);

	if ((myClock.getElapsedTime().asMilliseconds() > 1000)) {
		myClock.restart();
		std::pair<int, int> pos = getRandPos(1920 / 2, 1920, 0, 720);

		if (_foes.size() < 15 && (rand() % 100) > 20) {
			this->_foes.push_back(new Foe(sf::Vector2f(pos.first, pos.second), this->_main_types[unique_object::FIGHTER], vm2, this->_explosion_type[unique_object::EXPLOSION], 1, 100));
		}
		else {
			this->_bonus.push_back(new Bonus(sf::Vector2f(pos.first, pos.second), this->_bonus_types[unique_object::BONUS]));
		}
	}
}

std::pair<int, int> Lobby::getRandPos(int xMin, int xMax, int yMin, int yMax) {
	int x = ((rand() % (xMax - xMin)) + xMin);
	int y = ((rand() % (yMax - yMin)) + yMin);

	std::pair<int, int> *whyarewesolate = new std::pair<int, int>(x, y);

	return *whyarewesolate;
}

void Lobby::Update(const float dt)
{
	popStuff();

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
}

void Lobby::InitGame()
{
	std::vector<sf::Vector2f> p;
	p.push_back(sf::Vector2f(46, 13));
	p.push_back(sf::Vector2f(46, 68));
	Main_Object_type *m = new Main_Object_type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(2, 32), sf::Vector2f(60, 49)), p, std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(2, 60), sf::Vector2f(6, 74)), sf::Vector2f(26, 40));
	this->_main_types[unique_object::X_WING] = m;

	std::vector<sf::Vector2f> p1;
	p1.push_back(sf::Vector2f(10, 27));
	Main_Object_type *t1 = new Main_Object_type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(11, 9), sf::Vector2f(48, 48)), p1, std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-49, 9), sf::Vector2f(-48, 9)), sf::Vector2f(29, 27));
	this->_main_types[unique_object::FIGHTER] = t1;

	Missile_Type *t2 = new Missile_Type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(3, 4), sf::Vector2f(36, 8)), std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-40, 2), sf::Vector2f(-12, 2)), true);
	this->_missile_type[unique_object::RED_BULLET] = t2;

	Missile_Type *t3 = new Missile_Type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(3, 4), sf::Vector2f(36, 8)), std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-40, 2), sf::Vector2f(-12, 2)), false);
	this->_missile_type[unique_object::BLUE_BULLET] = t3;

	Explosion_Type *t4 = new Explosion_Type(sf::Vector2f(73, 73));
	this->_explosion_type[unique_object::EXPLOSION] = t4;

	Bonus_Type *t5 = new Bonus_Type(std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(1, 0), sf::Vector2f(38, 38)), std::pair<sf::Vector2f, sf::Vector2f>(sf::Vector2f(-38, 0), sf::Vector2f(-38, 1)), Bonus_action::HP);
	this->_bonus_types[unique_object::BONUS] = t5;

	int i;

	SOCKADDR_IN sin = { 0 };
	int sinsize = sizeof(sin);
	Input *entry = new Input();

	for (i = 0; i < nbPlayer; i++)
	{
		if ((recvfrom(sock, (char*)entry, sizeof(Input), 0, (SOCKADDR *)&sin, &sinsize)) < 0)
			throw new std::runtime_error("Recvfrom failed with error code " + WSAGetLastError());

		if (!this->isClientExist(&sin))
			this->players.push_back(Player(sin));
	}

	for (auto &player : players)
	{
		if (sendto(sock, (char*)&nbPlayer, sizeof(int), 0, (SOCKADDR*)player.GetSin(), sinsize) < 0)
			throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
	}

	for (auto &player : players)
	{
		std::vector<Missile_Type *> vm;
		vm.push_back(this->_missile_type[unique_object::BLUE_BULLET]);

		this->_shipplayers.push_back(new ShipPlayer(sf::Vector2f(50, 100 * (i + 1)), this->_main_types[unique_object::X_WING], vm, t4));

		CreateObject *co = new CreateObject(50, 100 * (i + 1), unique_object::X_WING, unique_object::BLUE_BULLET, unique_object::EXPLOSION, 0, 0, true);

		if (sendto(sock, (char*)co, sizeof(CreateObject), 0, (SOCKADDR*)player.GetSin(), sinsize) < 0)
			throw new std::runtime_error("Sendto failed with error code " + WSAGetLastError());
	}
}

