#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Vec2.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

struct Window {
    int width{};
    int height{};
};

struct Font {
    std::string name{};
    int size{};
    int rgb[3]{};
};

enum ShapeType { Circle, Rectangle };

struct Shape {
    ShapeType type{};
    sf::CircleShape circle{};
    sf::RectangleShape rectangle{};
    bool isDrawn{true};
    std::string name{};
    float x{0};
    float y{0};
    float sx{0};
    float sy{0};
    float rgb[3]{0, 0, 0};
    float scale{1.0f};
    float radius{0};
    int width{0};
    int height{0};
};

struct Game {
    Window window{};
    Font font{};
    std::vector<Shape> shapes{};
};

Game initGameFromConfigFile(std::string &filename) {
    Game Game{};
    std::ifstream file_in(filename);
    std::string type{};
    std::string name{};

    while (file_in >> type) {
        if (type == "Window") {
            file_in >> Game.window.width >> Game.window.height;
        }
        if (type == "Font") {
            file_in >> Game.font.name >> Game.font.size >> Game.font.rgb[0] >> Game.font.rgb[1] >> Game.font.rgb[2];
        }
        if (type == "Circle") {
            Shape tempCircle;

            file_in >> tempCircle.name >> tempCircle.x >> tempCircle.y >> tempCircle.sx >> tempCircle.sy >>
                    tempCircle.rgb[0] >> tempCircle.rgb[1] >> tempCircle.rgb[2] >> tempCircle.radius;
            tempCircle.type = ShapeType::Circle;
            tempCircle.rgb[0] = tempCircle.rgb[0] / 255.0f;
            tempCircle.rgb[1] = tempCircle.rgb[1] / 255.0f;
            tempCircle.rgb[2] = tempCircle.rgb[2] / 255.0f;
            Game.shapes.push_back(tempCircle);
        }
        if (type == "Rectangle") {
            Shape tempRect;
            file_in >> tempRect.name >> tempRect.x >> tempRect.y >> tempRect.sx >> tempRect.sy >> tempRect.rgb[0] >>
                    tempRect.rgb[1] >> tempRect.rgb[2] >> tempRect.width >> tempRect.height;
            tempRect.type = ShapeType::Rectangle;
            tempRect.rgb[0] = tempRect.rgb[0] / 255.0f;
            tempRect.rgb[1] = tempRect.rgb[1] / 255.0f;
            tempRect.rgb[2] = tempRect.rgb[2] / 255.0f;
            Game.shapes.push_back(tempRect);
        }
    };

    return Game;
};

// Main entry point
int main(int argc, char *argv[]) {
    std::string config_file = "config.txt";
    auto *vec_a = new Vec2<int>(0, 0);
    auto *vec_b = new Vec2<int>(1, 0);
    bool my_result = *vec_a == *vec_b;
    std::cout << "my_result: " << my_result << std::endl;
    Game Game = initGameFromConfigFile(config_file);
    sf::RenderWindow window(sf::VideoMode(Game.window.width, Game.window.height), "Shape Zapper");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) {
        return -1;
    }
    for (auto &[type, circle, rectangle, isDrawn, name, x, y, sx, sy, rgb, scale, radius, width, height]: Game.shapes) {
        switch (type) {
            case (ShapeType::Circle): {
                circle.setPosition(x, y);
                circle.setFillColor(sf::Color(static_cast<int>(rgb[0] * 255), static_cast<int>(rgb[1] * 255),
                                              static_cast<int>(rgb[2] * 255)));
                circle.setRadius(radius);
                circle.setOrigin(circle.getRadius(), circle.getRadius());
                circle.setScale(scale, scale);
                break;
            }
            case (ShapeType::Rectangle): {
                rectangle.setPosition(x, y);
                rectangle.setFillColor(sf::Color(static_cast<int>(rgb[0] * 255), static_cast<int>(rgb[1] * 255),
                                                 static_cast<int>(rgb[2] * 255)));
                sf::Vector2f dimensions{static_cast<float>(width), static_cast<float>(height)};
                rectangle.setSize(dimensions);
                rectangle.setScale(1.0f, 1.0f);
                break;
            }
            default: {
                break;
            }
        }
    }
    sf::Clock deltaClock;
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    sf::Font myFont;
    if (!myFont.loadFromFile(Game.font.name)) {
        std::cerr << "Could not load font file" << '\n';
    }

    // int totalShapes{static_cast<int>(Game.shapes.size())};

    std::vector<const char *> shapeNames{};
    // int shapeIndex{0};
    for (auto &[type, circle, rectangle, isDrawn, name, x, y, sx, sy, rgb, scale, radius, width, height]: Game.shapes) {
        shapeNames.push_back(name.c_str());
    }

    int comboBox_index = 0;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed: {
                    if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                        window.close();
                    }
                    break;
                }
                default:
                    break;
            }
        }
        // Create IMGUI UI
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Controls");
        ImGui::Text("The Shape Maker 3000!");

        std::string comboLabel = "Shape##" + std::to_string(comboBox_index);
        ImGui::Combo(comboLabel.data(), &comboBox_index, shapeNames.data(), static_cast<int>(shapeNames.size()));

        std::string checkboxLabel = "Draw##" + std::to_string(comboBox_index);
        ImGui::Checkbox(checkboxLabel.data(), &Game.shapes[comboBox_index].isDrawn);

        std::string sliderFloatLabel = "Scale##" + std::to_string(comboBox_index);
        ImGui::SliderFloat(sliderFloatLabel.data(), &Game.shapes[comboBox_index].scale, 0.0f, 4.0f);

        std::string sliderFloat2Label = "Velocity##" + std::to_string(comboBox_index);
        float *velocity[2] = {&Game.shapes[comboBox_index].sx, &Game.shapes[comboBox_index].sy};
        ImGui::SliderFloat2("Velocity", *velocity, 8.0f, -8.0f);

        std::string colorEditLabel = "Color##" + std::to_string(comboBox_index);
        float *rgb[3] = {&Game.shapes[comboBox_index].rgb[0], &Game.shapes[comboBox_index].rgb[1],
                         &Game.shapes[comboBox_index].rgb[2]};
        ImGui::ColorEdit3(colorEditLabel.data(), *rgb);

        char *displayName = Game.shapes[comboBox_index].name.data();
        ImGui::InputText("Name", displayName, 255);

        if (ImGui::Button("Reset position")) {
            if (Game.shapes[comboBox_index].type == ShapeType::Circle) {
                Game.shapes[comboBox_index].circle.setPosition(0.0f + Game.shapes[comboBox_index].circle.getRadius(),
                                                               0.0f + Game.shapes[comboBox_index].circle.getRadius());
            }
        }
        ImGui::End();

        // Update shapes logic
        for (int i = 0; i < Game.shapes.size(); i++) {

            if (Game.shapes[i].type == ShapeType::Circle) {
                // update position
                Game.shapes[i].circle.setPosition(Game.shapes[i].circle.getPosition().x + Game.shapes[i].sx,
                                                  Game.shapes[i].circle.getPosition().y + Game.shapes[i].sy);
                // set color
                Game.shapes[i].circle.setFillColor(sf::Color(static_cast<int>(Game.shapes[i].rgb[0] * 255),
                                                             static_cast<int>(Game.shapes[i].rgb[1] * 255),
                                                             static_cast<int>(Game.shapes[i].rgb[2] * 255)));
                // set scaling
                Game.shapes[i].circle.setScale(Game.shapes[i].scale, Game.shapes[i].scale);
                // bounce logic
                if (Game.shapes[i].circle.getGlobalBounds().left < 0.0f ||
                    Game.shapes[i].circle.getGlobalBounds().left >
                            (static_cast<float>(Game.window.width) -
                             Game.shapes[i].circle.getRadius() * 2 * Game.shapes[i].scale)) {
                    Game.shapes[i].sx *= -1.0f;
                }
                if (Game.shapes[i].circle.getGlobalBounds().top < 0.0f ||
                    Game.shapes[i].circle.getGlobalBounds().top >
                            static_cast<float>(Game.window.height) -
                                    Game.shapes[i].circle.getRadius() * 2 * Game.shapes[i].scale) {
                    Game.shapes[i].sy *= -1.0f;
                }
            } else {
                // update position
                Game.shapes[i].rectangle.setPosition(Game.shapes[i].rectangle.getPosition().x + Game.shapes[i].sx,
                                                     Game.shapes[i].rectangle.getPosition().y + Game.shapes[i].sy);
                // set color
                Game.shapes[i].rectangle.setFillColor(sf::Color(static_cast<int>(Game.shapes[i].rgb[0] * 255),
                                                                static_cast<int>(Game.shapes[i].rgb[1] * 255),
                                                                static_cast<int>(Game.shapes[i].rgb[2] * 255)));
                // set scaling
                Game.shapes[i].rectangle.setScale(Game.shapes[i].scale, Game.shapes[i].scale);
                // bounce logic
                if (Game.shapes[i].rectangle.getGlobalBounds().left < 0.0f ||
                    Game.shapes[i].rectangle.getGlobalBounds().left >
                            (static_cast<float>(Game.window.width) -
                             Game.shapes[i].rectangle.getSize().x * Game.shapes[i].scale)) {
                    Game.shapes[i].sx *= -1.0f;
                }
                if (Game.shapes[i].rectangle.getGlobalBounds().top < 0.0f ||
                    Game.shapes[i].rectangle.getGlobalBounds().top >
                            static_cast<float>(Game.window.height) -
                                    Game.shapes[i].rectangle.getSize().y * Game.shapes[i].scale) {
                    Game.shapes[i].sy *= -1.0f;
                }
            }
        }

        window.clear(sf::Color(33, 33, 33));

        // draw everything here...
        for (auto &[type, circle, rectangle, isDrawn, name, x, y, sx, sy, rgb, scale, radius, width, height]:
             Game.shapes) {
            sf::Text shapeText(name, myFont, 16);
            shapeText.setStyle(sf::Text::Bold);
            if (isDrawn) {
                if (type == ShapeType::Circle) {
                    // Name logic
                    shapeText.setOrigin(shapeText.getGlobalBounds().getSize() / 2.f +
                                        shapeText.getLocalBounds().getPosition());

                    shapeText.setPosition(circle.getPosition().x, circle.getPosition().y);
                    // Draw shape to window
                    window.draw(circle);
                } else {
                    // Name logic
                    shapeText.setOrigin(
                            // x
                            shapeText.getGlobalBounds().getSize().x / 2.f + shapeText.getLocalBounds().getPosition().x,
                            // y
                            shapeText.getGlobalBounds().getSize().y / 2.f + shapeText.getLocalBounds().getPosition().y);

                    shapeText.setPosition(
                            // x
                            rectangle.getPosition().x + rectangle.getGlobalBounds().getSize().x / 2,
                            // y
                            rectangle.getPosition().y + rectangle.getGlobalBounds().getSize().y / 2);

                    // Draw shape to window
                    window.draw(rectangle);
                }
            }
            window.draw(shapeText);
        }
        ImGui::SFML::Render(window);
        // end the current frame
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}
