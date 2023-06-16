#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <Arduino.h>
#include <Adafruit_GFX.h>

#include <memory>
#include <vector>

#include "Components/Component.hpp"
#include "Components/C_Drawable.hpp"
#include "Components/C_Position.hpp"

class Object
{
public:
    Object();
    // Awake is called when object created. Use to ensure
    // required components are present.
    void Awake();

    // Start is called after Awake method. Use to initialise variables.
    void Start();

    void Update(uint16_t deltaTime);
    void LateUpdate(uint16_t deltaTime);
    void Draw(GFXcanvas8 &canvas);

    template <typename T>
    std::shared_ptr<T> AddComponent()
    {
        // This ensures that we only try to add a class the derives
        // from Component. This is tested at compile time.
        static_assert(std::is_base_of<Component, T>::value,
                      "T must derive from Component");

        // Check that we don't already have a component of this type.
        for (auto &exisitingComponent : components)
        {
            // Currently we prevent adding the same component twice.
            // This may be something we will change in future.
            if (std::dynamic_pointer_cast<T>(exisitingComponent))
            {
                return std::dynamic_pointer_cast<T>(exisitingComponent); // 2
            }
        }

        // The object does not have this component so we create it and
        // add it to our list.
        std::shared_ptr<T> newComponent = std::make_shared<T>(this);
        components.push_back(newComponent);

        if (std::dynamic_pointer_cast<C_Drawable>(newComponent))
        {
            drawable = std::dynamic_pointer_cast<C_Drawable>(newComponent);
        }

        return newComponent;
    };

    template <typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args &&...args)
    {
        // This ensures that we only try to add a class the derives
        // from Component. This is tested at compile time.
        static_assert(std::is_base_of<Component, T>::value,
                      "T must derive from Component");

        // Check that we don't already have a component of this type.
        for (auto &existingComponent : components)
        {
            // Currently we prevent adding the same component twice.
            // This may be something we will change in future.
            if (std::dynamic_pointer_cast<T>(existingComponent))
            {
                return std::dynamic_pointer_cast<T>(existingComponent);
            }
        }

        // The object does not have this component so we create it and
        // add it to our list.
        std::shared_ptr<T> newComponent = std::make_shared<T>(this, std::forward<Args>(args)...);
        components.push_back(newComponent);

        if (std::dynamic_pointer_cast<C_Drawable>(newComponent))
        {
            drawable = std::dynamic_pointer_cast<C_Drawable>(newComponent);
        }

        return newComponent;
    };

    template <typename T>
    std::shared_ptr<T> GetComponent()
    {
        static_assert(std::is_base_of<Component, T>::value,
                      "T must derive from Component");

        // Check that we don't already have a component of this type.
        for (auto &exisitingComponent : components)
        {
            if (std::dynamic_pointer_cast<T>(exisitingComponent))
            {
                return std::dynamic_pointer_cast<T>(exisitingComponent);
            }
        }

        return nullptr;
    };
    std::shared_ptr<C_Drawable> GetDrawable();

    std::shared_ptr<C_Position> position;

    bool IsQueuedForRemoval();
    void QueueForRemoval();

protected:
    std::vector<std::shared_ptr<Component>> components;
    std::shared_ptr<C_Drawable> drawable;
    bool queuedForRemoval;
};

#endif /* OBJECT_HPP */