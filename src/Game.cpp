//
// Created by George Matthews on 10/26/24.
//

#include "Game.h"

#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

Game::Game(const std::string &config) { init(config); }

void Game::init(const std::string &path)
{
    // TODO: read in the config files
    // use the premade PlayerConfig, BulletConfig, EnemyConfig variabels
    // fin >> m_playerconfig.SR >> m_playerconfig.CR
    m_window.create(sf::VideoMode(1280, 720), 'Asteroids');
    m_window.setFramerateLimit(60);

    bool result = ImGui::SFML::Init(m_window);

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    spawnPlayer();
}

std::shared_ptr<Entity> Game::player()
{
    // auto &players = m_entities.get("player");

    // assert(players.size() == 1);

    // return players.front();
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
        SGUI();

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
    // TODO: finish adding all properties of the player with the correct values from the config file
    // we create every entity by calling EntityManager.addEntity(tag)
    // this returns std::shared_ptr<Entity>, so we use auto to save typing
    auto entity = m_entities.addEntity("player");

    entity->add<CTransform>(Vec2f(200.0f, 200.0f), Vec2f(1.0f, 1.0f), 0.0f);

    // radius 32, 8 sides, dark grey fill, red outline
    entity->add<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

    entity->add<CInput>();
}

void Game::spawnEnemy()
{
    // TODO: make sure the enemy is spawned properly with the m_enemyConfig vars
    // the enemy must be spawned completely within the bounds of the window

    // record when the most recent enemy was spawned
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
    // you should read the m_player->cInput component to determine if the player is moving

    // sample movement speed update
    auto &transform = player()->get<CTransform>();
    transform.pos = transform.pos + transform.velocity;
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
    // TODO: add enemy spawner code
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
    // player()->get<CShape>().circle.setPosition(player()->get<CTransform>().pos);
    //
    // player()->get<CTransform>().angle += 1.0f;
    // player()->get<CShape>().circle.setRotation(player()->get<CTransform>().angle);

    m_window.draw(player()->get<CShape>().circle);

    // draw the UI last
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::userInput()
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
                    std::cout << "W key was released" << '\n';
                    // todo: set player's input component "up" to true
                    break;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                    std::cout << "W key was released" << '\n';
                    // todo: set player's input component "up" to false
                    break;
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
