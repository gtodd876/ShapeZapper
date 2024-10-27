//
// Created by George Matthews on 10/26/24.
//

#include "Game.h"

#include <fstream>
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

Game::Game(const std::string &config) { init(config); }

void Game::init(const std::string &path)
{
    std::ifstream file_in(path);
    std::string type{};
    if (!file_in.is_open())
    {
        std::cerr << "Error opening file!" << '\n';
    }
    while (file_in >> type)
    {
        if (type == "Window")
        {
            unsigned int width, height;
            file_in >> width >> height;
            m_window.create(sf::VideoMode(width, height), "Asteroids");
            unsigned int fl;
            file_in >> fl;
            m_window.setFramerateLimit(fl);
        }
        if (type == "Player")
        {
            file_in >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >>
                    m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >>
                    m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
        }
        if (type == "Enemy")
        {
            file_in >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >>
                    m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >>
                    m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
        }
        if (type == "Bullet")
        {
            file_in >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >>
                    m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >>
                    m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
        }
    }

    if (!ImGui::SFML::Init(m_window))
    {
        std::cerr << "Error initializing ImGui window!" << '\n';
        return;
    }

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    spawnPlayer();
}

std::shared_ptr<Entity> Game::player()
{
    auto &players = m_entities.getEntitiesByTag("player");

    // assert(players.size() == 1);

    return players.front();
}

void Game::run()
{
    // TODO: Add pause functionality here,
    // some systems should still run while paused, like rendering
    // some systems shouldn't - movement, input

    while (m_running)
    {
        m_entities.update();
        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        sEnemySpawner();
        sMovement();
        sCollision();
        sUserInput();
        sGUI();
        sRender();
        // increment current frame
        // may need ot be moved when pause is implemented
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused)
{
    // TODO:
}

void Game::spawnPlayer()
{
    // we create every entity by calling EntityManager.addEntity(tag)
    // this returns std::shared_ptr<Entity>, so we use auto to save typing
    auto entity = m_entities.addEntity("player");

    entity->add<CTransform>(
            Vec2f(static_cast<float>(m_window.getSize().x) / 2.0f, static_cast<float>(m_window.getSize().y) / 2.0f),
            Vec2f(0.0f, 0.0f), 0.0f);

    entity->add<CShape>(m_playerConfig.SR, 8, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
                        sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

    entity->add<CInput>();
}

void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity("enemy");
    // r = min + (rand() % (1 + max - min))
    // [-3, 3]
    // r = -3 + (rand() % (1 + 3 - -3)
    srand(time(nullptr));
    const unsigned int pos_x =
            m_enemyConfig.SR + (rand() % (1 + (m_window.getSize().x - m_enemyConfig.SR) - m_enemyConfig.SR));
    const unsigned int pos_y =
            m_enemyConfig.SR + (rand() % (1 + (m_window.getSize().y - m_enemyConfig.SR) - m_enemyConfig.SR));
    std::cout << "pos_x: " << pos_x << '\n';
    std::cout << "pos_y: " << pos_y << '\n';
    std::cout << "-m_enemyConfig.SMIN " << -m_enemyConfig.SMIN << '\n';
    std::cout << "-m_enemyConfig.SMAX " << -m_enemyConfig.SMAX << '\n';
    const int vel_x = rand() % static_cast<int>(m_enemyConfig.SMAX + m_enemyConfig.SMIN + 1) -
                      static_cast<int>(m_enemyConfig.SMIN);
    const int vel_y = rand() % static_cast<int>(m_enemyConfig.SMAX + m_enemyConfig.SMIN + 1) -
                      static_cast<int>(m_enemyConfig.SMIN);
    std::cout << "vel_x: " << vel_x << '\n';
    std::cout << "vel_y: " << vel_y << '\n';
    entity->add<CTransform>(Vec2f(static_cast<float>(pos_x), static_cast<float>(pos_y)),
                            Vec2f(static_cast<float>(vel_x), static_cast<float>(vel_y)), 2.0f);
    const unsigned int num_of_points = m_enemyConfig.VMIN + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN));

    const unsigned int outline_red = rand() % 256;
    const unsigned int outline_green = rand() % 256;
    const unsigned int outline_blue = rand() % 256;
    std::cout << "outline_red " << outline_red << " outline_green " << outline_green << " outline_blue "
              << outline_blue;
    entity->add<CShape>(m_enemyConfig.SR, num_of_points, sf::Color::Black,
                        sf::Color(outline_red, outline_green, outline_blue), m_enemyConfig.OT);

    // record when the most recent enemy was spawned
    // std::cout << "In spawn enemy: "
    //           << "m_lastEnemySpawnTime " << m_lastEnemySpawnTime << " m_currentFrame " << m_currentFrame <<
    //           std::endl;
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies at the location of the input enemy e
    // when we create the smaller enemy, we have to read the values of the
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &target)
{
    // TODO: implement the spawning of a bullet that travels toward target
    // bullet speed is given as a scalar speed
    // you must set the velocity by using the formula in notes
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    // TODO: implement your own special weapon
}

void Game::sMovement()
{
    // TODO: implement all entity movement in this function
    auto &player = m_entities.getEntitiesByTag("player").front();
    if (player->get<CInput>().up == true)
    {
        auto &transform = player->get<CTransform>();
        transform.pos.m_y = transform.pos.m_y - 3.0f;
        player->get<CShape>().circle.setPosition(transform.pos);
    }
    if (player->get<CInput>().down == true)
    {
        auto &transform = player->get<CTransform>();
        transform.pos.m_y = transform.pos.m_y + 3.0f;
        player->get<CShape>().circle.setPosition(transform.pos);
    }
    if (player->get<CInput>().left == true)
    {
        auto &transform = player->get<CTransform>();
        transform.pos.m_x = transform.pos.m_x - 3.0f;
        player->get<CShape>().circle.setPosition(transform.pos);
    }
    if (player->get<CInput>().right == true)
    {
        auto &transform = player->get<CTransform>();
        transform.pos.m_x = transform.pos.m_x + 3.0f;
        player->get<CShape>().circle.setPosition(transform.pos);
    }
    auto &enemies = m_entities.getEntitiesByTag("enemy");
    for (auto e: enemies)
    {
        auto &transform = e->get<CTransform>();
        transform.pos.m_x = transform.pos.m_x + transform.velocity.m_x;
        transform.pos.m_y = transform.pos.m_y + transform.velocity.m_y;
        e->get<CShape>().circle.setPosition(transform.pos);
    }
}

void Game::sLifeSpan()
{
    // TODO: implement all lifespan functionality
    //
    // for all entities
    // if entity has no lifespan component , skip it
    // if entity has > 0 lifespan then subtract 1
    // if it has lifespan and is alive
    // scale its alpha channel appropriately
    // if it has lifespan and its time is up, destroy the entity
}

void Game::sCollision()
{
    // TODO: implement all the proper collision between entities
    // be sure to use the collision radius, NOT the shape radius
    // sample
    // for (auto b: m_entities.getEntities("bullet"))
    // {
    //     for (auto e: m_entities.getEntities("enemy"))
    //     {
    //          // detect collision
    //     }
    //     for (auto e: m_entities.getEntities("small-enemy"))
    //     {
    //          // detect collision
    //     }
    // }
}

void Game::sEnemySpawner()
{
    const auto result = m_entities.getEntitiesByTag("enemy").size();
    if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI || result == 0)
    {
        spawnEnemy();
    }
}

void Game::sGUI()
{
    ImGui::Begin("Asteroids");
    ImGui::Text("Stuff goes here");
    ImGui::End();
}

void Game::sRender()
{
    // TODO: change all of the code below to render all the entities
    // sample drawing of the player entity that we have created
    m_window.clear(sf::Color::Black);

    // TODO: use for loop to loop over all the entities
    auto &p = m_entities.getEntitiesByTag("player");
    auto &player = p.front();
    player->get<CShape>().circle.setPosition(player->get<CTransform>().pos);
    player->get<CTransform>().angle += 1.0f;
    player->get<CShape>().circle.setRotation(player->get<CTransform>().angle);
    // TODO: render enemies
    auto &enemies = m_entities.getEntitiesByTag("enemy");
    for (auto &e: enemies)
    {
        e->get<CShape>().circle.setPosition(e->get<CTransform>().pos);
        e->get<CTransform>().angle += 1.0f;
        e->get<CShape>().circle.setRotation(e->get<CTransform>().angle);
        m_window.draw(e->get<CShape>().circle);
    }
    m_window.draw(player->get<CShape>().circle);

    // draw the UI last
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::sUserInput()
{
    // TODO: handle user input here
    // note that you should only be setting the player's input component variables here
    // you should not implement the player's movement logic here
    // the movement system will read the variables you set in the this function

    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(m_window, event);

        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().up = true;
                    break;
                }
                case sf::Keyboard::A:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().left = true;
                    break;
                }
                case sf::Keyboard::S:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().down = true;
                    break;
                }
                case sf::Keyboard::D:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().right = true;
                    break;
                }
                case sf::Keyboard::Escape:
                {
                    m_running = false;
                    m_window.close();
                    break;
                }
                default:
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().up = false;
                    break;
                }
                case sf::Keyboard::A:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().left = false;
                    break;
                }
                case sf::Keyboard::S:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().down = false;
                    break;
                }
                case sf::Keyboard::D:
                {
                    auto &player = m_entities.getEntitiesByTag("player").front();
                    player->get<CInput>().right = false;
                    break;
                }
                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (ImGui::GetIO().WantCaptureMouse)
            {
                continue;
            }
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "Left button was clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")"
                          << '\n';
                // call spawn bullet here
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "Right button was clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y
                          << ")" << '\n';
                // call spawn special weapon here
            }
        }
    }
}
