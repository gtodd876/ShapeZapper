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

    void removeDeadEntities(EntityVec &vec){
            // TODO: remove all dead entities from the input vector this is called by the update() function
    };

public:
    EntityManager() = default;
    void update()
    {
        // TODO: add entities from m_entitiesToAdd to the proper locations
        // add them to the vector of all entities
        // add them to the vector inside the map, with the tag as the key

        // move the dead entities from the vector of all entities
        removeDeadEntities(m_entities);

        // also removing dead entities from the entity map
        // C++ 20 way of iterating through [key, value] pairs in a map
        for (auto &[tag, entityVec]: m_entityMap)
        {
            removeDeadEntities(entityVec);
        }
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
