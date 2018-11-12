#pragma once

#include <SFML/Network.hpp>
#include <stack>

class GameState;

class Game
{
public:
	Game();
	~Game();
	std::stack<GameState*> states;
	void PushState(GameState* state);
	void PopState();
	void ChangeState(GameState* state);
	GameState* PeekState();
	void GameLoop();
	sf::RenderWindow *_rw;
	sf::TcpSocket tcp;
	sf::UdpSocket udp;
	std::string serverAdress;
	int port;
	int idplayer;
};

