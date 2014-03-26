// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <list>

#include <nix.hpp>

using namespace std;
using namespace nix;

Section::Section()
    : NamedEntity()
{
}


Section::Section(nullptr_t ptr)
    : NamedEntity()
{
}


Section::Section(const Section &other)
    : NamedEntity(other.impl())
{
}


Section::Section(const shared_ptr<base::ISection> &p_impl)
    : NamedEntity(p_impl)
{
}


//-----------------------------------------------------
// Methods concerning child sections
//-----------------------------------------------------


/*
 * Helper struct for {@link findSections}.
 */
struct SectionCont {
    SectionCont(Section s, size_t d = 0)
    : entity(s), depth(d)
    {}

    Section entity;
    size_t depth;
};
    

std::vector<Section> Section::findSections(std::function<bool(Section)> filter,
                                  size_t max_depth) const 
{
    std::vector<Section>  results;
    std::list<SectionCont> todo;

    todo.push_back(SectionCont(*this));

    while(todo.size() > 0) 
    {
        SectionCont current = todo.front();
        todo.pop_front();

        bool filter_ok = filter(current.entity);
        if (filter_ok) {
            results.push_back(current.entity);
        }

        if (current.depth < max_depth) {
            std::vector<Section> children = current.entity.sections();
            size_t next_depth = current.depth + 1;

            for (auto it = children.begin(); it != children.end(); ++it) {
                todo.push_back(SectionCont(*it, next_depth));
            }
        }
    }

    return results;
}
    

std::vector<Section> Section::findRelated(std::function<bool(Section)> filter) const
{
    std::vector<Section> results = findDownstream(filter);
    if(results.size() > 0) { //This checking of results can be removed if we decide not to include this in findSection
        for (vector<Section>::iterator it = results.begin(); it != results.end(); ++it) {
            if((*it).id() == id()) {
                results.erase(it, it+1);
                if (it == results.end())
                    break;
            }
        }
    }
    if (results.size() == 0) {
        results = findUpstream(filter);
    }
    if(results.size() > 0) //This checking of results can be removed if we decide not to include this in findSection
        for (vector<Section>::iterator it = results.begin(); it != results.end(); ++it) {
            if((*it).id() == id()) {
                results.erase(it, it+1);
                if (it == results.end())
                    break;
            }
        }
    if (results.size() == 0) {
        results = findSideways(filter, id());
    }
    return results;
}


//-----------------------------------------------------
// Methods for property access
//-----------------------------------------------------


vector<Property> Section::inheritedProperties() const {

    vector<Property> own = properties();

    if (link() == nullptr)
        return own;

    vector<Property> linked = link().properties();

    for (auto it_linked = linked.begin(); it_linked != linked.end(); ++it_linked) {

        bool not_own = true;
        for (auto it_own = own.begin(); it_own != own.end(); ++it_own) {
            if (it_linked->name() == it_own->name()) {
                not_own = false;
                break;
            }
        }

        if (not_own) {
            own.push_back(*it_linked);
        }

    }

    return own;
}


//------------------------------------------------------
// Operators and other functions
//------------------------------------------------------
size_t Section::tree_depth() const{
  vector<Section> children = sections();
  size_t depth = 0;
  if (children.size() > 0) {
      for (vector<Section>::iterator it = children.begin(); it != children.end(); ++it) {
          size_t temp = it->tree_depth();
          if(temp > depth)
              depth = temp;
      }
      depth += 1;
  }
  return depth;
}


vector<Section> Section::findDownstream(std::function<bool(Section)> filter) const{
    vector<Section> results;
    size_t max_depth = tree_depth();
    size_t actual_depth = 1;
    while (results.size() == 0 && actual_depth <= max_depth) {
        results = findSections(filter, actual_depth);
        actual_depth += 1;
    }
    return results;
}


vector<Section> Section::findUpstream(std::function<bool(Section)> filter) const{
    vector<Section> results;
    Section p = parent();

    if(p != nullptr) {
        results = p.findSections(filter,1);
        if(results.size() > 0) {
            return results;
        }
        return p.findUpstream(filter);
    }
    return results;
}


vector<Section> Section::findSideways(std::function<bool(Section)> filter, const string &caller_id) const{
    vector<Section> results;
    Section p = parent();
    if(p != nullptr) {
        results = p.findSections(filter,1);
        if(results.size() > 0) {
            for (vector<Section>::iterator it = results.begin(); it != results.end(); ++it) {
                if(it->id() == caller_id) {
                    results.erase(it, it+1);
                    if (it == results.end())
                        break;
                }
            }
            return results;
        }
        return p.findSideways(filter, caller_id);
    }
    return results;
}


std::ostream& nix::operator<<(ostream &out, const Section &ent) {
    out << "Section: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
