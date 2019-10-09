#ifndef RENDER_HPP
# define RENDER_HPP

# include <SFML/Graphics.hpp>
# include "IRender.hpp"

# define WINDOW_TITLE   "BC-Chip8 EMU"

class   SFMLRenderer final : public IRender
{
    static constexpr uint16_t   s_pixelCount = 64 * 32;
    static constexpr uint8_t    s_componentsPerPixel = 4;
    static constexpr uint16_t   s_renderTargetSize = s_pixelCount * s_componentsPerPixel;

	public:
		SFMLRenderer();
		~SFMLRenderer();
		void              ClearScreen() override;
		void              UpdateScreen() override;
		void              SetPixel(uint8_t _x, uint8_t _y, uint8_t _color) override;
		unsigned char     GetPixelColor(uint8_t _x, uint8_t _y) const override;
		char**            GetScreen() const override;
		void              SetScreen(char** _vram) override;

	private:
        inline constexpr uint16_t    GetFlatArrayIndex(uint8_t _x, uint8_t _y) const;

		bool              m_displayFPS{ false };
		sf::RenderWindow  *m_window{ nullptr };
		uint8_t           m_fps;
        uint16_t          m_pixelSize;
        uint16_t          m_width, m_height;
        sf::Uint8         m_pixels[s_renderTargetSize];
		std::string       m_windowTitle{ WINDOW_TITLE };
		sf::Clock         m_clock;
        sf::Texture       m_renderTargetTexture;
        sf::Sprite        m_renderTargetSprite;
};

extern "C"
{
    SFMLRenderer*  CreateRenderer();
    void           DeleteRenderer(SFMLRenderer* _renderer);
}

#endif
