#define _SCL_SECURE_NO_WARNINGS

#include <array>
#include "Player.h"

void Player::SetReady(bool state)
{
	this->ready = state;
}

bool Player::IsReady() const
{
	return this->ready;
}

void Player::SetLobbyName(std::string newName)
{
	this->lobbyName = newName;
}

const std::string & Player::GetLobbyName() const
{
	return this->lobbyName;
}

const SOCKET Player::GetFd() const
{
	return this->fd;
}

SOCKADDR_IN *Player::GetSin() const
{
	return this->Sin;
}

void Player::SetSin(const SOCKADDR_IN &csin)
{
	this->Sin = new SOCKADDR_IN(csin);
}

Player::Player(const SOCKADDR_IN &csin)
{
	this->Sin = new SOCKADDR_IN(csin);
}

Player::Player(SOCKET &_fd)
{
	this->fd = _fd;
}

Player::~Player()
{
}

bool operator==(const Player& pl1, const Player& pl2)
{
	return (pl1.GetFd() == pl2.GetFd());
}

bool operator==(const Player& pl1, SOCKET &csock)
{
	return (pl1.GetFd() == csock);
}

std::ostream& operator<< (std::ostream& out, Player& object) {
	out << object.ready << " " << object.lobbyName;   //The space (" ") is necessari for separete elements
	return out;
}

std::istream& operator>> (std::istream& in, Player& object) {
	in >> object.ready;
	in >> object.lobbyName;
	return in;
}