/* XMLParser.cc
 * vim: set tw=80:
 * Eryn Wells <eryn@erynwells.me>
 */
/**
 * Implementation of an XML parser.
 */

#include <cstddef>
#include <mutex>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "XMLParser.hh"



namespace {

/*
 * initLibrary --
 */
void
initLibrary()
{
    static std::once_flag once;
    std::call_once(once, []() {
        xmlInitParser();
    });
}


/*
 * parseFile --
 */
XMLNode
parseFile(File&& file)
{
    const size_t kInitialBufferSize = 16;
    const size_t kBufferSize = 1024;

    static_assert(kInitialBufferSize < kBufferSize,
                  "XML parser initial buffer size must be smaller than the "
                  "total buffer size");

    initLibrary();

    char buffer[kBufferSize];
    ssize_t bytesRead = 0;

    bytesRead = file.read(buffer, kInitialBufferSize);
    xmlParserCtxtPtr context = xmlCreatePushParserCtxt(nullptr, nullptr,
                                                       buffer, bytesRead,
                                                       file.path().c_str());
    if (!context) {
        // TODO: Throw an appropriate error...
        throw 42;
    }

    /*
     * Read chunks until we're done. Once all data has been read, indicate that
     * the parser should terminate by calling xmlParseChunk() with a last
     * argument of 1 rather than 0.
     */
    while ((bytesRead = file.read(buffer, kBufferSize)) > 0) {
        xmlParseChunk(context, buffer, bytesRead, 0);
    }
    xmlParseChunk(context, buffer, 0, 1);

    bool succeeded = bool(context->wellFormed);
    xmlDocPtr document = context->myDoc;

    xmlFreeParserCtxt(context);

    return succeeded ? XMLDocument::UCPtr(new XML2Document(document)) : nullptr;
}

}


namespace erw {

struct XML2Node
    : public XMLNode
{
    XML2Node(xmlNodePtr node);

    virtual ~XML2Node();

    virtual String name() const noexcept override;
    virtual String content() const noexcept override;
    virtual XMLNode::AttributeMap attributes() const noexcept override;

private:
    xmlNodePtr mNode;

    /** Make a list of children of the node, excluding text nodes. */
    XMLNode::List childrenOfXML2Node(xmlNodePtr node) const noexcept;
};


/** An XML parser that uses libxml2. */
struct XML2Document
    : public XMLDocument
{
    /**
     * Initialize the xml2 library, if needed. This function may be called more
     * than once with no adverse affects.
     */
    static void initLibrary();

    /**
     * Parse a file into a libxml2 document object. Note: because of move
     * semantics related to the file UPtr, the file will be closed after this
     * method completes.
     *
     * @param [in] file     The file to parse.
     * @return A libxml2 document, or nullptr if the parse fails.
     */
    static XMLDocument::UCPtr parseFile(File::UPtr file);

    static XMLDocument::UCPtr parseString(const String& string);

    XML2Document(xmlDocPtr document);

    virtual ~XML2Document();

    virtual XMLNode::WCPtr root() const noexcept override;

private:
    xmlDocPtr mDocument;
};

#pragma mark - erw::XMLDocument

/* static */ XMLDocument::UCPtr
XMLDocument::parseFile(File::UPtr file)
{
    return XML2Document::parseFile(std::move(file));
}


/* static */ XMLDocument::UCPtr
XMLDocument::parseString(const String& string)
{
    return XML2Document::parseString(string);
}


XMLDocument(InFile&& file)
    : mRoot()
{ }


XMLDocument::~XMLDocument()
{ }

#pragma mark - erw::XML2Document



/* static */ XMLDocument::UCPtr
XML2Document::parseString(const String& string)
{
    initLibrary();
    xmlDocPtr document = xmlReadMemory(string.c_str(), string.size(), "memory.xml", NULL, 0);
    return document ? XMLDocument::UCPtr(new XML2Document(document)) : nullptr;
}


XML2Document::XML2Document(xmlDocPtr document)
    : XMLDocument(XMLNode::Ptr(new XML2Node(xmlDocGetRootElement(document)))),
      mDocument(document)
{ }


XML2Document::~XML2Document()
{
    if (mDocument) {
        xmlFreeDoc(mDocument);
        mDocument = nullptr;
    }
}


XMLNode::WCPtr
XML2Document::root()
    const noexcept
{
    return mRoot;
}

#pragma mark - XMLNode

XMLNode::~XMLNode()
{ }


XMLNode::XMLNode(XMLNode::List&& children)
    : mChildren(children)
{ }


XMLNode::WCList
XMLNode::children()
    const noexcept
{
    WCList weakChildren;
    for (Ptr child : mChildren) {
        weakChildren.push_back(WCPtr(child));
    }
    return weakChildren;
}

#pragma mark - XML2Node

XML2Node::XML2Node(xmlNodePtr node)
    : XMLNode(childrenOfXML2Node(node)),
      mNode(node)
{ }


XML2Node::~XML2Node()
{ }


String
XML2Node::name()
    const noexcept
{
    return (const char *)mNode->name;
}


String
XML2Node::content()
    const noexcept
{
    xmlChar *content = xmlNodeGetContent(mNode);
    String contentString((const char *)content);
    xmlFree(content);
    return contentString;
}


XMLNode::AttributeMap
XML2Node::attributes()
    const noexcept
{
    AttributeMap attrs;
    for (xmlAttrPtr attr = mNode->properties; attr && attr->name && attr->children; attr = attr->next) {
        xmlChar *value = xmlNodeListGetString(mNode->doc, attr->children, 1);
        attrs[(const char *)attr->name] = (const char *)value;
        xmlFree(value);
    }
    return attrs;
}


XMLNode::List
XML2Node::childrenOfXML2Node(xmlNodePtr node)
    const noexcept
{
    XMLNode::List children;
    for (xmlNodePtr c = node->children; c != nullptr; c = c->next) {
        if (c->type != XML_ELEMENT_NODE) {
            continue;
        }
        children.push_back(XMLNode::Ptr(new XML2Node(c)));
    }
    return children;
}

} /* namespace erw */
