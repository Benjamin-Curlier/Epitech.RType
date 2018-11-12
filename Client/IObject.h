#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>

enum Bonus_action
{
	HP,
	FIRE_RATE
};

enum Action_Update
{
	NOTHING,
	TO_DELETE,
};

struct S_sound
{
	S_sound(std::string path)
	{
		this->_buffer.loadFromFile(path);
		this->_sound.setBuffer(this->_buffer);
	}
	sf::SoundBuffer _buffer;
	sf::Sound _sound;
};

struct S_sprite
{
	S_sprite(std::string path, sf::Vector2f size)
	{
		this->_texture.loadFromFile(path, sf::IntRect(0, 0, size.x, size.y));
		this->_sprite.setTexture(this->_texture);
	}
	sf::Texture _texture;
	sf::Sprite _sprite;
};

class IObject
{
public:
	IObject(sf::RenderWindow *w, sf::Vector2f pos, std::vector<S_sprite *> sprites)
	{
		this->_w = w;
		this->_sprites = sprites;
		this->_pos = pos;
	}
	sf::Vector2f getPos() const { return _pos; }
	void setPos(sf::Vector2f pos) { this->_pos = pos; }
	void draw(int i) 
	{ 
		this->_sprites.at(i)->_sprite.setPosition(this->_pos);
		this->_w->draw(this->_sprites.at(i)->_sprite);
	}
	void setAlive(bool a) { this->_is_alive = a; }
	virtual bool getAlive() const { return this->_is_alive; }
	virtual Action_Update update(const float) = 0;
protected:
	bool _is_alive = true;
	sf::RenderWindow *_w;
	std::vector<S_sprite *> _sprites;
	sf::Vector2f _pos;
};

