#pragma once

#include <stdexcept>
#include <vector>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Header.h"

#include "IObject.h"
#include "Missile.h"
#include "ShipPlayer.h"
#include "Foe.h"
#include "Explosion.h"
#include "Bonus.h"
#include <stack>
#include <map>
#include <thread>

#ifdef _WIN64

#include <WinSock2.h>

#elif defined __unix__

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h> 

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define WSAGetLastError() errno

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

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

class Lobby
{
private:
	std::vector<Player> players;
	SOCKET sock;
	SOCKADDR_IN ssin;
	std::string PasswdHash;
public:
	const std::string GetPassword();
	std::thread *GameThread;
	void StartGame();
	std::vector<Player> &GetPlayers();
	Lobby(const std::string);
	~Lobby();
	Lobby() {};
	int port;
private:
	void SendClient(Player player, void *ptr, size_t argsSize);
	void ReadClient();
public:
	void Start();
	void Init();
	void End();
	bool isClientExist(SOCKADDR_IN* sin);

private:
	std::map<unique_object, Main_Object_type*> _main_types;
	std::map<unique_object, Missile_Type*> _missile_type;
	std::map<unique_object, Explosion_Type*> _explosion_type;
	std::map<unique_object, Bonus_Type*> _bonus_types;
	
	std::vector<ShipPlayer *> _shipplayers;
	std::vector<Foe *> _foes;
	std::vector<Bonus *> _bonus;
	
	sf::Clock _clock;
	sf::Clock myClock;

	void popStuff();
	std::pair<int, int> getRandPos(int xMin, int xMax, int yMin, int yMax);
	void Update(const float dt);
	void InitGame();
	void SendAllTheClient();
	int nbPlayer;
};

