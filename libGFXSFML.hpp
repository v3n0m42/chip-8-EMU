#ifndef RENDER_HPP
# define RENDER_HPP

# include <SFML/Graphics.hpp>
# include "IRender.hpp"

# define WINDOW_TITLE		"BC-Chip8 EMU"

class	SFMLRenderer : public IRender
{
	public:
		SFMLRenderer(void);
		~SFMLRenderer(void);
		void				drawPixel(unsigned char x, unsigned char y);
		void				clearScreen(void);
		void				updateScreen(void);
		void				setPixel(unsigned char x, unsigned char y, unsigned char color);
		unsigned char		getPixelColor(unsigned char x, unsigned char y) const;
		char				**getScreen(void) const;
		void				setScreen(char **vram);

	private:
		bool				_displayFPS;
		sf::RenderWindow	*_window;
		unsigned short		_fps, _pixelSize, _width, _height;
		sf::Texture			_white;
		char				_pixels[64][32];	//Virtual Screen
		sf::Sprite			**_whiteSprites;
		sf::Uint8			*_whitePixel;
		std::string			_windowTitle;
		sf::Clock			_clock;
};

extern "C"
{
	SFMLRenderer			*createRenderer(void);
	void					deleteRenderer(SFMLRenderer *renderer);
}

#endif
