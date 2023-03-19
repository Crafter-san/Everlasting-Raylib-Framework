#include <stdarg.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <set>
namespace ray {
#include "raylib.h"
}
struct MouseEvent {
    int x = 0;
    int y = 0;
    int m = 0;
    ray::Vector2 position;
    MouseEvent(ray::Vector2 pos, int mb) {
        x = pos.x;
        y = pos.y;
        position = pos;
        m = mb;
    }
};
struct Button {
    ray::Rectangle rect;
    int w;
    int h;
    int x;
    int y;
    bool checkCollision(ray::Vector2 pos) {
        return ray::CheckCollisionPointRec(pos, rect);
    }
    Button(int x = 0, int y = 0, int w = 0, int h = 0) {
        rect = ray::Rectangle(x + (w / 2), y + (h / 2), w, h);
        this->w = w;
        this->h = h;
        this->x = x;
        this->y = y;
    }
};
struct DelayBuffer {
    double before;
    double during;
    double after;
    bool done_before = false;
    bool done_during = false;
    bool done_after = false;
    DelayBuffer(double b = 0.0, double d = 0.0, double a = 0.0) {
        before = b;
        during = d;
        after = a;
    }
};
struct TimeOut {
    double startTime;
    DelayBuffer buffer;
    std::function<void()> execute;
    std::string id;
    bool exists = false;
    void Update() {
        double time = ray::GetTime();
        double elapsedTime = time - startTime;
        //std::cout << std::endl << elapsedTime << std::endl;
        if (elapsedTime >= buffer.before && !buffer.done_before) {
            buffer.done_before = true;
            startTime = time - buffer.before;
            elapsedTime = time - startTime;
        }
        if (elapsedTime <= buffer.during + buffer.before && buffer.done_before) {
            execute();
        }
        if (elapsedTime >= buffer.during + buffer.before && buffer.done_before) {
            startTime = time - (buffer.during + buffer.before);
            elapsedTime = time - startTime;
            buffer.done_during = true;
        }
        if (elapsedTime >= buffer.during + buffer.before + buffer.after && buffer.done_during) {
            buffer.done_after = true;
        }
        //std::cout << std::endl << buffer.done_before << std::endl << buffer.done_during << std::endl << buffer.done_after;
    }
    TimeOut(std::function<void()> exec = []() {}, DelayBuffer buff = DelayBuffer(), std::string id_ = "0") {
        exists = true;
        id = id_;
        startTime = ray::GetTime();
        execute = exec;
        buffer = buff;
    }
};
struct ContextBase {
    virtual void onKeyPressed(int k) {};
    virtual void onKeyDown(int k) {};
    virtual void onKeyUp(int k) {};
    virtual void onKeyReleased(int k) {};
    virtual void onMousePressed(MouseEvent m) {};
    virtual void onMouseDown(MouseEvent m) {};
    virtual void onMouseUp(MouseEvent m) {};
    virtual void onMouseReleased(MouseEvent m) {};
    virtual void draw() = 0;
    virtual ~ContextBase() {}
};
struct Context : ContextBase {
    int windowWidth;
    int windowHeight;
    int windowFps;
    int strokeWeight = 2;
    ray::Color strokeStyle = ray::BLACK;
    ray::Color fillStyle = {
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 255
    };
    ray::Color gradStyle = {
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 255
    };
    ray::Color backStyle = {
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 255
    };
    std::string windowTitle;
    void rect(int x, int y, int width, int height) {
        ray::DrawRectangleLines(x + (width * -0.5), y + (height * -0.5), width, height, strokeStyle);
    }
    void rect(ray::Rectangle rect) {
        ray::DrawRectangleLinesEx(rect, strokeWeight, strokeStyle);
    }
    void circle(int x, int y, int diameter) {
        ray::DrawCircleLines(x, y, diameter / 2, strokeStyle);
    }
    void fillRect(int x, int y, int width, int height, bool stroke = true) {
        ray::DrawRectangle(x + (width * -0.5), y + (height * -0.5), width, height, fillStyle);
        if (stroke) rect(x, y, width, height);
    }
    void fillRect(ray::Rectangle rec, bool stroke = true) {
        ray::DrawRectangleRec(rec, fillStyle);
        if (stroke) rect(rec);
    }
    void fillCircle(int x, int y, int diameter, bool stroke = true) {
        ray::DrawCircle(x, y, diameter / 2, fillStyle);
        if (stroke) circle(x, y, diameter);
    }
    void gradRect(int x, int y, int width, int height, bool stroke = true) {
        ray::DrawRectangleGradientH(x + (width * -0.5), y + (height * -0.5), width, height, fillStyle, gradStyle);
        if (stroke) rect(x, y, width, height);
    }
    void gradCircle(int x, int y, int diameter, bool stroke = true) {
        ray::DrawCircleGradient(x, y, diameter / 2, fillStyle, gradStyle);
        if (stroke) circle(x, y, diameter);
    }
    void text(std::string text, int x, int y, int size) {
        ray::DrawText(text.c_str(), x, y, size, strokeStyle);
    }
    void clearBack() {
        ray::ClearBackground(backStyle);
    }
    std::vector<ray::KeyboardKey> keyboard_keys_cache;
    std::unordered_map<ray::KeyboardKey, bool> keyboard_pressed;
    std::unordered_map<ray::MouseButton, bool> mouse_pressed;
    std::unordered_map<std::string, TimeOut> timeout_map;
    std::vector<std::string> timeouts;
    void run() {
        ray::InitWindow(windowWidth, windowHeight, windowTitle.c_str());
        ray::SetTargetFPS(windowFps);
        
        
        
        
        
        while (!ray::WindowShouldClose()) {
            std::vector<std::string> next_timeouts;
            //std::cout << std::endl << timeouts.size();
            int i = 0;
            for (auto& timeout : timeouts) {
                //std::cout << std::endl << timeout_map[timeout].buffer.before << std::endl << timeout_map[timeout].buffer.during << std::endl << timeout_map[timeout].buffer.after << std::endl;
                timeout_map[timeout].Update();
                if (!timeout_map[timeout].buffer.done_after) next_timeouts.push_back(timeout);
                else timeout_map.erase(timeout);
                i++;
            }
            timeouts = next_timeouts;
            while (true) {
                int k = ray::GetKeyPressed();
                ray::KeyboardKey i = static_cast <ray::KeyboardKey> (k);
                if (!i) break;
                onKeyPressed(k);
                keyboard_pressed[i] = true;
            }
            for (int k = ray::KEY_BACK; k != ray::KEY_KB_MENU; k++)
            {
                ray::KeyboardKey i = static_cast<ray::KeyboardKey>(k);
                if (keyboard_pressed[i]) {
                    if (!ray::IsKeyDown(i)) {
                        keyboard_pressed[i] = false;
                        onKeyReleased(k);
                    }
                    else onKeyDown(k);
                }
            }
            for (int m = 0; m < 7; m++) {
                ray::Vector2 position = ray::GetMousePosition();
                MouseEvent mEvent(position, m);
                ray::MouseButton i = static_cast <ray::MouseButton> (m);
                if (mouse_pressed[i]) {
                    if (!ray::IsMouseButtonDown(i)) {
                        mouse_pressed[i] = false;
                        onMouseReleased(mEvent);
                    }
                    else onMouseDown(mEvent);
                }
                else {
                    if (ray::IsMouseButtonDown(i)) {
                        mouse_pressed[i] = true;
                        onMousePressed(mEvent);
                    }
                    else onMouseUp(mEvent);
                }
            }
            ray::BeginDrawing();
            draw();
            ray::EndDrawing();
        }
        ray::CloseWindow();
    }
    int random(int min, int max) {
        return ray::GetRandomValue(min, max);
    }
    void setTimeout(std::function<void()> exec, DelayBuffer buffer, std::string id) {
        if (!timeout_map.count(id)) {
            timeout_map[id] = TimeOut(exec, buffer, id);
            timeouts.push_back(id);
        }
        //std::cout << timeouts.size();
    }
    Context(int width = 400, int height = 400, std::string title = "window", int fps = 60) {
        //timeout_exists = std::vector<bool>(100, false);
        windowWidth = width;
        windowHeight = height;
        windowTitle = title;
        windowFps = fps;
    }
    ~Context() {
        ray::CloseWindow();
    }
};
