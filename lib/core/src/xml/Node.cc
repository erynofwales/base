/* XMLNode.cc
 * vim: set tw=80:
 * Eryn Wells <eryn@erynwells.me>
 */
/**
 * Implementation of a node in an XML tree.
 */

#include "core/XMLDocument.hh"

#include <libxml/parser.h>
#include <libxml/tree.h>


namespace erw {
namespace xml {

/*
 * Node::Node --
 */
Node::Node()
    : mName(),
      mChildren()
{ }


/*
 * Node::Node --
 */
Node::Node(const String& name,
           const List& children)
    : mName(name),
      mChildren(children)
{ }


/*
 * Node::Node --
 */
Node::Node(const Node& other)
    : mName(other.name),
      mChildren(other.children)
{ }


/*
 * Node::~Node --
 */
Node::~Node()
{ }

#pragma mark Properties

/*
 * Node::name --
 */
String
Node::name()
    const
{
    return mName;
}


/*
 * Node::children --
 */
List
Node::children()
    const
{
    return mChildren;
}

} /* namespace xml */
} /* namespace erw */
