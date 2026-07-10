#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include "EntityManager.hpp"

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
  float S;
};

struct EnemyConfig {
  int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
  float SMIN, SMAX;
};

struct BulletConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S;
};



class Game {
  sf::RenderWindow m_window;
  EntityManager m_entities;
  sf::Font m_font;
  sf::Text m_text;
  PlayerConfig m_platerConfig;
  EnemyConfig m_enemyConfig;
  BulletConfig m_bulletConfig;
  sf::Clock m_deltaClock;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  bool m_paused = false;

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
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

  std::shared_ptr<Entity> player();

public:
WindowConfig wCf;
FontConfig fCf;
PlayerConfig pCf;
EnemyConfig eCf;
BulletConfig bCf;

sf::Font myFont;

  Game(const std::string &config);
  void run();
};
