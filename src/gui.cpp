#include "gui.h"
#include <random>
#include <sstream>
#include <iostream>

const std::string FONT_PATH = "resources/fonts/DejaVuSans.ttf";

GUI::GUI()
: window(sf::VideoMode(1000, 600), "MiniOS GUI Simulator"), showError(false)
{
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Warning: could not load font at '" << FONT_PATH << "'. Text will not display.\n";
    }
    // buttons
    btnCreate.setSize({160,40}); btnCreate.setPosition(20,20);
    btnCreate.setFillColor(sf::Color(100,200,100));
    btnTerminate.setSize({160,40}); btnTerminate.setPosition(200,20);
    btnTerminate.setFillColor(sf::Color(200,100,100));
    btnFCFS.setSize({160,40}); btnFCFS.setPosition(380,20);
    btnFCFS.setFillColor(sf::Color(100,150,250));
    btnRR.setSize({160,40}); btnRR.setPosition(560,20);
    btnRR.setFillColor(sf::Color(200,150,50));

    txtCreate.setFont(font); txtCreate.setString("Create Process"); txtCreate.setCharacterSize(16);
    txtCreate.setPosition(30,28);
    txtTerminate.setFont(font); txtTerminate.setString("Terminate Last"); txtTerminate.setCharacterSize(16);
    txtTerminate.setPosition(210,28);
    txtFCFS.setFont(font); txtFCFS.setString("Run FCFS"); txtFCFS.setCharacterSize(16);
    txtFCFS.setPosition(400,28);
    txtRR.setFont(font); txtRR.setString("Run Round Robin"); txtRR.setCharacterSize(16);
    txtRR.setPosition(570,28);

    title.setFont(font); title.setCharacterSize(20); title.setString("MiniOS GUI Simulator - SFML"); title.setPosition(20,80);
}

void GUI::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void GUI::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        else if (event.type == sf::Event::MouseButtonPressed) {
            auto pos = sf::Mouse::getPosition(window);
            sf::Vector2f p((float)pos.x, (float)pos.y);
            if (btnCreate.getGlobalBounds().contains(p)) createRandomProcess();
            else if (btnTerminate.getGlobalBounds().contains(p)) {
                // terminate last created ready process (highest pid that's not terminated)
                auto procs = scheduler.list_processes();
                int last = -1;
                for (auto &pr : procs) if (pr->state != ProcState::Terminated) last = pr->pid;
                if (last != -1) scheduler.terminate_process(last);
            } else if (btnFCFS.getGlobalBounds().contains(p)) {
                scheduler.run_fcfs();
            } else if (btnRR.getGlobalBounds().contains(p)) {
                scheduler.run_rr(2); // quantum 2
            }
        }
    }
}

void GUI::update() {
    // could animate or step scheduler; currently actions happen on button clicks
}

void GUI::render() {
    window.clear(sf::Color(30,30,30));
    drawButtons();
    drawProcessList();
    drawMemoryMap();
    window.display();
}

void GUI::drawButtons() {
    window.draw(btnCreate); window.draw(btnTerminate); window.draw(btnFCFS); window.draw(btnRR);
    if (font.getInfo().family != "") {
        window.draw(txtCreate); window.draw(txtTerminate); window.draw(txtFCFS); window.draw(txtRR);
        window.draw(title);
    }
}

void GUI::drawProcessList() {
    auto procs = scheduler.list_processes();
    float startY = 120;
    float x = 20, w = 460, h = 28;
    // header
    if (font.getInfo().family != "") {
        sf::Text hdr("PID  Name       Pri  Mem  Burst  Rem  State", font, 14); hdr.setPosition(x, startY-20);
        window.draw(hdr);
    }
    for (size_t i=0;i<procs.size();++i) {
        auto &p = procs[i];
        sf::RectangleShape row({w,h}); row.setPosition(x, startY + i*(h+6));
        if (p->state == ProcState::Terminated) row.setFillColor(sf::Color(60,60,60));
        else row.setFillColor(sf::Color(80,80,100));
        window.draw(row);
        if (font.getInfo().family != "") {
            std::ostringstream ss;
            ss<<p->pid<<"    "<<p->name<<"    "<<p->priority<<"    "<<p->memory<<"    "<<p->burst<<"    "<<p->remaining<<"    ";
            std::string st;
            switch (p->state){case ProcState::Ready: st="Ready";break;case ProcState::Running: st="Running";break;case ProcState::Waiting: st="Waiting";break;case ProcState::Terminated: st="Terminated";break;}
            ss<<st;
            sf::Text txt(ss.str(), font, 14); txt.setPosition(x+6, startY + i*(h+6)+4);
            window.draw(txt);
        }
    }
}

void GUI::drawMemoryMap() {
    // draw on right side
    float x = 500, y = 120, w = 460, h = 30;
    // total bar background
    sf::RectangleShape bg({w, h}); bg.setPosition(x, y); bg.setFillColor(sf::Color(50,50,50));
    window.draw(bg);

    auto blocks = scheduler.memory_manager().blocks();
    int total = 0; for (auto &b: blocks) total += b.size;
    float scale = w / float(total > 0 ? total : 1);
    float cx = x;
    for (auto &b: blocks) {
        float bw = b.size * scale;
        sf::RectangleShape r({bw, h}); r.setPosition(cx, y);
        if (b.pid==0) r.setFillColor(sf::Color(100,200,100)); else r.setFillColor(sf::Color(200,100,100));
        window.draw(r);
        // pid text
        if (font.getInfo().family != "") {
            std::ostringstream ss; ss << (b.pid==0?"free":("PID=" + std::to_string(b.pid)));
            sf::Text t(ss.str(), font, 12); t.setPosition(cx+4, y+6);
            window.draw(t);
        }
        cx += bw;
    }
    // legend
    if (font.getInfo().family != "") {
        sf::Text legend("Memory (MB) - free / allocated", font, 14); legend.setPosition(x, y-24);
        window.draw(legend);
    }
}

void GUI::createRandomProcess() {
    static std::mt19937 rng((unsigned)time(nullptr));
    static int counter = 1;
    std::uniform_int_distribution<int> prio(1,10);
    std::uniform_int_distribution<int> mem(10,300); // MB
    std::uniform_int_distribution<int> burst(1,10);
    std::ostringstream name; name<<"P"<<counter++;
    int pid = scheduler.create_process(name.str(), prio(rng), mem(rng), burst(rng));
    if (pid<0) {
        showError = true;
        errorMsg = "Memory allocation failed: not enough memory.";
        std::cerr << errorMsg << std::endl;
    }
}
