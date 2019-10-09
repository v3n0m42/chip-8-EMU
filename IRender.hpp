#ifndef IRENDER_HPP
# define IRENDER_HPP

class   IRender
{
    public:
        virtual             ~IRender() {}
        virtual void        ClearScreen() = 0;
        virtual void        UpdateScreen() = 0;
        virtual void        SetPixel(uint8_t _x, uint8_t _y, uint8_t _color) = 0;
        virtual uint8_t     GetPixelColor(uint8_t _x, uint8_t _y) const = 0;
        virtual char**      GetScreen() const = 0;
        virtual void        SetScreen(char** _vram) = 0;
};

#endif
