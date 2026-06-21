#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <array>
#include <cstring>
#include <iostream>
#include <memory>

// FONT
sf::Font myFont;

// imgui uses 0-1 float for colors, sfml uses 0-255 for colors
// we must convert from ui floats to sfml Uint8
sf::Color colorToSFML(const std::array<float, 3> &color) {
  return sf::Color(uint8_t(color[0] * 255), uint8_t(color[1] * 255),
                   uint8_t(color[2] * 255));
}

int main(int argc, char *argv[]) {
  // create new window of size w*h pixels
  const int wWidth = 1280;
  const int wHeight = 720;
  sf::RenderWindow window(sf::VideoMode({wWidth, wHeight}), "SFML works!");
  window.setFramerateLimit(60); // limit frame rate to 60 fps

  // scale the imgui ui and text size by 2
  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

  // initialize imgui and create a clock used for its internal timing
  if (!ImGui::SFML::Init(window)) { // imgui has to know where is drawing to,
                                    // function returns a boolean
    std::cout << "Could not initialize window\n";
    std::exit(-1);
  }

  sf::Clock deltaClock;

  // attempt to load the font from a file
  if (!myFont.openFromFile("fonts/tech.ttf")) {
    // if we can't load the font, print an error to the error console and exit
    std::cout << "Could not load the font!\n";
    std::exit(-1);
  }

  struct Shape {
    std::unique_ptr<sf::Shape> shape;
    sf::Vector2f pos;
    sf::Vector2f vel;
    std::array<float, 3> color;
    bool isDrawShape = true;
    bool isDrawText = true;
    char text[255]; // it must be C-String to work in ImGUI
    sf::Text displayText;
    int shapeType;

    Shape(std::unique_ptr<sf::Shape> shp, const sf::Vector2f &pos,
          const sf::Vector2f &vel, const std::array<float, 3> &color,
          const char *newText, int type)
        : shape(std::move(shp)), pos(pos), vel(vel), color(color),
          displayText(myFont, newText, 24), shapeType(type) {
      shape->setPosition(pos);
      shape->setFillColor(colorToSFML(color));
      std::strncpy(text, newText, sizeof(text) - 1);
      text[254] = '\0';
    }
  };

  struct Circle : Shape {
    float radius;
    int segments;

    Circle(float radius = 50.0f, int segments = 32,
           const sf::Vector2f &pos = {10.0f, 10.0f},
           const sf::Vector2f &vel = {1.0f, 0.5f},
           const std::array<float, 3> &color = {0.0f, 1.0f, 1.0f},
           const char *newText = "Circle")
        : Shape(std::make_unique<sf::CircleShape>(radius, segments), pos, vel,
                color, newText, 0),
          radius(radius), segments(segments) {}
  };

  struct Rect : Shape {
    float height;
    float width;

    Rect(float width = 25.0f, float height = 50.0f,
         const sf::Vector2f &pos = {10.0f, 10.0f},
         const sf::Vector2f &vel = {1.0f, 0.5f},
         const std::array<float, 3> &color = {1.0f, 0, 1.0f},
         const char *newText = "Rectangle")
        : Shape(
              std::make_unique<sf::RectangleShape>(sf::Vector2f{width, height}),
              pos, vel, color, newText, 1),
          height(height), width(width) {}
  };

  // instance shapes
  std::vector<std::unique_ptr<Shape>> shapes;
  shapes.push_back(std::make_unique<Circle>());
  shapes.push_back(std::make_unique<Rect>());

  int itemIndex = 0; // index for the ImGui::Combo

  // position the top-left corner of the text so that the text aligns on the
  // bottom text character size is in pixels, so move the text up from the
  // bottom by its height
  // text.setPosition({0, wHeight - (float)text.getCharacterSize()});

  // main loop - continues for each frame while window is open
  while (window.isOpen()) {
    // event handling
    while (auto event = window.pollEvent()) {
      // pass the event to imgui to be parsed
      ImGui::SFML::ProcessEvent(window, *event);

      // this event triggers the window is closed
      if (event->is<sf::Event::Closed>())
        window.close();

      // this event triggers when a key is pressed
      if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        // print the key that was pressed to the console
        std::cout << "Key pressed with code = " << int(keyPressed->scancode)
                  << std::endl;
      }
    }

    // update imgui for this frame with the time that the last frame took
    ImGui::SFML::Update(window, deltaClock.restart());

    if (ImGui::BeginCombo("Shapes", shapes[itemIndex]->text)) {
      for (int i = 0; i < shapes.size(); i++) {
        const bool isSelected = (itemIndex == i);
        if (ImGui::Selectable(shapes[i]->text, isSelected))
          itemIndex = i;
        if (isSelected)
          ImGui::SetItemDefaultFocus();
      }
    }

    // draw the UI
    ImGui::Begin("Window title");
    ImGui::Text("Window text!");
    ImGui::Checkbox("Draw Shape", &shapes[itemIndex]->isDrawShape);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Text", &shapes[itemIndex]->isDrawText);
    if (shapes[itemIndex]->shapeType == 0) {
      Circle *circle = static_cast<Circle *>(shapes[itemIndex].get());
      ImGui::SliderFloat("Radius", &circle->radius, 0.0f, 300.0f);
      ImGui::SliderInt("Sides", &circle->segments, 3, 64);
    } else if (shapes[itemIndex]->shapeType == 1) {
      Rect *rect = static_cast<Rect *>(shapes[itemIndex].get());
      ImGui::SliderFloat("Width", &rect->width, 0.0f, 300.0f);
      ImGui::SliderFloat("Height", &rect->height, 0.0f, 300.0f);
    }

    ImGui::ColorEdit3("Color Shape", shapes[itemIndex]->color.data());
    ImGui::InputText("Text", shapes[itemIndex]->text, 255);

    if (ImGui::Button("Set Text"))
      shapes[itemIndex]->displayText.setString(shapes[itemIndex]->text);

    ImGui::SameLine();
    if (ImGui::Button("Reset Shape"))
      shapes[itemIndex]->shape->setPosition({0, 0});

    ImGui::End();

    window.clear(); // clear the window of anything previously drawn

    // Logics
    for (auto &entity : shapes) {
      switch (entity->shapeType) {
      case 0: { // is a Circle
        Circle *circle = static_cast<Circle *>(entity.get());
        sf::CircleShape *circleShape =
            static_cast<sf::CircleShape *>(entity->shape.get());
        circleShape->setRadius(circle->radius);
        circleShape->setPointCount(circle->segments);
        break;
      }
      case 1: { // is a Rectangle
        Rect *rect = static_cast<Rect *>(entity.get());
        sf::RectangleShape *rectShape =
            static_cast<sf::RectangleShape *>(entity->shape.get());
        rectShape->setSize({rect->height, rect->width});
        break;
      }
      }

      entity->shape->setFillColor(colorToSFML(entity->color));

      // basic animation - move the shape each frame if it's still in frame
      entity->shape->setPosition(entity->pos);
      entity->pos += entity->vel;

      // basic rendering function calls
      if (entity->isDrawShape)
        window.draw(*entity->shape);
      if (entity->isDrawShape)
        window.draw(entity->displayText);
    }

    ImGui::SFML::Render(window); // draw the ui last so it's on top
    window.display();            // call the window display function
  }

  return (0);
}
