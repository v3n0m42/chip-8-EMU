# include <SFML/Graphics.hpp>
#include "Keyboard.hpp"

Keyboard	*createInput()
{
	return new Keyboard();
}

void		deleteInput(Keyboard *input)
{
	delete input;
}

Keyboard::Keyboard(void)
{
	unsigned char	i;

	this->escape = false;
	this->newKey = false;
	this->speedup = false;
	this->slowdown = false;
	this->saveConfig = false;
	for (i = 0; i < 16; i++)
		this->keys[i] = false;
}

Keyboard::~Keyboard(void)
{}

void		Keyboard::updateInput(void)
{
	unsigned char	i;

	this->escape = false;
	this->newKey = false;
	for (i = 0; i < 16; i++)
		this->keys[i] = false;

	if (sf::Keyboard::isKeyPressed(ESCAPE))
	{
		this->escape = true;
		this->newKey = true;
	}
	if (!this->saveConfig && sf::Keyboard::isKeyPressed(SAVECONFIG))
		this->saveConfig = true;
	else
		this->saveConfig = false;
	if (!this->savestate && sf::Keyboard::isKeyPressed(SAVESTATE))
		this->savestate = true;
	else
		this->savestate = false;
	if (!this->loadstate && sf::Keyboard::isKeyPressed(LOADSTATE))
		this->loadstate = true;
	else
		this->loadstate = false;
	if (!this->speedup && sf::Keyboard::isKeyPressed(SPEEDUP))
		this->speedup = true;
	else
		this->speedup = false;
	if (!this->slowdown && sf::Keyboard::isKeyPressed(SLOWDOWN))
		this->slowdown = true;
	else
		this->slowdown = false;
	if (sf::Keyboard::isKeyPressed(KEY_0))
	{
		this->newKey = true;
		this->keys[0] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_1))
	{
		this->newKey = true;
		this->keys[1] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_2))
	{
		this->newKey = true;
		this->keys[2] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_3))
	{
		this->newKey = true;
		this->keys[3] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_4))
	{
		this->newKey = true;
		this->keys[4] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_5))
	{
		this->newKey = true;
		this->keys[5] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_6))
	{
		this->newKey = true;
		this->keys[6] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_7))
	{
		this->newKey = true;
		this->keys[7] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_8))
	{
		this->newKey = true;
		this->keys[8] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_9))
	{
		this->newKey = true;
		this->keys[9] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_A))
	{
		this->newKey = true;
		this->keys[10] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_B))
	{
		this->newKey = true;
		this->keys[11] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_C))
	{
		this->newKey = true;
		this->keys[12] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_D))
	{
		this->newKey = true;
		this->keys[13] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_E))
	{
		this->newKey = true;
		this->keys[14] = true;
	}
	if (sf::Keyboard::isKeyPressed(KEY_F))
	{
		this->newKey = true;
		this->keys[15] = true;
	}
}
