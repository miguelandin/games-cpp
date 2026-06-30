#pragma once
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

struct PlayerConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
  float S;
};

struct EnemyConfig {
  int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
  float SMIN, SMAX;
};

struct BulletConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
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

  void spanwPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

  std::shared_ptr<Entity> player();

public:
  Game(const std::string &config);
  void run();
};
