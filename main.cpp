
#include <iostream>
#include "gui.hpp"
#include <string>
namespace db {
    int i = 1;
};

struct Page : Context {
    int w = 50;
    int h = 50;
    std::vector<std::vector<int>> buttons = {
        {100, 100, 0},
        {200, 100, 0},
        {100, 200, 0},
        {200, 200, 0}
    };
    std::vector<int> commands = { 1, 2, 3, 4, 1 };
    std::vector<int> executed_commands;
    bool awaiting_input = false;
    void draw() {
        
        backStyle = ray::RAYWHITE;
        clearBack();
        db::i++;
        if (!awaiting_input) {
            commands.push_back(random(1, 4));
            awaiting_input = true;
        }
        for (int i = 0; i < commands.size(); i++) {
            setTimeout([this, i]() {
                buttons[(i == 0) ? (commands.size() - 1) : (i - 1)][2] = 0;
                std::cout << "hi";
                if (buttons[i][2] == 0) {
                    fillRect(buttons[i][0], buttons[i][1], w, h);
                    buttons[i][2] = 1;
                }
            }, DelayBuffer(i, i + 2, 0.0), std::to_string(i));
        }
        

        /*setTimeout([this]() {
            std::cout << "hi";
            fillRect(200, 200, 50, 50);
        }, DelayBuffer(2.0, 2.0, 0.0), "2");*/
        /*fillStyle = ray::RED;
        fillRect(x, y, w, 200);
        fillRect(400, 300, 100, 200);
        fillStyle = ray::BLUE;
        fillCircle(100, 100, 200);
        fillCircle(400, 300, 100);
        text("Simple Game", 20, 20, 20);
        text(std::to_string(db::i), 200, 200, 20);*/

    }
    virtual void onKeyPressed(int k) {
        //if (k == ray::KEY_A) x -= 1;
    }
    virtual void onKeyDown(int k) {
        //if (k == ray::KEY_A) x -= 1;
    }
    virtual void onKeyReleased(int k) {
        //if (k == ray::KEY_A) x += 100;
    }
    virtual void onMouseDown(MouseEvent m) {
        //if (m.m == ray::MOUSE_BUTTON_LEFT && m.x > 0 && m.x < 200 && m.y > 0 && m.y < 200) w -= 1;
    }
    virtual void onMouseReleased(MouseEvent m) {
        //if (m.m == ray::MOUSE_BUTTON_LEFT && m.x > 0 && m.x < 200 && m.y > 0 && m.y < 200) w += 100;
    }
    using Context::Context;
};
int main(void)
{
    
    const int screenWidth = 1920*0.75; // 1440
    const int screenHeight = 1080*0.75; // 810
    Page context(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing", 60);

    context.run();
    
    return 0;
}
