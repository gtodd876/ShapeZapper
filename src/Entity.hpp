//
// Created by George Matthews on 10/26/24.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <tuple>
#include "Component.hpp"

class EntityManager;

using ComponentTuple = std::tuple<CTransform, CShape, CCollision, CInput, CScore, CLifeSpan>;

class Entity : public Component
{
    friend class EntityManager;

    ComponentTuple m_components;
    bool m_Active{true};
    std::string m_tag{"default"};
    size_t m_id{0};

    Entity(const size_t &id, std::string& tag) : m_tag(tag), m_id(id) {}

public:
    bool isActive() const { return m_Active; }
    void destroy() { m_Active = false; }
    size_t getId() const { return m_id; }
    const std::string &getTag() const { return m_tag; }

    template<typename T>
    bool has() const {
        return get<T>().exists;
    }
    template<typename T, typename... TArgs>
    T &add(TArgs &&...mArgs) {
        auto &component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.exists = true;
        return component;
    }
    template<typename T>
    T &get() {
        return std::get<T>(m_components);
    }
    template<typename T>
    const T &get() const {
        return std::get<T>(m_components);
    }
    template<typename T>
    void remove() {
        get<T>() = T();
    }
};


#endif // ENTITY_HPP
