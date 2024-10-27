//
// Created by George Matthews on 10/26/24.
//

#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "Entity.hpp"

using EntityVec = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_entities_to_add;
    std::map<std::string, EntityVec> m_entityMap;
    size_t m_totalEntities{0};

    void removeDeadEntities(EntityVec &vec)
    {
        std::erase_if(vec, [](std::shared_ptr<Entity> e) { return !e->isActive(); });
    };

public:
    EntityManager() = default;
    void update()
    {
        for (auto &entity: m_entities_to_add)
        {
            // add them to the vector of all entities
            m_entities.push_back(entity);
            // add them to the vector inside the map, with the tag as the key
            m_entityMap[entity->m_tag].push_back(entity);
        }
        if (!m_entities.empty())
        {
            removeDeadEntities(m_entities);
        }
        // move the dead entities from the vector of all entities

        // also removing dead entities from the entity map
        // C++ 20 way of iterating through [key, value] pairs in a map
        for (auto &[tag, entityVec]: m_entityMap)
        {
            if (!entityVec.empty())
            {
                removeDeadEntities(entityVec);
            }
        }

        m_entities_to_add.clear();
    }

    std::shared_ptr<Entity> addEntity(const std::string &tag)
    {
        // create the entity shared pointer
        auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

        m_entities_to_add.push_back(entity);

        if (m_entityMap.find(tag) == m_entityMap.end())
        {
            m_entityMap[tag] = EntityVec();
        }
        m_entityMap[tag].push_back(entity);
        return entity;
    }

    EntityVec &getEntities() { return m_entities; }

    EntityVec &getEntitiesByTag(const std::string &tag) { return m_entityMap[tag]; }
};


#endif // ENTITYMANAGER_HPP
