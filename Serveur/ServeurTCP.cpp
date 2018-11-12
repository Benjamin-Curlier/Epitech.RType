#include <sstream>
#include "sha256.h"
#include "ServeurTCP.h"

struct LobbyInfo
{
	std::string LobbyName;
	std::string PasswdHash;
};

ServeurTCP *ServeurTCP::Instance;

void ServeurTCP::Init(int port)
{
#ifdef _WIN64
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
		throw new std::runtime_error("WSAStartup failed with error code : " + WSAGetLastError());
#endif
	SOCKADDR_IN sin = { 0 };

	if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		throw new std::runtime_error("Socket failed with error code " + WSAGetLastError());

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if (bind(sock, (SOCKADDR *)&sin, sizeof sin) == SOCKET_ERROR)
		throw new std::runtime_error("Bind failed with error code " + WSAGetLastError());
	if (listen(sock, 100) == SOCKET_ERROR)
		throw new std::runtime_error("Listen failed with error code " + WSAGetLastError());
}

void ServeurTCP::Start()
{
	fd_set rdfs;
	int max = (int)sock;

	this->InitializeInstructions();
	while (1)
	{
		FD_ZERO(&rdfs);
		FD_SET(sock, &rdfs);

		this->GetHighestSocket(max, &rdfs);
		if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
			throw new std::runtime_error("Select failed with error code " + WSAGetLastError());
		this->CheckIsNewPlayer(&rdfs);
		this->CheckIsPlayerTalking(&rdfs);
	}
}

void ServeurTCP::End()
{
#ifdef _WIN64
	WSACleanup();
#endif
}

ServeurTCP::ServeurTCP()
{
	ServeurTCP::Instance = this;
}


ServeurTCP::~ServeurTCP()
{
}

void ServeurTCP::CheckIsNewPlayer(fd_set *rdfs)
{
	if (FD_ISSET(sock, rdfs))
	{
		SOCKADDR_IN csin = { 0 };
		int sinsize = sizeof(csin);
		SOCKET csock;

		if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) == SOCKET_ERROR)
			throw std::runtime_error("Accept failed with error code " + WSAGetLastError());

		auto newPlayer = Player(csock);
		this->players.push_back(newPlayer);
	}
}

int ServeurTCP::WritePlayer(const SOCKET csock, Header *header)
{
	int n = 0;

	if ((n = send(csock, (char*)header, sizeof(Header), 0)) < 0)
	{
		perror("Send failed with error ");
		n = 0;
	}
	return n;
}

int ServeurTCP::WritePlayer(const SOCKET csock, void *ptr, size_t argsSize)
{
	int n = 0;

	if ((n = send(csock, (char*)ptr, argsSize, 0)) < 0)
	{
		perror("Send failed with error ");
		n = 0;
	}
	return n;
}

int ServeurTCP::ReadPlayer(const SOCKET param1, void *ptr, size_t argsSize)
{
	int n = 0;

	if ((n = recv(param1, (char*)ptr, (int)argsSize, 0)) < 0)
	{
		perror("Receive failed with error ");
		n = 0;
	}
	return n;
}

int ServeurTCP::ReadPlayer(const SOCKET param1, Header *header)
{
	int n = 0;

	if ((n = recv(param1, (char*)header, sizeof(Header), 0)) < 0)
	{
		perror("Receive failed with error ");
		n = 0;
	}
	return n;
}

void ServeurTCP::CheckIsPlayerTalking(fd_set *rdfs)
{
	int cntPlayers;
	Header *header = new Header();

	for (cntPlayers = (int)this->players.size() - 1; cntPlayers >= 0; cntPlayers--)
	{
		Player& player = this->players[cntPlayers];
		if (FD_ISSET(player.GetFd(), rdfs))
		{
			int c = ReadPlayer(player.GetFd(), header);
			if (c == 0)
			{
				closesocket(player.GetFd());
				this->players.erase(std::find(this->players.begin(), this->players.end(), player));
			}
			else if (c > 0)
			{
				void (ServeurTCP::*ptr)(Player&, size_t);
				ptr = this->Instructions[header->command];
				if (ptr != NULL)
					(this->*ptr)(player, header->argsSize);
			}
			break;
		}
	}
}

void ServeurTCP::GetHighestSocket(int &max, fd_set* rdfs)
{
	for (auto player : players)
	{
		FD_SET(player.GetFd(), rdfs);
		if (player.GetFd() > max)
			max = (int)player.GetFd();
	}
}

void ServeurTCP::CreateLobby(Player &actualPlayer, size_t argsSize)
{
	SHA256 sha;
	LobbyInfo *lobbyinfo = new LobbyInfo();
	Header *header = new Header();
	header->argsSize = -1;

	this->ReadPlayer(actualPlayer.GetFd(), lobbyinfo, argsSize);
	if (this->lobbies.find(lobbyinfo->LobbyName) != this->lobbies.end())
	{
		header->command = "KO";
	}
	else
	{
		this->lobbies[lobbyinfo->LobbyName] = Lobby(sha(lobbyinfo->PasswdHash));
		if (this->lobbies[lobbyinfo->LobbyName].GetPlayers().size() < 4)
		{
			header->command = "OK";
			this->lobbies[lobbyinfo->LobbyName].GetPlayers().push_back(actualPlayer);
			actualPlayer.SetLobbyName(lobbyinfo->LobbyName);
		}
		else
			header->command = "KO";
	}

	this->WritePlayer(actualPlayer.GetFd(), header);
}

void ServeurTCP::JoinLobby(Player &actualPlayer, size_t argsSize)
{
	LobbyInfo *lobbyName = new LobbyInfo();
	Header *header = new Header();
	header->argsSize = -1;
	SHA256 sha;

	this->ReadPlayer(actualPlayer.GetFd(), lobbyName, argsSize);
	if (this->lobbies.find(lobbyName->LobbyName) == this->lobbies.end())
	{
		header->command = "KO";
	}
	else
	{
		if (this->lobbies[lobbyName->LobbyName].GetPlayers().size() < 4 && this->lobbies[lobbyName->LobbyName].GetPassword() == sha(lobbyName->PasswdHash))
		{
			this->lobbies[lobbyName->LobbyName].GetPlayers().push_back(actualPlayer);
			actualPlayer.SetLobbyName(lobbyName->LobbyName);
			header->command = "OK";
		}
		else
			header->command = "KO";
	}
	this->WritePlayer(actualPlayer.GetFd(), header);
}

void ServeurTCP::ExitLobby(Player &actualPlayer, size_t argsSize)
{
	Header *header = new Header();
	header->argsSize = -1;

	std::vector<Player> &lobbyPlayers = this->lobbies[actualPlayer.GetLobbyName()].GetPlayers();

	lobbyPlayers.erase(std::find(lobbyPlayers.begin(), lobbyPlayers.end(), actualPlayer));
	if (lobbyPlayers.size() == 0)
		this->lobbies.erase(actualPlayer.GetLobbyName());
	header->command = "OK";
	this->WritePlayer(actualPlayer.GetFd(), header);
}

void ServeurTCP::ListLobbies(Player &actualPlayer, size_t argsSize)
{
	(void)argsSize;
}

void ServeurTCP::ListPlayers(Player &actualPlayer, size_t argsSize)
{
	std::vector<Player> players = this->lobbies[actualPlayer.GetLobbyName()].GetPlayers();
	Header *header = new Header();
	header->command = "PLAYERS";

	header->argsSize = players.size();
	this->WritePlayer(actualPlayer.GetFd(), header);
	for (auto player : players)
	{
		player.SetLobbyName(actualPlayer.GetLobbyName());
		std::stringstream ss;

		ss << player;

		size_t size = strlen(ss.str().c_str());
		this->WritePlayer(actualPlayer.GetFd(), &size, sizeof(size_t));	
		this->WritePlayer(actualPlayer.GetFd(), (char*)ss.str().c_str(), size);
	}
}

void ServeurTCP::IsReady(Player &actualPlayer, size_t argsSize)
{
	Header *header = new Header();
	header->argsSize = -1;
	std::vector<Player> &lobbyPlayers = this->lobbies[actualPlayer.GetLobbyName()].GetPlayers();

	auto it = std::find(lobbyPlayers.begin(), lobbyPlayers.end(), actualPlayer);
	if ((*it).IsReady())
		(*it).SetReady(false);
	else
		(*it).SetReady(true);
	header->command = "OK";
	this->WritePlayer(actualPlayer.GetFd(), header);
}

void ServeurTCP::StartGame(Player &actualPlayer, size_t argsSize)
{
	std::vector<Player> &lobbyPlayers = this->lobbies[actualPlayer.GetLobbyName()].GetPlayers();
	Header *header = new Header();
	header->argsSize = -1;

	int cnt = 0;
	for (auto it : lobbyPlayers)
	{
		if (it.IsReady())
			cnt++;
	}

	if (cnt == lobbyPlayers.size())
	{
		this->lobbies[actualPlayer.GetLobbyName()].GameThread = new std::thread(&Lobby::StartGame, this->lobbies[actualPlayer.GetLobbyName()]);
	}

	//	header->command = "OK";
	//else
	//	header->command = "KO";
	//this->WritePlayer(actualPlayer.GetFd(), header);
}

void ServeurTCP::InitializeInstructions()
{
	this->Instructions["CREATE_LOBBY"] = &ServeurTCP::CreateLobby;
	this->Instructions["JOIN_LOBBY"] = &ServeurTCP::JoinLobby;
	this->Instructions["EXIT_LOBBY"] = &ServeurTCP::ExitLobby;
	this->Instructions["LIST_LOBBIES"] = &ServeurTCP::ListLobbies;
	this->Instructions["LIST_PLAYERS"] = &ServeurTCP::ListPlayers;
	this->Instructions["IS_READY"] = &ServeurTCP::IsReady;
	this->Instructions["START_GAME"] = &ServeurTCP::StartGame;
}
