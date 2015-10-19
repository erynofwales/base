/* Node.hh
 * vim: set tw=80:
 * Eryn Wells <eryn@erynwells.me>
 */
/**
 * An XML node.
 */

#pragma once

#include <map>
#include <vector>

#include "String.hh"

namespace erw {
namespace xml {

/** A node in an XML tree. */
struct Node
{
    typedef std::vector<Node> List;
    typedef std::map<String, String> AttributeMap;

    Node();
    Node(const String& name, const List& children);
    Node(const Node& other);
    ~Node();

    String name() const noexcept;
    List children() const noexcept;

protected:
    /** The name of the node. */
    String mName;
    /** Children of this node. The node owns its children. */
    List mChildren;
};

} /* namespace xml */
} /* namespace erw */
