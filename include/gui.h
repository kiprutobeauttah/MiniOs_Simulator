#pragma once
#include <SFML/Graphics.hpp>
#include "scheduler.h"

class GUI {
public:
    GUI();
    void run();
private:
    void processEvents();
    void update();
    void render();
    void drawButtons();
    void drawProcessList();
    void drawMemoryMap();
    void createRandomProcess();

    sf::RenderWindow window;
    sf::Font font;
    Scheduler scheduler;

    // simple UI
    sf::RectangleShape btnCreate, btnTerminate, btnFCFS, btnRR;
    sf::Text txtCreate, txtTerminate, txtFCFS, txtRR, title;
    bool showError;
    std::string errorMsg;
};
