
#include <iostream>
#include "gui.hpp"
#include <string>
namespace db {
    int i = 1;
};

struct Page : Context {
    int x = 100;
    int y = 100;
    int w = 200;
    std::vector<int> commands;
    std::vector<int> executed_commands;
    bool awaiting_input = false;
    void func () {
        fillRect(100, 100, 50, 50);
    }
    void draw() {
        std::cout << "hi";
        backStyle = ray::RAYWHITE;
        clearBack();
        db::i++;
        if (!awaiting_input) {
            commands.push_back(random(1, 4));
            awaiting_input = true;
        }
        
        DelayBuffer buff(0.0, 1.0, 0.0);
        setTimeout(func, buff, "1");
        //test.execute();
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
        if (k == ray::KEY_A) x -= 1;
    }
    virtual void onKeyDown(int k) {
        if (k == ray::KEY_A) x -= 1;
    }
    virtual void onKeyReleased(int k) {
        if (k == ray::KEY_A) x += 100;
    }
    virtual void onMouseDown(MouseEvent m) {
        if (m.m == ray::MOUSE_BUTTON_LEFT && m.x > 0 && m.x < 200 && m.y > 0 && m.y < 200) w -= 1;
    }
    virtual void onMouseReleased(MouseEvent m) {
        if (m.m == ray::MOUSE_BUTTON_LEFT && m.x > 0 && m.x < 200 && m.y > 0 && m.y < 200) w += 100;
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