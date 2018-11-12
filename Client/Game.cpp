#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "GameState.h"
#include "Game.h"

Game::Game()
{
	this->_rw = new sf::RenderWindow(sf::VideoMode(1920, 720), "RTYPE");
}

Game::~Game()
{
}

void Game::PushState(GameState* state)
{
	this->states.push(state);
	return;
}

void Game::PopState()
{
	delete this->states.top();
	this->states.pop();
	return;
}

void Game::ChangeState(GameState* state)
{
	if (!this->states.empty())
		PopState();
	PushState(state);

	return;
}

GameState* Game::PeekState()
{
	if (this->states.empty()) return nullptr;
	return this->states.top();
}

void Game::GameLoop()
{
	sf::Clock clock;

	while (this->_rw->isOpen())
	{
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		if (PeekState() == nullptr) continue;
		PeekState()->HandleInput();
		this->_rw->clear(sf::Color::Black);
		PeekState()->Update(dt);
		PeekState()->Draw(dt);
		this->_rw->display();
	}
}

