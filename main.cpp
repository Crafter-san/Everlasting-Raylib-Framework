
#include <iostream>
#include "gui.hpp"
#include <string>
namespace db {
    int i = 1;
};

struct Page : Context {
    int w = 50;
    int h = 50;
    std::vector<Button> buttons = {
        Button(100, 100, w, h),
        Button(200, 100, w, h),
        Button(100, 200, w, h),
        Button(200, 200, w, h)
    };
    std::vector <ray::Color> button_colors = { ray::BLUE, ray::RED, ray::GREEN, ray::YELLOW };
    std::vector<int> commands = { 1, 1 };
    int steps = 0;
    std::vector<int> executed_commands;
    bool awaiting_input = false;
    bool green = false;
    int score = 0;
    int high_score = 0;
    void animate() {
        for (int i = 0; i < (commands.size() * 2); i++) {
            if ((i % 2)) {
                setTimeout([this, i]() {
                    fillStyle = ray::BLUE;
                    if (steps == i - 1) {
                        steps++;
                    }
                    std::cout << std::endl << steps << std::endl << commands.size() * 2 << std::endl;
                    if (steps == (commands.size() * 2)-1) {
                        steps = 0;
                        green = true;
                    }
                    }, DelayBuffer(i, 1), std::to_string(i));
            }
            else {
                setTimeout([this, i]() {
                    fillStyle = ray::BLUE;
                    if (steps == i - 1) {
                        steps++;
                    }
                    if (steps == i) {

                        fillStyle = button_colors[commands[i / 2]];
                        fillRect(buttons[commands[i / 2]].rect);
                    }

                    }, DelayBuffer(i, 1), std::to_string(i));
            }
        }
    }
    void draw() {
        
        backStyle = ray::RAYWHITE;
        clearBack();
        db::i++;
        fillStyle = ray::BLACK;
        for (int i = 0; i < buttons.size(); i++) {
            rect(buttons[i].rect);
        }
        if (!awaiting_input) {
            animate();
            awaiting_input = true;
        }
        if (green) {
            fillStyle = ray::GREEN;
            fillRect(600, 600, 100, 100);
        }
        else {
            fillStyle = ray::RED;
            fillRect(600, 600, 100, 100);
        }
        if (green) {
            for (int i = 0; i < buttons.size(); i++) {
                fillStyle = button_colors[i];
                fillRect(buttons[i].rect);
            }
        }
        
        text("Score: " + std::to_string(score), 500, 200, 20);
        text("High Score: " + std::to_string(high_score), 500, 300, 20);
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
    virtual void onMousePressed(MouseEvent m) {
        if (green) {
            for (int i = 0; i < buttons.size(); i++) {
                bool test = buttons[i].checkCollision(m.position);
                if (test && commands[steps] == i) {
                    steps += 1;
                    score++;
                    if (high_score < score) high_score = score;
                    std::cout << std::endl << commands.size() << std::endl << steps << std::endl;


                    break;
                }
                else if (test) {
                    commands = {};
                    commands.push_back(random(0, 3));
                    score = 0;
                    awaiting_input = false;
                    green == false;
                    steps = 0;
                }
                //std::cout << std::endl<< commands.size() << std::endl;
            }
            if (steps == commands.size()) {
                std::cout << std::endl << "true" << std::endl;
                commands.push_back(random(0, 3));
                steps = 0;
                score = 0;
                awaiting_input = false;
                green = false;
            }
        }
         
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
