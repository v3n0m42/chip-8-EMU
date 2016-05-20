#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include "Config.hpp"
#include "libGFXSFML.hpp"

SFMLRenderer	*createRenderer()
{
	return new SFMLRenderer();
}

void			deleteRenderer(SFMLRenderer *renderer)
{
	delete renderer;
}

SFMLRenderer::SFMLRenderer(void) : _displayFPS(false), _window(NULL), _fps(60), _pixelSize(8), _width(512), _height(256), _whiteSprites(NULL), _whitePixel(NULL), _windowTitle(WINDOW_TITLE)
{
	unsigned short	x, y;
	Config			config("libGFXSFML.cfg");

	//Load Config
	x = config.getUShort("DISPLAY_FPS", 0);
	if (x == 1)
		this->_displayFPS = true;
	this->_fps = config.getUShort("FPS", 60);
	this->_pixelSize = config.getUShort("PIXEL_SIZE", 8);
	this->_width = this->_pixelSize * 64;
	this->_height = this->_pixelSize * 32;

	//Init pixels of virtual screen
	for (x = 0; x < 64; x++)
		for (y = 0; y < 32; y++)
			this->_pixels[x][y] = BLACK;

	//Init Screen
	this->_window = new sf::RenderWindow(sf::VideoMode(this->_width, this->_height), WINDOW_TITLE);
	this->_window->setVerticalSyncEnabled(true);
	this->_window->setFramerateLimit(this->_fps);

	//Init Sprites
	this->_whitePixel = new sf::Uint8[this->_pixelSize * this->_pixelSize * 4];

	//Init Pixels
	for (x = 0; x < (this->_pixelSize * this->_pixelSize * 4); x += 4)
	{
		this->_whitePixel[x] = 255;
		this->_whitePixel[x + 1] = 255;
		this->_whitePixel[x + 2] = 255;
		this->_whitePixel[x + 3] = 255;
	}
	this->_white.create(this->_pixelSize, this->_pixelSize);
	this->_white.update(this->_whitePixel);

	this->_whiteSprites = new sf::Sprite*[64];
	for (x = 0; x < 64; x++)
	{
		this->_whiteSprites[x] = new sf::Sprite[32];
		for (y = 0; y < 32; y++)
		{
			this->_whiteSprites[x][y].setTexture(this->_white);
			this->_whiteSprites[x][y].setPosition(float(x * this->_pixelSize), float(y * this->_pixelSize));
		}
	}
	this->_clock.restart();
}

SFMLRenderer::~SFMLRenderer(void)
{
	this->_window->close();
	delete [] this->_whiteSprites;
	delete [] this->_whitePixel;
}

char			**SFMLRenderer::getScreen(void) const
{
	char			**screen = NULL;
	unsigned char	i;

	screen = (char **)malloc(sizeof(char *) * 64);
	if (!screen)
		return NULL;
	for (i = 0; i < 64; i++)
	{
		screen[i] = (char *)malloc(sizeof(char) * 32);
		if (!screen[i])
			return NULL;
		memcpy(screen[i], this->_pixels[i], 32);
	}
	return screen;
}

void			SFMLRenderer::setScreen(char **vram)
{
	unsigned char	i;

	for (i = 0; i < 64; i++)
		memcpy(this->_pixels[i], vram[i], 32);
}

void			SFMLRenderer::drawPixel(unsigned char x, unsigned char y)
{
	if (this->_pixels[x][y] == BLACK)
		return;
	this->_window->draw(this->_whiteSprites[x][y]);
}

void			SFMLRenderer::clearScreen(void)
{
	unsigned char	x, y;

	for (x = 0; x < 64; x++)
		for (y = 0; y < 32; y++)
			this->_pixels[x][y] = BLACK;
	this->_window->clear(sf::Color::Black);
	this->_window->display();
}

void			SFMLRenderer::updateScreen(void)
{
	unsigned char		x, y;
	std::stringstream	ss;
	sf::Time			elapsed;

	this->_window->clear(sf::Color::Black);
	for (x = 0; x < 64; x++)
		for (y = 0; y < 32; y++)
			this->drawPixel(x, y);
	this->_window->display();
	if (this->_displayFPS)
	{
		std::string		title(this->_windowTitle);
		elapsed = this->_clock.getElapsedTime();
		ss << 1000 / elapsed.asMilliseconds();
		std::string		strTime(ss.str());
		this->_window->setTitle(title + " - " + strTime + " FPS");
	}
	this->_clock.restart();
}

void			SFMLRenderer::setPixel(unsigned char x, unsigned char y, unsigned char color)
{
	this->_pixels[x][y] = color;
}

unsigned char	SFMLRenderer::getPixelColor(unsigned char x, unsigned char y) const
{
	return this->_pixels[x][y];
}
