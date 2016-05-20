#ifndef IRENDER_HPP
# define IRENDER_HPP

# define BLACK					0
# define WHITE					1

class	IRender
{
	public:
		virtual					~IRender(void) {}
		virtual void			drawPixel(unsigned char x, unsigned char y) = 0;
		virtual void			clearScreen(void) = 0;
		virtual void			updateScreen(void) = 0;
		virtual void			setPixel(unsigned char x, unsigned char y, unsigned char color) = 0;
		virtual unsigned char	getPixelColor(unsigned char x, unsigned char y) const = 0;
		virtual char			**getScreen(void) const = 0;
		virtual void			setScreen(char **vram) = 0;
};

#endif
