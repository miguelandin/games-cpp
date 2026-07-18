#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "EntityManager.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

struct WindowConfig {
  unsigned int W, H;
  int FL;
  bool FS;
};

struct FontConfig {
  std::string F;
  int S, R, G, B;
};

struct PlayerConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
  float S, A, F;
};

struct EnemyConfig {
  int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI, M, T;
  float SMIN, SMAX;
};

struct BulletConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S, I;
};

class Game {
  sf::RenderWindow m_window;
  EntityManager m_entities;
  sf::Clock m_deltaClock;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  bool m_paused = false;
  WindowConfig m_wCf;
  FontConfig m_fCf;
  PlayerConfig m_pCf;
  EnemyConfig m_eCf;
  BulletConfig m_bCf;
  sf::Font m_font;
  sf::Text m_text;

  void init(const std::string &config); // read config file
  void setPaused(bool paused);          // pause the game

  /* SYSTEMS */
  void sMovement();
  void sUserInput();
  void sLifespan();
  void sRender();
  void sGUI();
  void sEnemySpawner();
  void sCollision();

  void spawnPlayer();
  void spawnEnemy(size_t points, const sf::Color &fill, const Vec2f &p,
                  const Vec2f &v, float angle);
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

  std::shared_ptr<Entity> player();

  bool isColliding(const Vec2f &p1, const Vec2f &p2, float r1, float r2);

public:
  Game(const std::string &config);
  void run();
};
