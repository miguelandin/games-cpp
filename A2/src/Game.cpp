#include "Game.h"
#include "Components.hpp"
#include "Vec2.hpp"

#include <SFML/System/Angle.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
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
      iss >> m_wCf.W >> m_wCf.H >> m_wCf.FL >> m_wCf.FS;
    } else if (type == "Font") {
      iss >> m_fCf.F >> m_fCf.S >> m_fCf.R >> m_fCf.G >> m_fCf.B;
    } else if (type == "Player") {
      iss >> m_pCf.SR >> m_pCf.CR >> m_pCf.S >> m_pCf.A >> m_pCf.F >>
          m_pCf.FR >> m_pCf.FG >> m_pCf.FB >> m_pCf.OR >> m_pCf.OG >>
          m_pCf.OB >> m_pCf.OT >> m_pCf.V;
    } else if (type == "Enemy") {
      iss >> m_eCf.SR >> m_eCf.CR >> m_eCf.SMIN >> m_eCf.SMAX >> m_eCf.OR >>
          m_eCf.OG >> m_eCf.OB >> m_eCf.OT >> m_eCf.VMIN >> m_eCf.VMAX >>
          m_eCf.L >> m_eCf.SI >> m_eCf.M >> m_eCf.T;
    } else if (type == "Bullet") {
      iss >> m_bCf.SR >> m_bCf.CR >> m_bCf.S >> m_bCf.I >> m_bCf.FR >>
          m_bCf.FG >> m_bCf.FB >> m_bCf.OR >> m_bCf.OG >> m_bCf.OB >>
          m_bCf.OT >> m_bCf.V >> m_bCf.L;
    }
  }

  fin.close();

  m_window.create(sf::VideoMode({m_wCf.W, m_wCf.H}), "Assignment 2");
  m_window.setKeyRepeatEnabled(false);
  m_window.setFramerateLimit(m_wCf.FL);
  if (!ImGui::SFML::Init(m_window)) {
    std::cerr << "Window init exploded" << std::endl;
    std::exit(-1);
  }

  ImGui::GetIO().FontGlobalScale = m_fCf.S;
  if (!m_font.openFromFile(m_fCf.F)) {
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

    sUserInput();
    sEnemySpawner();
    sLifespan();
    sMovement();
    sShooting();
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
  static Vec2f spawnCoords = Vec2f((float)m_wCf.W / 2, (float)m_wCf.H / 2);
  if (player() != nullptr) {
    player()->get<CTransform>().pos = spawnCoords;
  } else {
    auto e = m_entities.addEntity("player");
    e->add<CTransform>(spawnCoords, Vec2f(), 0.0f);
    e->add<CShape>(m_pCf.SR, m_pCf.V, sf::Color(m_pCf.FR, m_pCf.FG, m_pCf.FB),
                   sf::Color(m_pCf.OR, m_pCf.OG, m_pCf.OB), m_pCf.OT);
    e->add<CInput>();
    e->add<CCollision>(m_pCf.CR);
    e->add<CWeapon>();
  }
}

// spawn an enemy at a random position
void Game::spawnEnemy(size_t points, const sf::Color &fill, const Vec2f &p,
                      const Vec2f &v, float angle) {
  auto e = m_entities.addEntity("enemy");
  e->add<CShape>(m_eCf.SR, points, fill,
                 sf::Color(m_eCf.OR, m_eCf.OG, m_eCf.OB), m_eCf.OT);
  e->add<CTransform>(p, v, angle);
  e->add<CCollision>(m_eCf.CR);
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

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &target) {
  auto t = entity->get<CTransform>();
  auto dsp = target - t.pos;
  auto vel = dsp.normalize() * m_bCf.S + t.velocity * m_bCf.I;
  auto bullet = m_entities.addEntity("bullet");
  bullet->add<CTransform>(t.pos, vel, t.angle);
  bullet->add<CShape>(m_bCf.SR, m_bCf.V,
                      sf::Color(m_bCf.FR, m_bCf.FG, m_bCf.FB),
                      sf::Color(m_bCf.OR, m_bCf.OG, m_bCf.OB), m_bCf.OT);
  bullet->add<CCollision>(m_bCf.CR);
  bullet->add<CLifespan>(m_bCf.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {
  // la entidad es quien genera la ulti en este caso el jugador
  // TODO: implement your own special weapon
}

void Game::sMovement() {
  if (player() != nullptr) {
    if (auto &t = player()->get<CTransform>(); t.exists) {
      if (auto &i = player()->get<CInput>(); i.exists) {
        Vec2f dir;
        if (i.w) {
          dir.y -= 1.0f;
        }
        if (i.s) {
          dir.y += 1.0f;
        }
        if (i.a) {
          dir.x -= 1.0f;
        }
        if (i.d) {
          dir.x += 1.0f;
        }
        if (dir != Vec2f()) {
          t.velocity += dir.normalize() * m_pCf.A;
        }
        t.velocity *= m_pCf.F;
        if (t.velocity.length() > m_pCf.S) {
          t.velocity = t.velocity.normalize() * m_pCf.S;
        }
        t.pos += t.velocity;
      }
    }
  }

  for (auto &e : m_entities.getEntities()) {
    if (auto &t = e->get<CTransform>(); t.exists) {
      if (e->id() != player()->id()) {
        t.pos += t.velocity;
      }
    }
  }
}

void Game::sLifespan() {
  for (auto &e : m_entities.getEntities()) {
    if (auto &l = e->get<CLifespan>(); l.exists) {
      if (l.remaining > 0) {
        l.remaining--;
        if (auto &s = e->get<CShape>(); s.exists) {
          auto f = s.circle.getFillColor();
          auto o = s.circle.getOutlineColor();
          f.a = (255 * l.remaining) / l.lifespan;
          o.a = f.a;
          s.circle.setFillColor(f);
          s.circle.setOutlineColor(o);
        }
      } else {
        e->destroy();
      }
    }
  }
}

bool Game::isColliding(const Vec2f &p1, const Vec2f &p2, float r1, float r2) {
  return p1.disSq(p2) < (r1 + r2) * (r1 + r2);
}

void Game::manageCollision(std::shared_ptr<Entity> entity) {
  if (auto &t = entity->get<CTransform>(); t.exists) {
    if (auto &c = entity->get<CCollision>(); c.exists) {
      if (t.pos.x - c.radius <= 0) {
        t.velocity.x *= -1;
        t.pos.x = 0 + c.radius;
      } else if (t.pos.x + c.radius >= m_wCf.W) {
        t.velocity.x *= -1;
        t.pos.x = m_wCf.W - c.radius;
      }
      if (t.pos.y - c.radius <= 0) {
        t.velocity.y *= -1;
        t.pos.y = 0 + c.radius;
      } else if (t.pos.y + c.radius >= m_wCf.H) {
        t.velocity.y *= -1;
        t.pos.y = m_wCf.H - c.radius;
      }
    }
  }
}

void Game::sCollision() {
  for (auto &e : m_entities.getEntities("enemy")) {
    for (auto &b : m_entities.getEntities("bullet")) {
      if (isColliding(e->get<CTransform>().pos, b->get<CTransform>().pos,
                      b->get<CCollision>().radius,
                      e->get<CCollision>().radius)) {
        b->destroy();
        e->destroy();
      }
    }
    if (isColliding(player()->get<CTransform>().pos, e->get<CTransform>().pos,
                    player()->get<CCollision>().radius,
                    e->get<CCollision>().radius)) {
      e->destroy();
      spawnPlayer();
    }
    manageCollision(e);
  }
  manageCollision(player());
}

void Game::sEnemySpawner() {
  static auto rng = [](auto min, auto max) {
    return min + (std::fmod(rand(), (1 + max - min)));
  };

  if (m_currentFrame - m_lastEnemySpawnTime > m_eCf.SI) {
    Vec2f pos;
    int t = 0;
    do {
      if (t > m_eCf.T) {
        return;
      }
      pos = Vec2f(rng(m_eCf.CR, m_wCf.W - m_eCf.CR),
                  rng(m_eCf.CR, m_wCf.H - m_eCf.CR));
      t++;
    } while (isColliding(player()->get<CTransform>().pos, pos,
                         m_pCf.CR * m_eCf.M, m_eCf.CR));
    auto speed = rng(m_eCf.SMIN, m_eCf.SMAX);
    auto points = rng(m_eCf.VMIN, m_eCf.VMAX);
    auto color = sf::Color(rng(0, 255), rng(0, 255), rng(0, 255));
    auto angle = rng(0, std::numbers::pi * 2);
    auto vel = Vec2f(angle) * speed;

    spawnEnemy(points, color, pos, vel, angle);
    m_lastEnemySpawnTime = m_currentFrame;
  }
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

  m_window.clear();

  for (auto &e : m_entities.getEntities()) {
    if (auto &s = e->get<CShape>(); s.exists) {
      if (auto &t = e->get<CTransform>(); t.exists) {
        s.circle.setPosition(t.pos);
        t.angle += 1.0f;
        s.circle.setRotation(sf::degrees(t.angle));
      }
      m_window.draw(s.circle);
    }
  }

  ImGui::SFML::Render(m_window);

  m_window.display();
}

void Game::sShooting() {
  auto p = player()->get<CTransform>().pos;
  if (auto &w = player()->get<CWeapon>(); w.exists) {
    if (m_currentFrame > w.lastFired + w.fireRate) {
      if (auto &i = player()->get<CInput>(); i.exists) {
        Vec2f dir;
        if (i.up) {
          dir.y -= 1.0f;
        }
        if (i.down) {
          dir.y += 1.0f;
        }
        if (i.left) {
          dir.x -= 1.0f;
        }
        if (i.right) {
          dir.x += 1.0f;
        }
        if (dir != Vec2f()) {
          spawnBullet(player(), dir + p);
          w.lastFired = m_currentFrame;
        }
      }
    }
  }

  for (auto &e : m_entities.getEntities("enemy")) {
    if (auto &w = e->get<CWeapon>(); w.exists) {
      if (m_currentFrame > w.lastFired + w.fireRate) {
        spawnBullet(e, p);
        w.lastFired = m_currentFrame;
      }
    }
  }
}

void Game::sUserInput() {
  static auto updateInput = [](CInput &input, sf::Keyboard::Scancode code,
                               bool isPressed) {
    switch (code) {
    case sf::Keyboard::Scancode::W:
      input.w = isPressed;
      break;
    case sf::Keyboard::Scancode::A:
      input.a = isPressed;
      break;
    case sf::Keyboard::Scancode::S:
      input.s = isPressed;
      break;
    case sf::Keyboard::Scancode::D:
      input.d = isPressed;
      break;
    case sf::Keyboard::Scancode::Up:
      input.up = isPressed;
      break;
    case sf::Keyboard::Scancode::Left:
      input.left = isPressed;
      break;
    case sf::Keyboard::Scancode::Down:
      input.down = isPressed;
      break;
    case sf::Keyboard::Scancode::Right:
      input.right = isPressed;
      break;
    case sf::Keyboard::Scancode::Space:
      input.special = isPressed;
      break;
    default:
      break;
    }
  };

  while (auto event = m_window.pollEvent()) {
    ImGui::SFML::ProcessEvent(m_window, *event);

    if (event->is<sf::Event::Closed>()) {
      std::exit(0);
    }

    if (auto p = player()) {
      auto &input = p->get<CInput>();
      if (input.exists) {

        if (const auto *pressed = event->getIf<sf::Event::KeyPressed>()) {
          updateInput(input, pressed->scancode, true);
        } else if (const auto *released =
                       event->getIf<sf::Event::KeyReleased>()) {
          updateInput(input, released->scancode, false);
        }
      }
    }
  }
}
