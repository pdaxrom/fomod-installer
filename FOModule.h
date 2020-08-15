#include <string>
#include <list>

using namespace std;

class ModuleTitle {
public:
	string position;
	string colour;

	string value;
};

class HeaderImage {
public:
	string path;
	bool showImage;
	bool showFade;
	int height;
};

class FileDependency {
	string file;
	string state;
};

class FlagDependency {
	string flag;
	bool value;
};

class CompositeDependency {
	string op;

	list<FileDependency> fileDependency;
	list<FlagDependency> flagDependency;
	string gameDependency;
	string fommDependency;
	list<CompositeDependency> compositeDependency;
};

class FileType {
public:
	string source;
	string destination;
	bool alwaysInstall;
	bool installIfUsable;
	int priority;
};

class FileList {
public:
	list<FileType> file;
	list<FileType> folder;
};

class SetConditionFlag {
	string name;
	bool value;
};

class ConditionFlagList {
	list<SetConditionFlag> flag;
};

class DependencyPattern {
	CompositeDependency dependencies;
	string type;
};

class DependencyPatternList {
	list<DependencyPattern> pattern;
};

class DependencyPluginType {
	string defaultType;
	DependencyPatternList patterns;
};

class PluginTypeDescriptor {
	DependencyPluginType dependencyType;
	string type;
};

class Plugin {
	string description;
	string image;
	FileList files;
	ConditionFlagList conditionFlags;
	PluginTypeDescriptor typeDescriptor;
};

class PluginList {
	list<Plugin> plugin;
};

class Group {
	string name;
	string type;

	PluginList plugins;
};

class GroupList {
	list<Group> group;
};

class InstallStep {
	CompositeDependency visible;
	list<GroupList> optionalFileGroups;
};

class StepList {
	string order;
	list<InstallStep> installStep;
};

class ConditionalInstallPattern {
	CompositeDependency dependencies;
	FileList files;
};

class ConditionalInstallPatternList {
	list<ConditionalInstallPattern> pattern;
};

class ConditionalFileInstallList {
	ConditionalInstallPatternList patterns;
};

class FOModule {
public:
	ModuleTitle moduleName;
	HeaderImage moduleImage;
	CompositeDependency moduleDependencies;
	FileList requiredInstallFiles;
	StepList installSteps;
	ConditionalFileInstallList conditionalFileInstalls;
};
