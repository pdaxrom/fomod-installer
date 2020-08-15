#include <string.h>
#include <iostream>
#include <libxml/parser.h>

#include "FOModule.h"

using namespace std;

bool GetBooleanFromString(char *s, int f) {
	if (s) {
		string str(s);

		if (f) {
			free(s);
		}

		if (str.compare("true")) {
			return true;
		}
	}
	return false;
}

int GetIntegerFromString(char *s, int f) {
	int val = 0;
	if (s) {
		val = stoi(s);

		if (f) {
			free(s);
		}
	}
	return val;
}

void ParseRequiredInstallFiles(FOModule *fomod, xmlNode *node) {
	xmlNode *cur_node = NULL;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		printf(">>%s\n", cur_node->name);

		string name((char *)cur_node->name);

		if (!name.compare("folder") ||
				!name.compare("file")) {
			FileType file;
			file.source = (char *)xmlGetProp(cur_node, (const xmlChar*)"source");
			file.destination = (char *)xmlGetProp(cur_node, (const xmlChar*)"destination");
			file.alwaysInstall = GetBooleanFromString((char *) xmlGetProp(cur_node, (const xmlChar *)"alwaysInstall"), 1);
			file.installIfUsable = GetBooleanFromString((char *) xmlGetProp(cur_node, (const xmlChar *)"installIfUsable"), 1);
			file.priority = GetIntegerFromString((char *) xmlGetProp(cur_node, (const xmlChar *)"priority"), 1);

			if (name.compare("folder")) {
				fomod->requiredInstallFiles.folder.push_back(file);
			} else {
				fomod->requiredInstallFiles.file.push_back(file);
			}
		}
	}
}

FOModule* ParseConfigXml(xmlNode *node) {
	FOModule* fomod = new FOModule();
	xmlNode *cur_node = NULL;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		printf(">>%s\n", cur_node->name);

		string name((char *)cur_node->name);

		if (!name.compare("moduleName")) {
			fomod->moduleName.value = (char *)xmlNodeGetContent(cur_node);
		} else if (!name.compare("moduleImage")) {
			fomod->moduleImage.path = (char *)xmlGetProp(cur_node, (const xmlChar*)"path");
		} else if (!name.compare("requiredInstallFiles")) {
			ParseRequiredInstallFiles(fomod, cur_node->children);
		}
	}

	return fomod;
}

FOModule* ParseXml(xmlNode *node) {
	FOModule* fomod;
	xmlNode *cur_node = NULL;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf(">%s\n", cur_node->name);

			if (!string((char *)cur_node->name).compare("config")) {
				fomod = ParseConfigXml(cur_node->children);
			}

			if (xmlChildElementCount(cur_node) == 0) {
				/* JSON string object */
//        cur_jstr = json_object_new_string(xmlNodeGetContent(cur_node));
//        cur_jobj = json_object_new_object();
//        json_object_object_add(jobj, cur_node->name, cur_jstr);
			} else {
				/* JSON object */
//        cur_jobj = json_object_new_object();
//        json_object_object_add(jobj, cur_node->name, json_object_get(cur_jobj));
			}
		}

//		ParseXml(cur_node->children, data);
	}

	return fomod;
}

int ParseModuleConfig(char *name) {
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

	if (doc == NULL) {
		printf("error: could not parse file %s\n", name);
		retVal = EXIT_FAILURE;
	}

	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);

	FOModule *fomod = ParseXml(root_element);

	cout << "Name " << fomod->moduleName.value << endl;
	cout << "Image " << fomod->moduleImage.path << endl;

	delete fomod;

	/*free the document */
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	return retVal;
}

int main(int argc, char *argv[]) {
	ParseModuleConfig(argv[1]);

	return 0;
}
