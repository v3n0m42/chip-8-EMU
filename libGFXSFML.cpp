#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include "Config.hpp"
#include "libGFXSFML.hpp"

////////////////////////////////////////////////////////////////////////////////

SFMLRenderer*   CreateRenderer()
{
    SFMLRenderer*   renderer = nullptr;
    if (sf::Shader::isAvailable())
        renderer = new SFMLRenderer();
    return renderer;
}

////////////////////////////////////////////////////////////////////////////////

void    DeleteRenderer(SFMLRenderer* _renderer)
{
    delete _renderer;
}

////////////////////////////////////////////////////////////////////////////////

SFMLRenderer::SFMLRenderer()
{
    Config  config("libGFXSFML.cfg");

    //Load Config
    m_displayFPS = (config.getUShort("DISPLAY_FPS", 0) == 1);
    m_fps = config.getUShort("FPS", 60);
    m_pixelSize = config.getUShort("PIXEL_SIZE", 8);
    m_width = m_pixelSize * 64;
    m_height = m_pixelSize * 32;

    //Init pixels of virtual screen
    memset(m_pixels, 0, s_renderTargetSize);

    //Init render target
    m_renderTargetTexture.create(64, 32);
    m_renderTargetSprite.setTexture(m_renderTargetTexture);
    m_renderTargetSprite.setScale(m_pixelSize, m_pixelSize);

    //Init Screen
    m_window = new sf::RenderWindow(sf::VideoMode(m_width, m_height), WINDOW_TITLE);
    m_window->setVerticalSyncEnabled(true);
    m_window->setFramerateLimit(m_fps);

    m_clock.restart();
}

////////////////////////////////////////////////////////////////////////////////

SFMLRenderer::~SFMLRenderer()
{
    m_window->close();
}

////////////////////////////////////////////////////////////////////////////////

char*   SFMLRenderer::GetScreen() const
{
	char* const   screen = (char*)malloc(sizeof(char) * s_pixelCount);
	if (!screen)
		return nullptr;
	for (uint16_t pixel = 0; pixel < s_pixelCount; pixel++)
        screen[pixel] = m_pixels[pixel * s_componentsPerPixel];
	return screen;
}

////////////////////////////////////////////////////////////////////////////////

void    SFMLRenderer::SetScreen(char* _vram)
{
    for (uint16_t pixel = 0; pixel < s_renderTargetSize / s_componentsPerPixel; pixel++)
        memset(m_pixels + (pixel * s_componentsPerPixel), _vram[pixel], s_componentsPerPixel);
}

////////////////////////////////////////////////////////////////////////////////

void    SFMLRenderer::ClearScreen()
{
    memset(m_pixels, 0, s_renderTargetSize);
    m_window->clear(sf::Color::Black);
    m_window->display();
}

////////////////////////////////////////////////////////////////////////////////

void    SFMLRenderer::UpdateScreen()
{
    m_window->clear(sf::Color::Black);
    m_renderTargetTexture.update(m_pixels);
    m_window->draw(m_renderTargetSprite);
    m_window->display();

    if (m_displayFPS)
    {
        std::string         title(m_windowTitle);
        const sf::Time      elapsed = m_clock.getElapsedTime();
        std::stringstream   ss;
        ss << 1000 / elapsed.asMilliseconds();
        std::string   strTime(ss.str());
        m_window->setTitle(title + " - " + strTime + " FPS");
    }

    m_clock.restart();
}

////////////////////////////////////////////////////////////////////////////////

void    SFMLRenderer::SetPixel(uint8_t _x, uint8_t _y, uint8_t _color)
{
    const uint16_t   flatArrayIndex = GetFlatArrayIndex(_x, _y);
    memset(m_pixels + flatArrayIndex, _color * 255, s_componentsPerPixel);
}

////////////////////////////////////////////////////////////////////////////////

uint8_t SFMLRenderer::GetPixelColor(uint8_t _x, uint8_t _y) const
{
    const uint16_t   flatArrayIndex = GetFlatArrayIndex(_x, _y);
    return m_pixels[flatArrayIndex] / 255;
}

////////////////////////////////////////////////////////////////////////////////

inline constexpr uint16_t    SFMLRenderer::GetFlatArrayIndex(uint8_t _x, uint8_t _y) const
{
    return ((64 * _y) + _x) * s_componentsPerPixel;
}

////////////////////////////////////////////////////////////////////////////////
