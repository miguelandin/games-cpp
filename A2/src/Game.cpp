#include "Game.h"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include <SFML/System/Angle.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Game::Game(const std::string &config) : m_text(m_font, "Default", 24) {
  init(config);
}

void Game::init(const std::string &path) {
  // TODO: read in config file here
  // use the premade PlayerConfig, EnemyConfig, BulletConfig variables
  // set up default  window parameters
  m_window.create(sf::VideoMode({1280, 720}), "Assignment 2");
  m_window.setKeyRepeatEnabled(false);
  m_window.setFramerateLimit(60);

  if (!ImGui::SFML::Init(m_window)) {
  }

  ImGui::GetStyle().ScaleAllSizes(2.0f);
  ImGui::GetIO().FontGlobalScale = 2.0f;

  spawnPlayer();
}

std::shared_ptr<Entity> Game::player() {
  return m_entities.getEntities("player").back();
}

void Game::run() {
  // TODO: add pause functionlay in here
  // some systems should function while paused (rendering)
  // some systems shouldn't (movement / input)
  while (true) {
    // update Entity manager
    m_entities.update();

    // required update call to imgui
    ImGui::SFML::Update(m_window, m_deltaClock.restart());

    sUserInput();
    sEnemySpawner();
    sMovement();
    sCollision();
    sGUI();
    sRender();

    // increment the current frame
    // may need to be moved when paused implemented
    m_currentFrame++;
  }
}

// respawn player in the middle of the screen
void Game::spawnPlayer() {
  // TODO: Finish adding all properties of the player with the correct values
  // from the  config
  auto e = m_entities.addEntity("player");
  e->add<CTransform>(Vec2f(200.0f, 200.0f), Vec2f(1.0f, 1.0f), 0.0f);
  e->add<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
  e->add<CInput>();
}

// spawn an enemy at a random position
void Game::spawnEnemy() {
  // TODO: make suer the enemy is spawned properly with the m_enemyConfig
  // variables the enemy must be spawned completely within the bounds of the
  // window
  //

  // record when the most recent enemy was spawned
  m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e) {
  // TODO: spawn small enemies at the location of the input enemy e

  // when we create the smaller enemy, we have to read the values of the
  // original enemy - spawn a number of small enemies equal to the vertices of
  // the original enemy
  // - set each small enemy to the same color as the original, half the size
  // - small enemies are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &target) {
  // TODO: implement the spawning of a bullet wich travels toward a target
  //    - bullet speed is given as a scalar speed
  //    - you must set the velocity by using formula in notes
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {
  // la entidad es quien genera la ulti en este caso el jugador
  // TODO: implement your own special weapon
}

void Game::sMovement() {
  // TODO: implement all entity movement in this function
  // you should read the m_player->cInput component to determine if the player
  // is moving
  auto &t =
      player()
          ->get<CTransform>(); // se usa por referencia en este caso por que te
                               // daría una copia el get, pero cuando hagamos un
                               // for que recorra el vector no hace falta porque
                               // ya son punteros lo que estás accediendo
  t.pos.x += t.velocity.x;
  t.pos.y += t.velocity.y;
}

void Game::sLifespan() {
  // TODO: implement all lifespawn functionality
  //
  // for all entities
  //   if entity has no lifespan component, skip it
  //   if entity has > 0 remaining lifespan, subtract 1
  //   if it has lifespan and is alive
  //       scale its alpha channel properly
  //    if it has lifespan and its time is up
  //        destroy the entity
  //
}

void Game::sCollision() {
  // TODO::implement all proper collision between entities
  //   be sure to use the collision radius, Not the shape radius

  // estas no son tdas las colisiones
  for (auto b : m_entities.getEntities("bullet")) {
    for (auto e : m_entities.getEntities("enemy")) {
      // do collision logic
    }

    for (auto e : m_entities.getEntities("bullet")) {
      // do collision logic
    }
  }
}

void Game::sEnemySpawner() {
  // TODO: code wich implements enemy spawning should go here
}

void Game::sGUI() {
  ImGui::Begin("Geometry Wars");
  ImGui::Text("Stuff Goes Here");
  ImGui::End();
}

void Game::sRender() {
  if (!m_window.isOpen()) {
    return;
  }

  // TODO: change the code below to draw ALL of the entities
  m_window.clear();

  // set the position of the shape based on the entity's transform->pos
  player()->get<CShape>().circle.setPosition(player()->get<CTransform>().pos);

  // set the rotation of the shape based on the entity's transform->angle
  player()->get<CTransform>().angle += 1.0f;
  player()->get<CShape>().circle.setRotation(sf::degrees(player()->get<CTransform>().angle);
    
//draw the entity's sf::CircleShape
m_window.draw(player()->get<CShape>().cricle);

//draw the ui last
ImGui::SFML::Render(m_window);

m_window.display();
}

void Game::sUserInput() {
  // TODO: handle user input here

  while (auto event = m_window.pollEvent()) {
    // pass the event to imgui to be parsed
    ImGui::SFML::ProcessEvent(m_window, *event);

    if (event->is<sf::Event::Closed>()) {
      std::exit(0);
    }

    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      std::cout << "KEY PRESSED = " << int(keyPressed->scancode) << std::endl;

      if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
        std::cout << "W Key pressed\n";
      }
    }

    if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
    }

    if (const auto *mousePressed =
            event->getIf<sf::Event::MouseButtonPressed>()) {
      Vec2f mpos(mousePressed->position);
      if (mousePressed->button == sf::Mouse::Button::Left) {
          //TODO: se puede llamar a la función de spawnBullet directamente aquí, lo mismo con la ulti
      }
    }
  }
}
