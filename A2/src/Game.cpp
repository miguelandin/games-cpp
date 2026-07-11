#include "Game.h"

#include <SFML/Window/Keyboard.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

Game::Game(const std::string &config) : m_text(m_font, "Default", 24) {
  init(config);
}

void Game::init(const std::string &path) {
  std::ifstream fin(path);
  if (!fin.is_open()) {
    std::cerr << "Config file not found" << std::endl;
    std::exit(-1);
  }

  std::string line;
  while (std::getline(fin, line)) {
    if (line.empty() || line.starts_with('#')) {
      continue;
    }
    std::istringstream iss(line);
    std::string type;

    iss >> type;

    if (type == "Window") {
      iss >> wCf.W >> wCf.H >> wCf.FL >> wCf.FS;
    } else if (type == "Font") {
      iss >> fCf.F >> fCf.S >> fCf.R >> fCf.G >> fCf.B;
    } else if (type == "Player") {
      iss >> pCf.SR >> pCf.CR >> pCf.S >> pCf.FR >> pCf.FG >> pCf.FB >>
          pCf.OR >> pCf.OG >> pCf.OB >> pCf.OT >> pCf.V;
    } else if (type == "Enemy") {
      iss >> eCf.SR >> eCf.CR >> eCf.SMIN >> eCf.SMAX >> eCf.OR >> eCf.OG >>
          eCf.OB >> eCf.OT >> eCf.VMIN >> eCf.VMAX >> eCf.L >> eCf.SI;
    } else if (type == "Bullet") {
      iss >> bCf.SR >> bCf.CR >> bCf.S >> bCf.FR >> bCf.FG >> bCf.FB >>
          bCf.OR >> bCf.OG >> bCf.OB >> bCf.OT >> bCf.V >> bCf.L;
    }
  }

  fin.close();

  m_window.create(sf::VideoMode({wCf.W, wCf.H}), "Assignment 2");
  m_window.setKeyRepeatEnabled(false);
  m_window.setFramerateLimit(wCf.FL);
  if (!ImGui::SFML::Init(m_window)) {
    std::cerr << "Window init exploded" << std::endl;
    std::exit(-1);
  }

  ImGui::GetIO().FontGlobalScale = fCf.S;
  if (!myFont.openFromFile(fCf.F)) {
    std::cerr << "Font file not found" << std::endl;
    std::exit(-1);
  }

  spawnPlayer();
}

std::shared_ptr<Entity> Game::player() {
  auto &p = m_entities.getEntities("player");
  if (!p.empty()) {
    return p.back();
  }
  return nullptr;
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

    std::cout << "P1" << std::endl;
    sUserInput();
    std::cout << "P2" << std::endl;
    sEnemySpawner();
    std::cout << "P3" << std::endl;
    sMovement();
    std::cout << "P4" << std::endl;
    sCollision();
    std::cout << "P5" << std::endl;
    sGUI();
    std::cout << "P6" << std::endl;
    sRender();
    std::cout << "P7" << std::endl;

    // increment the current frame
    // may need to be moved when paused implemented
    m_currentFrame++;
  }
}

// respawn player in the middle of the screen
void Game::spawnPlayer() {
  std::cout << "P0" << std::endl;
  static Vec2f spawnCoords = Vec2f((float)wCf.W / 2, (float)wCf.H / 2);
  if (player() != nullptr) {
    std::cout << "P-1" << std::endl;
    player()->get<CTransform>().pos = spawnCoords;
  } else {
    auto e = m_entities.addEntity("player");
    e->add<CTransform>(spawnCoords, Vec2f(), 0.0f);
    e->add<CShape>(pCf.SR, pCf.V, sf::Color(pCf.FR, pCf.FG, pCf.FB),
                   sf::Color(pCf.OR, pCf.OG, pCf.OB), pCf.OT);
    e->add<CInput>();
    e->add<CCollision>(pCf.CR);
  }
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
  if (player() != nullptr) {
    if (auto &t = player()->get<CTransform>(); t.exists) {
      if (auto &i = player()->get<CInput>(); i.exists) {
        if (i.up) {
          t.pos.y -= pCf.S;
        }
        if (i.down) {
          t.pos.y += pCf.S;
        }
        if (i.left) {
          t.pos.x -= pCf.S;
        }
        if (i.right) {
          t.pos.x += pCf.S;
        }
      }
    }
  }
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
  player()->get<CShape>().circle.setRotation(
      sf::degrees(player()->get<CTransform>().angle));

  // draw the entity's sf::CircleShape
  m_window.draw(player()->get<CShape>().circle);

  // draw the ui last
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

    if (auto p = player()) {
      auto &input = p->get<CInput>();
      if (input.exists) {
        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
          if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
            input.up = true;
          }
          if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
            input.down = true;
          }
          if (keyPressed->scancode == sf::Keyboard::Scancode::A) {
            input.left = true;
          }
          if (keyPressed->scancode == sf::Keyboard::Scancode::D) {
            input.right = true;
          }
        }
        if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
          if (keyReleased->scancode == sf::Keyboard::Scancode::W) {
            input.up = false;
          }
          if (keyReleased->scancode == sf::Keyboard::Scancode::S) {
            input.down = false;
          }
          if (keyReleased->scancode == sf::Keyboard::Scancode::A) {
            input.left = false;
          }
          if (keyReleased->scancode == sf::Keyboard::Scancode::D) {
            input.right = false;
          }
        }

        if (const auto *mousePressed =
                event->getIf<sf::Event::MouseButtonPressed>()) {
          Vec2f mpos(mousePressed->position);
          if (mousePressed->button == sf::Mouse::Button::Left) {
            // TODO: se puede llamar a la función de spawnBullet directamente
            // aquí, lo mismo con la ulti
          }
        }
      }
    }
  }
}
