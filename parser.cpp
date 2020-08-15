#include <stdio.h>
#include <libxml/parser.h>

#include "FOModule.h"

int ParseXml(xmlNode *node, void *data)
{
  xmlNode *cur_node = NULL;

  for (cur_node = node; cur_node; cur_node = cur_node->next)
  {
    if (cur_node->type == XML_ELEMENT_NODE)
    {

      printf(">%s\n", cur_node->name);

      if (xmlChildElementCount(cur_node) == 0)
      {
        /* JSON string object */
//        cur_jstr = json_object_new_string(xmlNodeGetContent(cur_node));
//        cur_jobj = json_object_new_object();
//        json_object_object_add(jobj, cur_node->name, cur_jstr);
      }
      else
      {
        /* JSON object */
//        cur_jobj = json_object_new_object();
//        json_object_object_add(jobj, cur_node->name, json_object_get(cur_jobj));
      }
    }

    ParseXml(cur_node->children, data);
  }

    return 0;
}

int ParseModuleConfig(char *name)
{
  int retVal = EXIT_SUCCESS;
  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;

  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION

  /*parse the file and get the DOM */
  doc = xmlReadFile(name, NULL, 0);

  if (doc == NULL)
  {
    printf("error: could not parse file %s\n", name);
    retVal = EXIT_FAILURE;
  }

  /*Get the root element node */
  root_element = xmlDocGetRootElement(doc);

  ParseXml(root_element, NULL);

  /*free the document */
  xmlFreeDoc(doc);

  /*
   *Free the global variables that may
   *have been allocated by the parser.
   */
    xmlCleanupParser();



    return retVal;
}

int main(int argc, char *argv[])
{
    ParseModuleConfig(argv[1]);

    return 0;
}
