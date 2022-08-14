#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <vector>
#include <SDL_render.h>

struct LabeledRect {
    uint8_t SrcID;
    SDL_Rect Rect;

    LabeledRect() : SrcID(0), Rect{ 0, 0, 0, 0 } {}
    LabeledRect(uint8_t id, const SDL_Rect& rect) : SrcID(id), Rect(rect) {}
    LabeledRect(uint8_t id, int x, int y, int w, int h) : SrcID(id), Rect{ x, y, w, h } {}
};

class Texture {
public:
    Texture(SDL_Texture* data, int w, int h);
    ~Texture();

    void destroy();

    inline SDL_Texture* data() {
        return m_Data;
    }

    constexpr int getWidth() const {
        return m_Width;
    }

    constexpr int getHeight() const {
        return m_Height;
    }
    
    inline std::vector<LabeledRect>& getSrc() {
        return m_Src;
    }

    inline const std::vector<LabeledRect>& getSrc() const {
        return m_Src;
    }
    
    inline std::vector<LabeledRect>& getDst() {
        return m_Dst;
    }

    inline const std::vector<LabeledRect>& getDst() const {
        return m_Dst;
    }

private:
    SDL_Texture* m_Data;
    int m_Width, m_Height;
    std::vector<LabeledRect> m_Src;
    std::vector<LabeledRect> m_Dst;
};

#endif // !TEXTURE_H_