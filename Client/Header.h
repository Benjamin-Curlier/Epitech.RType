#pragma once

#include <string>

struct Header
{
	std::string command;
	size_t argsSize;
};

struct LobbyInfo
{
	std::string LobbyName;
	std::string PasswdHash;
};

struct CreateObject
{
	float posX;
	float posY;
	int type;
	int typebullet;
	int typeexplosion;
	int score;
	int hp;
	bool alive;

	CreateObject() {}
	CreateObject(float x, float y, int t, int tb, int te, int sc, int h, bool a) : posX(x), posY(y), type(t), typebullet(tb), typeexplosion(te), score(sc), hp(h), alive(a) {
	}
};

struct Input
{
	bool input[5];
	int idplayer;

	Input() { }
	Input(std::map<sf::Keyboard::Key, int> keys, int playerid) {
		this->input[0] = keys[sf::Keyboard::Z];
		this->input[1] = keys[sf::Keyboard::Q];
		this->input[2] = keys[sf::Keyboard::S];
		this->input[3] = keys[sf::Keyboard::D];
		this->input[4] = keys[sf::Keyboard::Space];
		this->idplayer = playerid;
	}

	friend std::ostream& operator<< (std::ostream& out, Input& object) {
		out << object.input[0] << " " << object.input[1] << " " << object.input[2] << " " << object.input[3] << " " << object.input[4];
		return out;
	}
	
	friend std::istream& operator>> (std::istream& in, Input& object) {
		in >> object.input[0];
		in >> object.input[1];
		in >> object.input[2];
		in >> object.input[3];
		in >> object.input[4];
		return in;
	}
};