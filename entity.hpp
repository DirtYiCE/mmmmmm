#ifndef UUID_B5F58220_16E2_4B05_A7FD_A2E5F72B3FF5
#define UUID_B5F58220_16E2_4B05_A7FD_A2E5F72B3FF5
#pragma once

#include <map>
#include <vector>
#include "clone_ptr.hpp"

class Player;
class Entity
{
public:
    Entity() = default;
    Entity(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}
    virtual ~Entity() {}

    virtual void Simul(double dt) {}
    virtual void Render() = 0;
    virtual void Interact(Player& p) {}

    virtual ClonePtr<Entity> Clone() const = 0;

    int X() const { return x; }
    int Y() const { return y; }
    int Width() const { return width; }
    int Height() const { return height; }

protected:
    int x, y;
    int width, height;
};

template <typename T>
class EntityFrom : public Entity
{
public:
    using Entity::Entity;

    ClonePtr<Entity> Clone() const
    {
        return ClonePtr<Entity>(new T(static_cast<const T&>(*this)));
    }
};

class EntityFactory
{
public:
    EntityFactory(std::string name)
    {
        GetEMap()[std::move(name)] = this;
    }
    EntityFactory(const EntityFactory&) = delete;
    void operator=(const EntityFactory&) = delete;

    virtual ClonePtr<Entity> ICreate(std::vector<std::string>& args) = 0;

    static ClonePtr<Entity> Create(std::vector<std::string>& args)
    {
        return GetEMap().at(args.at(0))->ICreate(args);
    }

private:
    typedef std::map<std::string, EntityFactory*> EMap;
    static EMap& GetEMap()
    {
        static EMap emap;
        return emap;
    }
};

template <typename T>
class EntityFactoryImpl : public EntityFactory
{
public:
    using EntityFactory::EntityFactory;

    ClonePtr<Entity> ICreate(std::vector<std::string>& args) override
    {
        return ClonePtr<Entity>(new T(args));
    }
};

#endif
