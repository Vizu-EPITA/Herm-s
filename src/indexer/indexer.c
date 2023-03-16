#include <stdio.h>
#include <stdlib.h>
#include <libxml/HTMLparser.h>

void extract_visible_text(xmlNode *node)
{
    if (node == NULL) {
        return;
    }
    if (node->type == XML_TEXT_NODE) {
        xmlChar *content = xmlNodeGetContent(node);
        if (content != NULL) {
            printf("%s ", content);
            xmlFree(content);
        }
    } else if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar *)"script") != 0) {
        extract_visible_text(node->children);
    }
    extract_visible_text(node->next);
}

void extract_hyperlinks(xmlNode *node)
{
    if (node == NULL) {
        return;
    }
    if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar *)"a") == 0) {
        xmlChar *href = xmlGetProp(node, (const xmlChar *)"href");
        if (href != NULL) {
            printf("Link: %s\n", href);
            xmlFree(href);
        }
    }
    extract_hyperlinks(node->children);
    extract_hyperlinks(node->next);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    xmlDocPtr doc;
    htmlDocPtr html;
    xmlNodePtr root;

    // Parse the HTML file
    html = htmlReadFile(argv[1], NULL, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    if (html == NULL) {
        fprintf(stderr, "Error: could not parse file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Convert HTML document to generic XML document
    doc = (xmlDocPtr)html;
    if (doc == NULL) {
        fprintf(stderr, "Error: could not convert HTML document to XML document\n");
        return EXIT_FAILURE;
    }

    // Get the root element
    root = xmlDocGetRootElement(doc);

    // Extract visible text and hyperlinks
    extract_visible_text(root);
    printf("\n");
    extract_hyperlinks(root);

    // Free resources
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return EXIT_SUCCESS;
}
