
#include <iostream>
#include "everlasting_framework.hpp"
#include <string>

struct Page : Context {
    int w = 50;
    int h = 50;
    std::vector<Button::RectButton> buttons = {
        Button::RectButton(100, 100, w, h),
        Button::RectButton(200, 100, w, h),
        Button::RectButton(100, 200, w, h),
        Button::RectButton(200, 200, w, h)
    };
    std::vector <ray::Color> button_colors = { ray::BLUE, ray::RED, ray::GREEN, ray::YELLOW };
    std::vector<int> commands = { 1 };
    int steps = 0;
    std::vector<int> executed_commands;
    bool awaiting_input = false;
    bool green = false;
    int score = 0;
    int high_score = 0;
    void animate() {
        std::cout << commands.size();
        for (int i = 0; i < (commands.size() * 2); i++) {
            if ((i % 2)) {
                setTimeout([this, i]() {
                    fillStyle = ray::BLUE;
                    if (steps == i - 1) {
                        steps++;
                    }
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
        if (!green && !awaiting_input) {
            commands.push_back(random(0, 3));
            steps = 0;
            score = 0;
            animate();
            awaiting_input = true;
        }
        if (green) {
            fillStyle = ray::GREEN;
            fillRect(600, 600, 100, 100);
            for (int i = 0; i < buttons.size(); i++) {
                fillStyle = button_colors[i];
                fillRect(buttons[i].rect);
            }
        }
        else {
            fillStyle = ray::RED;
            fillRect(600, 600, 100, 100);
        }
        text("Score: " + std::to_string(score), 500, 200, 20);
        text("High Score: " + std::to_string(high_score), 500, 300, 20);
    }
    virtual void onKeyPressed(int k) {
    }
    virtual void onKeyDown(int k) {
    }
    virtual void onKeyReleased(int k) {
    }
    virtual void onMousePressed(MouseEvent m) {
        if (green) {
            for (int i = 0; i < buttons.size(); i++) {
                bool test = buttons[i].checkCollision(m.position);
                if (test && commands[steps] == i) {
                    steps += 1;
                    score++;
                    if (high_score < score) high_score = score;
                }
                else if (test) {
                    commands = {};
                    awaiting_input = false;
                    green = false;
                }
                else continue;
                break;
            }
        }

        if (steps == commands.size()) {
            std::cout << std::endl << "true" << std::endl;
            awaiting_input = false;
            green = false;
        }
    }
    virtual void onMouseReleased(MouseEvent m) {
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
