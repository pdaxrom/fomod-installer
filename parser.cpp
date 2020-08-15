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
	for (; node; node = node->next) {
		if(xmlIsBlankNode(node)) {
			continue;
		}

		string name((char *)node->name);

		if (!name.compare("folder") ||
				!name.compare("file")) {
			FileType file;
			file.source = (char *)xmlGetProp(node, (const xmlChar*)"source");
			file.destination = (char *)xmlGetProp(node, (const xmlChar*)"destination");
			file.alwaysInstall = GetBooleanFromString((char *) xmlGetProp(node, (const xmlChar *)"alwaysInstall"), 1);
			file.installIfUsable = GetBooleanFromString((char *) xmlGetProp(node, (const xmlChar *)"installIfUsable"), 1);
			file.priority = GetIntegerFromString((char *) xmlGetProp(node, (const xmlChar *)"priority"), 1);

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

	for (; node; node = node->next) {
		if(xmlIsBlankNode(node)) {
			continue;
		}

		string name((char *)node->name);

		if (!name.compare("moduleName")) {
			fomod->moduleName.value = (char *)xmlNodeGetContent(node);
		} else if (!name.compare("moduleImage")) {
			fomod->moduleImage.path = (char *)xmlGetProp(node, (const xmlChar*)"path");
		} else if (!name.compare("requiredInstallFiles")) {
			ParseRequiredInstallFiles(fomod, node->children);
		}
	}

	return fomod;
}

FOModule* ParseXml(xmlNode *node) {
	FOModule* fomod;

	for (; node; node = node->next) {
		if(xmlIsBlankNode(node)) {
			continue;
		}
		if (node->type == XML_ELEMENT_NODE) {
			if (!string((char *)node->name).compare("config")) {
				fomod = ParseConfigXml(node->children);
			}
		}
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
