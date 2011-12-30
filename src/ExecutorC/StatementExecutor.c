#include "StatementExecutor.h"
#include "SlimList.h"
#include "SlimListDeserializer.h"
#include "SlimListSerializer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimUtil.h"
#include "SymbolTable.h"
#include "assert.h"
#include <ctype.h>

typedef struct methodNode {
	struct methodNode* next;
	char const* name;
    SlimBaseMethod0 method;
    MethodInvoker methodInvoker;
} MethodNode;

typedef struct fixtureNode {
	struct fixtureNode* next;
    ConstructorInvoker constructorInvoker;
	MethodNode* methods;
	char const* name;
} FixtureNode;

typedef struct instanceNode {
	struct instanceNode* next;
	char const* name;
    SlimFixtureBase* instance;
	FixtureNode* fixture;
} InstanceNode;

typedef struct symbolNode {
	struct symbolNode* next;
	char const* name;
	char const* value;
} SymbolNode;

struct StatementExecutor
{
	FixtureNode* fixtures;
	InstanceNode* instances;	
	SymbolTable * symbolTable;
	char message[120];
	char const* userMessage;
};

static void destroyInstances(InstanceNode*);
static void destroyFixtures(FixtureNode*);
static void destroyMethods(MethodNode*);
void replaceSymbols(SymbolTable*, SlimList*);
static char* replaceString(SymbolTable*, char*);
static char* replaceStringFrom(SymbolTable*, char*, char*);
static int lengthOfSymbol(char *);
static FixtureNode * findFixture(StatementExecutor* executor, char const * className);


StatementExecutor* StatementExecutor_Create(void)
{
	StatementExecutor* self = (StatementExecutor*)malloc(sizeof(StatementExecutor));
	memset(self, 0, sizeof(StatementExecutor));
	self->symbolTable = SymbolTable_Create();
	return self;
}

InstanceNode* GetInstanceNode(StatementExecutor* executor, char const* instanceName)
{
	InstanceNode* instanceNode;
	for (instanceNode = executor->instances; instanceNode; instanceNode = instanceNode->next) {
		if (strcmp(instanceNode->name, instanceName) == 0) {
			return instanceNode;
		}
	}
	return NULL;
}

void StatementExecutor_Destroy(StatementExecutor* self)
{
	destroyInstances(self->instances);
	destroyFixtures(self->fixtures);
	SymbolTable_Destroy(self->symbolTable);
    free(self);
}

static void destroyInstances(InstanceNode* head) {
	InstanceNode* instanceNode;
	for (instanceNode = head; instanceNode;) {
		InstanceNode* nextInstanceNode = instanceNode->next;
        delete instanceNode->instance;
		free(instanceNode);
		instanceNode = nextInstanceNode;	
	}
}

static void destroyFixtures(FixtureNode* head) {
	FixtureNode* fixtureNode;
	for (fixtureNode = head; fixtureNode;) {
		FixtureNode* nextFixtureNode = fixtureNode->next;
		destroyMethods(fixtureNode->methods);
		free(fixtureNode);
		fixtureNode = nextFixtureNode;
	}
}

static void destroyMethods(MethodNode* head) {
	MethodNode* node;
	for (node = head; node;) {
		MethodNode* nextNode = node->next;
		free(node);
		node = nextNode;
	}
}

char* StatementExecutor_Make(StatementExecutor* executor, char const* instanceName, char const* className, SlimList* args){
	FixtureNode* fixtureNode = findFixture(executor, className);
	if (fixtureNode) {
		InstanceNode* instanceNode = (InstanceNode* )malloc(sizeof(InstanceNode));
		instanceNode->next = executor->instances;
		executor->instances = instanceNode;
		instanceNode->name = instanceName;
		instanceNode->fixture = fixtureNode;
		replaceSymbols(executor->symbolTable, args);
		executor->userMessage = NULL;
        instanceNode->instance = fixtureNode->constructorInvoker(className, executor, args);

		if (instanceNode->instance != NULL) {	
			return "OK";
		} else {
            // Empty instance name so GetInstanceNode() won't succeed in StatementExecutor_Call(), and won't call the null-instance method, and will return "NO_INTANCE" exception
            // The other way is to move the above instanceNode malloc and member-filling code to the preceding if-body
            // i.e. only when instance can be instantiated then instanceNode is created and linked
            instanceNode->name = "";
			char * formatString = "__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR %.32s %.32s.>>";
			snprintf(executor->message, 120, formatString, className, executor->userMessage ? executor->userMessage : "");	
			return executor->message;	
		}
	}
	char * formatString = "__EXCEPTION__:message:<<NO_CLASS %.32s.>>";
	snprintf(executor->message, 120, formatString, className);	
	return executor->message;	
}

char* StatementExecutor_Call(StatementExecutor* executor, char const* instanceName, char const* methodName, SlimList* args){
	InstanceNode* instanceNode = GetInstanceNode(executor, instanceName);
	if (instanceNode)
	{
		MethodNode* node;
		for (node = instanceNode->fixture->methods; node; node = node->next) {
			if (strcmp(methodName, node->name) == 0) {
				replaceSymbols(executor->symbolTable, args);
                char* retval =  node->methodInvoker(node->method, instanceNode->instance, args);
				return retval;
			}
		}
		char * formatString = "__EXCEPTION__:message:<<NO_METHOD_IN_CLASS %.32s[%d] %.32s.>>";
		snprintf(executor->message, 120, formatString, methodName, SlimList_GetLength(args), instanceNode->fixture->name);
		return executor->message;
	}
	char * formatString = "__EXCEPTION__:message:<<NO_INSTANCE %.32s.>>";
	snprintf(executor->message, 120, formatString, instanceName);
	return executor->message;
}

void replaceSymbols(SymbolTable* symbolTable, SlimList* list) {
	int i;
	for (i=0; i<SlimList_GetLength(list); i++) {
		char* string = SlimList_GetStringAt(list, i);
		SlimList* embeddedList = SlimList_Deserialize(string);
		if (embeddedList == NULL) {
			char* replacedString = replaceString(symbolTable, string);
			SlimList_ReplaceAt(list, i, replacedString);
			free(replacedString);
		} else {
			replaceSymbols(symbolTable, embeddedList);
			char* serializedReplacedList = SlimList_Serialize(embeddedList);
			SlimList_ReplaceAt(list, i, serializedReplacedList);
			SlimList_Destroy(embeddedList);
			SlimList_Release(serializedReplacedList);
		}
	}
}

static char* replaceString(SymbolTable* symbolTable, char* string) {
	return replaceStringFrom(symbolTable, string, string);
}

static char* replaceStringFrom(SymbolTable* symbolTable, char* string, char* from) {
	char * dollarSign = strpbrk(from, "$");
	if (dollarSign)
	{
		int length = lengthOfSymbol(dollarSign + 1);
		char * symbolValue = SymbolTable_FindSymbol(symbolTable, dollarSign + 1, length);
		if (symbolValue)
		{
			int valueLength = strlen(symbolValue);
			int bufferLength = strlen(string)+valueLength-(length+1) + 1;
			char * newString = (char*)malloc(bufferLength);
			memset(newString, 0, bufferLength);
			strncat(newString, string, (dollarSign - string));
			strcat(newString, symbolValue);
			strcat(newString, dollarSign + 1 + length);

			assert(bufferLength == strlen(newString) + 1);

			char* recursedString = replaceStringFrom(symbolTable, newString, newString);
			free(newString);
			return recursedString;
		}
		else
		{
			if (*(dollarSign+1) == 0)
				return CSlim_BuyString(string);
				
			return replaceStringFrom(symbolTable, string, dollarSign+1);
		}
	}
	return CSlim_BuyString(string);
}

static int lengthOfSymbol(char * start)
{
	int length = 0;
	while(isalnum(*start))
	{
		start++;
		length ++;
	}
	return length;
}

void* StatementExecutor_Instance(StatementExecutor* executor, char const* instanceName){
	
	InstanceNode* instanceNode = GetInstanceNode(executor, instanceName);
	if (instanceNode)
		return instanceNode->instance;
	return NULL;
}

void StatementExecutor_AddFixture(StatementExecutor* executor, Fixture fixture) {
	fixture(executor);
}

bool Register(ClassInfo* ci)
{
    return SlimFixtureBase::Register(ci);
}

bool SlimFixtureBase::Register(ClassInfo* classInfo)
{
    ClassInfoMap()[classInfo->type] = classInfo;
    return true;
}
 
// Use this to avoid the "static initialization order fiasco" between original SlimFixtureBase::m_classInfoMap and <DerivedFixtureClass>::m_cInfo
// http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.14
std::map<std::string, ClassInfo*>& SlimFixtureBase::ClassInfoMap()
{
    static std::map<std::string, ClassInfo*> classInfoMap;
    return classInfoMap;
}

SlimFixtureBase* SlimFixtureBase::CreateObject0(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject0)ClassInfoMap()[className]->funcCreate)(executor);
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject1(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject1)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject2(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject2)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0), 
            SlimList_GetStringAt(args, 1));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject3(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject3)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject4(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject4)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject5(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject5)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
            SlimList_GetStringAt(args, 4));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject6(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject6)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
            SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject7(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject7)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
            SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject8(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject8)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
            SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6),
            SlimList_GetStringAt(args, 7));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject9(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject9)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
            SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6),
            SlimList_GetStringAt(args, 7), SlimList_GetStringAt(args, 8));
    }
    return NULL;
}

SlimFixtureBase* SlimFixtureBase::CreateObject10(char const* className, StatementExecutor* executor, SlimList* args)
{
    if (ClassInfoMap()[className] != NULL)
    {
        return ((SlimBaseCreateObject10)ClassInfoMap()[className]->funcCreate)(executor, SlimList_GetStringAt(args, 0),
            SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
            SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6),
            SlimList_GetStringAt(args, 7), SlimList_GetStringAt(args, 8), SlimList_GetStringAt(args, 9));
    }
    return NULL;
}

ConstructorInvoker findConstructorInvoker(int argc)
{
    static ConstructorInvoker constructorInvokers[] =
    {
        SlimFixtureBase::CreateObject0,
        SlimFixtureBase::CreateObject1,
        SlimFixtureBase::CreateObject2,
        SlimFixtureBase::CreateObject3,
        SlimFixtureBase::CreateObject4,
        SlimFixtureBase::CreateObject5,
        SlimFixtureBase::CreateObject6,
        SlimFixtureBase::CreateObject7,
        SlimFixtureBase::CreateObject8,
        SlimFixtureBase::CreateObject9,
        SlimFixtureBase::CreateObject10
    };

    if (argc < 0 || argc > sizeof(constructorInvokers)/sizeof(constructorInvokers[0]) - 1)
    {
        return NULL;
    }
    return constructorInvokers[argc];
}

void StatementExecutor_RegisterFixture(StatementExecutor* executor, char const * className, int argc){
	FixtureNode* fixtureNode = findFixture(executor, className);
	if (!fixtureNode)
	{
		fixtureNode = (FixtureNode*)malloc(sizeof(FixtureNode));
		fixtureNode->next = executor->fixtures;
		executor->fixtures = fixtureNode;
		fixtureNode->name = className;		
		fixtureNode->methods = NULL;
	}
		
    fixtureNode->constructorInvoker = findConstructorInvoker(argc);
}

static FixtureNode * findFixture(StatementExecutor* executor, char const* className)
{
	FixtureNode* fixtureNode = NULL;
	for (fixtureNode = executor->fixtures; fixtureNode; fixtureNode = fixtureNode->next) {
		if (strcmp(fixtureNode->name, className) == 0) {
			break;
		}
	}
	return fixtureNode;
}

char* SlimFixtureBase::Method0(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod0)method)();
}

char* SlimFixtureBase::Method1(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod1)method)(SlimList_GetStringAt(args, 0));
}

char* SlimFixtureBase::Method2(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod2)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1));
}

char* SlimFixtureBase::Method3(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod3)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2));
}

char* SlimFixtureBase::Method4(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod4)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3));
}

char* SlimFixtureBase::Method5(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod5)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
        SlimList_GetStringAt(args, 4));
}

char* SlimFixtureBase::Method6(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod6)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
        SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5));
}

char* SlimFixtureBase::Method7(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod7)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
        SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6));
}

char* SlimFixtureBase::Method8(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod8)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
        SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6),
        SlimList_GetStringAt(args, 7));
}

char* SlimFixtureBase::Method9(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod9)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
        SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6),
        SlimList_GetStringAt(args, 7), SlimList_GetStringAt(args, 8));
}

char* SlimFixtureBase::Method10(SlimBaseMethod0 method, void* instance, SlimList* args)
{
    return (((SlimFixtureBase*)instance)->*(SlimBaseMethod10)method)(SlimList_GetStringAt(args, 0),
        SlimList_GetStringAt(args, 1), SlimList_GetStringAt(args, 2), SlimList_GetStringAt(args, 3),
        SlimList_GetStringAt(args, 4), SlimList_GetStringAt(args, 5), SlimList_GetStringAt(args, 6),
        SlimList_GetStringAt(args, 7), SlimList_GetStringAt(args, 8), SlimList_GetStringAt(args, 9));
}

MethodInvoker findMethodInvoker(int argc)
{
    static MethodInvoker methodInvokers[] =
    {
        SlimFixtureBase::Method0,
        SlimFixtureBase::Method1,
        SlimFixtureBase::Method2,
        SlimFixtureBase::Method3,
        SlimFixtureBase::Method4,
        SlimFixtureBase::Method5,
        SlimFixtureBase::Method6,
        SlimFixtureBase::Method7,
        SlimFixtureBase::Method8,
        SlimFixtureBase::Method9,
        SlimFixtureBase::Method10,
    };

    if (argc < 0 || argc > sizeof(methodInvokers)/sizeof(methodInvokers[0]) - 1)
    {
        return NULL;
    }
    return methodInvokers[argc];
}

void StatementExecutor_RegisterMethod(StatementExecutor* executor, char const * className, char const * methodName, SlimBaseMethod0 method, int argc){
	FixtureNode* fixtureNode = findFixture(executor, className);
	if (fixtureNode == NULL) {
		StatementExecutor_RegisterFixture(executor, className, 0);
		fixtureNode = findFixture(executor, className);
	}
		
	MethodNode* node = (MethodNode*)malloc(sizeof(MethodNode));
	node->name = methodName;
	node->method = method;  // node->method is type SlimBaseMethod0
    node->methodInvoker = findMethodInvoker(argc);
	node->next = fixtureNode->methods;
	fixtureNode->methods = node;
	return;			
}

void StatementExecutor_SetSymbol(StatementExecutor* self, char const* symbol, char const* value) {
	SymbolTable_SetSymbol(self->symbolTable, symbol, value);
}

void StatementExecutor_ConstructorError(StatementExecutor* executor, char const* message) {
	executor->userMessage = message;	
}

char* StatementExecutor_FixtureError(char const* message) {
	static char buffer[128];
	char * formatString = "__EXCEPTION__:message:<<%.100s.>>";
	snprintf(buffer, 128, formatString, message);	
	return buffer;	
}
