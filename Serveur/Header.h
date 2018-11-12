#pragma once

#include <string>

struct Header
{
	std::string command;
	size_t argsSize;
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

	CreateObject(float x, float y, int t, int tb, int te, int sc, int h, bool a) : posX(x), posY(y), type(t), typebullet(tb), typeexplosion(te), score(sc), hp(h), alive(a) {
	}
};

struct Input
{
	bool input[5];
	int idplayer;

	Input() { }

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