#include <stdarg.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <set>
#include <string>
namespace ray {
#include "raylib.h"
}

namespace db {
    int i = 1;
};


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
namespace Button {
    struct Style {
        ray::Color fillColor;
        ray::Color strokeColor;
        Style(ray::Color fill = { .r = 0, .g = 0, .b = 0, .a = 255 }, ray::Color stroke = { .r = 0, .g = 0, .b = 0, .a = 255 }) {
            this->fillColor = fill;
            this->strokeColor = stroke;
        }
    };
    struct Meta {
        std::string id;
        int intid;
        std::string label;
        Meta(std::string id = "none", int intid = 0, std::string label = "none") {
            this->label = label;
            this->id = id;
            this->intid = intid;
        }
    };
    struct RectButton {
        ray::Rectangle rect;
        Style style;
        Meta meta;
        double realWidth;
        double realHeight;
        double w;
        double h;
        double x;
        double y;
        
        bool checkCollision(ray::Vector2 pos, double realWidth, double realHeight) {
            ray::Rectangle rec = { (rect.x - (rect.width / 2)) * realWidth, (rect.y - (rect.height / 2))*realHeight, rect.width * realWidth, rect.height * realHeight};
            return ray::CheckCollisionPointRec(pos, rec);
        }
        RectButton(ray::Rectangle rect = ray::Rectangle(0,0,0,0), Meta meta = Meta(), Style style = Style()) {
            //this->realWidth = cont.realWidth;
            //this->realHeight = cont.realHeight;
            this->rect = { (float)(x), (float)(y), (float)w, (float)h };
            this->meta = meta;
            this->style = style;
            this->w = w;
            this->h = h;
            this->x = x;
            this->y = y;
        }
    };
};
namespace Animation {
    struct AnimationBuffer {
        
    };
    struct AnimationFrame {

    };
}
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
    virtual void onScreenButton(std::string id) {};
    virtual void onKeyDown(int k) {};
    virtual void onKeyUp(int k) {};
    virtual void onKeyReleased(int k) {};
    virtual void onMousePressed(MouseEvent m, bool screenButton = false, std::string buttonId = "none") {};
    virtual void onMouseDown(MouseEvent m, bool screenButton = false, std::string buttonId = "none") {};
    virtual void onMouseUp(MouseEvent m, bool screenButton = false, std::string buttonId = "none") {};
    virtual void onMouseReleased(MouseEvent m, bool screenButton = false, std::string buttonId = "none") {};
    virtual void draw() = 0;
    //virtual void setup() = 0;
    virtual ~ContextBase() {}
};
struct Context : ContextBase {
    double windowWidth;
    double windowHeight;
    double screenRatio;
    double pixelWidth = 640;
    double pixelHeight = 640;
    double realWidth;
    double realHeight;

    std::vector<Button::RectButton> buttons = {};/*
    std::vector<std::string> button_labels = {};
    std::vector <ray::Color> button_colors = {};
    std::vector <ray::Color> button_strokes = {};*/

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
        ray::DrawRectangleLines((x - (width / 2)) * realWidth, (y - (height / 2)) * realHeight, width * realWidth, height * realHeight, strokeStyle);
    }
    Button::RectButton newRectButton(double x = 0, double y = 0, double w = 0, double h = 0) {
        Button::RectButton button(x, y, w, h);
    }
    void rect(ray::Rectangle rec) {
        ray::Rectangle reca = { (rec.x - (rec.width / 2)) * realWidth, (rec.y - (rec.height / 2)) * realHeight, rec.width * realWidth, rec.height * realHeight };
        ray::DrawRectangleLinesEx(reca, strokeWeight, strokeStyle);
    }
    void circle(int x, int y, int diameter) {
        ray::DrawCircleLines(x * realWidth, y * realHeight, (diameter / 2)*screenRatio, strokeStyle);
    }
    void fillRect(int x, int y, int width, int height, bool stroke = true) {
        ray::DrawRectangle((x - (width / 2)) * realWidth, (y - (height / 2)) * realHeight, width * realWidth, height * realHeight, fillStyle);
        if (stroke) rect(x, y, width, height);
    }
    void fillRect(ray::Rectangle rec, bool stroke = true) {
        ray::Rectangle reca = { (rec.x - (rec.width / 2)) * realWidth, (rec.y - (rec.height / 2)) * realHeight, rec.width * realWidth, rec.height * realHeight };
        ray::DrawRectangleRec(reca, fillStyle);
        if (stroke) rect(rec);
    }
    void fillCircle(int x, int y, int diameter, bool stroke = true) {
        ray::DrawCircle(x * realWidth, y * realHeight, (diameter / 2)*screenRatio, fillStyle);
        if (stroke) circle(x, y, diameter);
    }
    void gradRect(int x, int y, int width, int height, bool stroke = true) {
        ray::DrawRectangleGradientH((x + (width * -0.5))*realWidth, y + (height * -0.5), width, height, fillStyle, gradStyle);
        if (stroke) rect(x, y, width, height);
    }
    void gradCircle(int x, int y, int diameter, bool stroke = true) {
        ray::DrawCircleGradient(x, y, diameter / 2, fillStyle, gradStyle);
        if (stroke) circle(x, y, diameter);
    }
    void text(std::string text, int x, int y, int size) {
        const char* str = text.c_str();
        ray::Vector2 wh = ray::MeasureTextEx(ray::GetFontDefault(), str, size, 1);
        ray::DrawText(text.c_str(), (x * realWidth) - (wh.x / 2), (y * realHeight)-(wh.y / 2), size, strokeStyle);
    }
    void line(double xa, double ya, double xb, double yb) {
        ray::DrawLine(xa*realWidth, ya*realHeight, xb*realWidth, yb*realHeight, strokeStyle);
    }
    void clearBack() {
        ray::ClearBackground(backStyle);
        std::cout << this->buttons.size();
        for (int i = 0; i < buttons.size(); i++) {
            fillStyle = buttons[i].style.fillColor;
            strokeStyle = buttons[i].style.strokeColor;
            fillRect(buttons[i].rect);
            //if (button_labels[i] == "CONST") {
            //    text("Add Path From Clipboard", buttons[i].x, buttons[i].y, 30);
            //}
            //else {
                //fillRect(buttons[i].rect);
            text(buttons[i].label, buttons[i].x, buttons[i].y, 30);
            // }
        }
    }
    std::vector<ray::KeyboardKey> keyboard_keys_cache;
    std::unordered_map<ray::KeyboardKey, bool> keyboard_pressed;
    std::unordered_map<ray::MouseButton, bool> mouse_pressed;
    std::unordered_map<std::string, TimeOut> timeout_map;
    std::vector<std::string> timeouts;
    bool forceClose = false;
    bool exists = false;
    bool shouldClose() {
        return ray::WindowShouldClose() || forceClose;
    }
    void run() {
        exists = true;
        ray::InitWindow(windowWidth, windowHeight, windowTitle.c_str());
        ray::SetTargetFPS(windowFps);
        //setup();

        std::cout << "\nscreenRatio: " + std::to_string(screenRatio) + " realWidth: " + std::to_string(realWidth) + " realHeight: " + std::to_string(realHeight) + " windowWidth: " + std::to_string(windowWidth) + " windowHeight: " + std::to_string(windowHeight) + " ";


        while (!shouldClose()) {
            std::vector<std::string> next_timeouts;
            int i = 0;
            for (auto& timeout : timeouts) {
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
                std::string buttonId = "none";
                bool onScreenButton = false;
                for (int i = 0; i < buttons.size(); i++) {
                    bool test = buttons[i].checkCollision(position, realWidth, realHeight);
                    std::cout << test;
                    if (test) {
                        onScreenButton = true;
                        buttonId = buttons[i].id;
                        break;
                    }
                }
                MouseEvent mEvent(position, m);
                ray::MouseButton i = static_cast <ray::MouseButton> (m);
                if (mouse_pressed[i]) {
                    if (!ray::IsMouseButtonDown(i)) {
                        mouse_pressed[i] = false;
                        onMouseReleased(mEvent, onScreenButton, buttonId);
                    }
                    else onMouseDown(mEvent, onScreenButton, buttonId);
                }
                else {
                    if (ray::IsMouseButtonDown(i)) {
                        mouse_pressed[i] = true;
                        onMousePressed(mEvent, onScreenButton, buttonId);
                    }
                    else onMouseUp(mEvent, onScreenButton, buttonId);
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
    void closeWindow() {
        forceClose = true;
    }
    void setTimeout(std::function<void()> exec, DelayBuffer buffer, std::string id) {
        if (!timeout_map.count(id)) {
            timeout_map[id] = TimeOut(exec, buffer, id);
            timeouts.push_back(id);
        }
    }
    Context(double width = 400, double height = 400, std::string title = "window", int fps = 60, std::vector<Button::RectButton> buttons = {}/*, std::vector<std::string> button_labels = {}, std::vector<ray::Color>button_colors = {}, std::vector<ray::Color>button_strokes = {}*/) {
        windowWidth = width;
        windowHeight = height;
        this->buttons = buttons;/*
        this->button_labels = button_labels;
        this->button_colors = button_colors;
        this->button_strokes = button_strokes;*/
        screenRatio = windowWidth / windowHeight;
        pixelWidth = 640;
        pixelHeight = 640;
        realWidth = windowWidth / (pixelWidth);
        realHeight = windowHeight / (pixelHeight);
        windowTitle = title;
        windowFps = fps;
    }
    ~Context() {
        if (!shouldClose() && exists) ray::CloseWindow();
    }
};
