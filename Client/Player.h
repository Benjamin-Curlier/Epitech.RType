#pragma once

#include <string>

#ifdef _WIN64

#include <WinSock2.h>

#elif defined __unix__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;

#else

#error not defined for this platform

#endif

class Player
{
private:
	SOCKET fd;
	SOCKADDR_IN *Sin;
	std::string lobbyName;
	bool ready;
public:
	Player() {};
	Player(SOCKET&);
	Player(const SOCKADDR_IN &);
	~Player();
public:
	void SetReady(bool);
	void SetLobbyName(std::string);
	void SetSin(const SOCKADDR_IN &);
	const std::string &GetLobbyName() const;
	const SOCKET GetFd() const;
	SOCKADDR_IN *GetSin() const;
	bool IsReady() const;
	friend std::istream& operator>> (std::istream& in, Player& object);
	friend std::ostream& operator<< (std::ostream& out, Player& object);
};

bool operator==(const Player& pl1, const Player& pl2);
bool operator==(const Player& pl1, SOCKET &csock);