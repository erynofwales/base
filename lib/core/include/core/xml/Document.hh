/* XMLDocument.hh
 * vim: set tw=80:
 * Eryn Wells <eryn@erynwells.me>
 */
/**
 * An XML document and related structures.
 */

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "core/File.hh"
#include "core/String.hh"


namespace erw {
namespace xml {

struct Document;


/** An XML document. */
struct Document
{
    /**
     * Constructor. Parse an XML document out of the given file. Doing so takes
     * ownership of the file.
     */
    Document(InFile&& file);

    /** Constructor. Parse an XML document from the given string. */
    Document(const String& string);

    ~Document();

    const Node& root() const noexcept;

protected:
    /** The root of the XML tree. The document owns its root. */
    Node mRoot;
};

} /* namespace xml */
} /* namespace erw */
