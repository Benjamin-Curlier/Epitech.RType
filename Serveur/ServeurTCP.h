#pragma once

#include <stdexcept>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include "Player.h"
#include "Header.h"
#include "Lobby.h"

#ifdef _WIN64

#include <WinSock2.h>

#elif defined __unix__

#include <algorithm>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

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

class ServeurTCP
{
public:
	static ServeurTCP *Instance;
private:
	SOCKET sock;
	std::vector<Player> players;
	std::map<std::string, Lobby> lobbies;
	std::map<std::string, void(ServeurTCP::*)(Player &, size_t)> Instructions;
private:
	void InitializeInstructions();
	void GetHighestSocket(int&, fd_set *);
	void CreateLobby(Player &, size_t);
	void JoinLobby(Player &, size_t);
	void ExitLobby(Player &, size_t);
	void ListLobbies(Player &, size_t);
	void ListPlayers(Player &, size_t);
	void IsReady(Player &actualPlayer, size_t argsSize);
	void StartGame(Player &actualPlayer, size_t argsSize);
public:
	void Init(int);
	void Start();
	void End();
	ServeurTCP();
	~ServeurTCP();
	void CheckIsNewPlayer(fd_set *);
	int WritePlayer(const SOCKET csock, Header *header);
	int WritePlayer(const SOCKET csock, void *ptr, size_t argsSize);
	void CheckIsPlayerTalking(fd_set *);
	int ReadPlayer(const SOCKET param1, Header *header);
	int ReadPlayer(const SOCKET param1, void *ptr, size_t argsSize);
};

