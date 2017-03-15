#pragma once
#include <ThreadSafe.h>
#include <atomic>
#define VERDANA_FONT "../Engine/Resources/Fonts/verdana.spritefont"


class Text2D
{
    std::string m_Text;
    thread_safe<std::string> m_newText;
    std::atomic<float> m_PosX, m_PosY;

    void update_text();
public:
    bool initialize(const std::string &font = VERDANA_FONT);
    void set_text(const std::string &);
    void set_text(std::string &&);
    void set_coord(float x, float y);
    void shutdown();
    void render();
    Text2D();
    ~Text2D();
};

