// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_NAMED_ENTITY_H
#define NIX_NAMED_ENTITY_H

#include <nix/base/INamedEntity.hpp>
#include <nix/base/Entity.hpp>

#include <memory>
#include <string>

namespace nix {
namespace base {

/**
 * @brief Base class for entities with a {@link name}, {@link type} and a
 * {@link definition}
 *
 * In addition to the properties defined by {@link nix::base::Entity} most entities of the
 * NIX data model further provide a {@link name}, {@link type} and a {@link definition}.
 * The {@link name} of an entity serves as a human readable identifier. It is not obliged
 * to be unique. However it is strongly recommended to use unique name inside one specific
 * {@link nix::Block}. The property {@link type} is used in order to allow the specification
 * of additional semantic meaning for an entity and therefore can introduce
 * domain-specificity into the quite generic data model. The {@link definition} is an optional
 * property that allows the user to add a freely assignable textual definition to the entity.
 */
template<typename T>
class NamedEntity : public Entity<T> {

public:

    /**
     * @brief Constructor that creates a null entity.
     */
    NamedEntity()
        : Entity<T>()
    {
    }

    /**
     * @brief Constructor that creates a new entity from a shared pointer to
     * an implementation instance.
     */
    NamedEntity(const std::shared_ptr<T> &p_impl)
        : Entity<T>(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a
     * shared pointer to an implementation instance.
     */
    NamedEntity(std::shared_ptr<T> &&ptr)
        : Entity<T>(std::move(ptr))
    {
    }

    /**
     * @brief Setter for the type of the entity.
     *
     * @param type The type to set.
     */
    void type(const std::string &type) {
        Entity<T>::backend()->type(type);
    }

    /**
     * @brief Getter for the type of the entity.
     *
     * The property {@link type} is used in order to allow the specification
     * of additional semantic meaning for an entity and therefore can introduce
     * domain-specificity into the quite generic data model.
     *
     * @return The current type.
     */
    std::string type() const {
        return Entity<T>::backend()->type();
    }

    /**
     * @brief Getter for the name of the entity.
     *
     * The {@link name} of an entity serves as a human readable identifier. It is not obliged
     * to be unique. However it is strongly recommended to use unique name inside one specific
     * {@link nix::Block}.
     *
     * @return string The name of the entity.
     */
    std::string name() const {
        return Entity<T>::backend()->name();
    }

    /**
     * @brief Setter for the definition of the entity.
     *
     * @param definition The definition of the entity.
     */
    void definition(const std::string &definition) {
        Entity<T>::backend()->definition(definition);
    }

    /**
     * @brief Getter for the definition of the entity.
     *
     * The {@link definition} is an optional property that allows the user to add
     * a freely assignable textual definition to the entity.
     *
     * @return The definition of the entity.
     */
    boost::optional<std::string> definition() const {
        return Entity<T>::backend()->definition();
    }

    /**
     * @brief Deleter for the definition of the entity.
     */
    void definition(const none_t t)
    {
        Entity<T>::backend()->definition(t);
    }

    /**
     * @brief Compare two named entities.
     *
     * @param other The entity to compare with.
     *
     * @return > 0 if the entity is larger that other, 0 if both are
     * equal, and < 0 otherwise.
     */
    int compare(NamedEntity &other) const {
        return Entity<T>::backend()->compare(other.impl());
    }

    /**
     * @brief Destructor
     */
    virtual ~NamedEntity() {}

};

template<typename Entity>
struct NIXAPI has_name {
#ifndef _WIN32
    typedef char have;
    typedef long nope;

        template<typename C> static have check(decltype(&C::name));
        template<typename C> static nope check(...);

    public:
        enum {
            value = sizeof(check<Entity>(0)) == sizeof(have)
        };

#else
    __if_exists(Entity::name) {
        static const bool value = true;
    }
    __if_not_exists(Entity::name) {
        static const bool value = false;
    }
#endif
};

template<typename Entity>
typename std::enable_if<has_name<Entity>::value,
               boost::optional<std::string>>::type
getEntityName(const Entity &e) {
    return boost::make_optional(e.name());
}

template<typename Entity>
typename std::enable_if<! has_name<Entity>::value,
                        boost::optional<std::string>>::type
getEntityName(const Entity &e) {
    return nix::none;
}

} // namespace base

/**
 * @brief Get the name of an entity.
 *
 * @param e The entity to get the name of.
 *
 * @return the name of the entity, if it is a {@link nix::base::Entity} to begin
 * with or {@link nix::none}.
 */
template<typename Entity>
boost::optional<std::string> getEntityName(const Entity &e) {
    return base::getEntityName<Entity>(e);
}


} // namespace nix
#endif // NIX_NAMED_ENTITY_H
