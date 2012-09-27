#include "config.h"

#include "DomDocument.h"

DOM_DOCUMENT *domDocumentNew()
{
    DOM_DOCUMENT *ret = domNodeNew(DOCUMENT_NODE, (DOMString)"#document", NULL);

    return ret;
}

void domDocumentDestroy(DOM_DOCUMENT *doc)
{
    if (doc)
        domNodeDestroy(doc);
}
