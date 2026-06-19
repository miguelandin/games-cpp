#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include <SFML/Graphics.hpp>

int main(int argc, char *argv[]) {

  // create new window of size w*h pixels
  const int wWidth = 1280;
  const int wHeight = 720;
  sf::RenderWindow window(sf::VideoMode({wWidth, wHeight}), "SFML works!");
  window.setFramerateLimit(60); // limit frame rate to 60 fps

  // initialize imgui and create a clock used for its internal timing
  if (!ImGui::SFML::Init(window)) { // imgui has to know where is drawing to,
                                    // function returns a boolean
    std::cout << "Could not initialize window\n";
    std::exit(-1);
  }

  sf::Clock deltaClock;

  // scale the imgui ui and text size by 2
  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

  // the imgui color {r, g, b} wheel requires floats from 0-1
  // sfml will require instead of unit8_t from 0-255
  // this is the only really annoying conversion between sflm and imgui
  float c[3] = {0.0f, 1.0f, 1.0f};

  // let's make a shape that will draw to the screen
  float circleRadius = 50.0f;
  int circleSegments = 32;
  float circleSpeedX = 1.0f;
  float circleSpeedY = 0.5f;
  bool drawCircle = true;
  bool drawText = true;

  // create the sfml circle shape based on our parameters
  sf::CircleShape circle(circleRadius, circleSegments);
  circle.setPosition({10.0f, 10.0f});

  // let's load a font so we can display some text
  sf::Font myFont;

  // attempt to load the font from a file
  if (!myFont.openFromFile("fonts/tech.ttf")) {
    // if we can't load the font, print an error to the error console and exit
    std::cout << "Could not load the font!\n";
    std::exit(-1);
  }

  // set up the text object that will be drawn to the screen
  sf::Text text(myFont, "Sample Text", 24);

  // position the top-left corner of the text so that the text aligns on the
  // bottom text character size is in pixels, so move the text up from the
  // bottom by its height
  text.setPosition({0, wHeight - (float)text.getCharacterSize()});

  // set up a character array to set the text
  char displayString[255] = "Sample Text";

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

    // draw the UI
    ImGui::Begin("Window title");
    ImGui::Text("Window text!");
    ImGui::Checkbox("Draw Circle", &drawCircle);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Text", &drawText);
    ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
    ImGui::SliderInt("Sides", &circleSegments, 3, 64);
    ImGui::ColorEdit3("Color circle", c);
    ImGui::InputText("Text", displayString, 255);

    if (ImGui::Button("Set Text"))
      text.setString(displayString);

    ImGui::SameLine();
    if (ImGui::Button("Reset Circle"))
      circle.setPosition({0, 0});

    ImGui::End();

    // set the circle properties, because they may have been updated with the ui
    circle.setPointCount(circleSegments);
    circle.setRadius(circleRadius);

    // imgui uses 0-1 float for colors, sfml uses 0-255 for colors
    // we must convert from ui floats to sfml Uint8
    circle.setFillColor(sf::Color(uint8_t(c[0] * 255), uint8_t(c[1] * 255),
                                  uint8_t(c[2] * 255)));

    // basic animation - move the circle each frame if it's still in frame
    circle.setPosition({circle.getPosition().x + circleSpeedX,
                        circle.getPosition().y + circleSpeedY});

    // basic rendering function calls
    window.clear(); // clear the window of anything previously drawn
    if (drawCircle)
      window.draw(circle);
    if (drawText)
      window.draw(text);

    ImGui::SFML::Render(window); // draw the ui last so it's on top
    window.display();            // call the window display function
  }

  return(0);
}
