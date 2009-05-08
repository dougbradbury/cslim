#include "StatementExecutor.h"
#include "SlimList.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "SlimUtil.h"
#include <regex.h>
#include "SymbolTable.h"
#include "assert.h"

typedef struct methodNode {
	struct methodNode* next;
	char* name;
	Method method;
} MethodNode;

typedef struct fixtureNode {
	struct fixtureNode* next;
	Constructor constructor;
	Destructor destructor;
	MethodNode* methods;
	char* name;
} FixtureNode;

typedef struct instanceNode {
	struct instanceNode* next;
	char* name;
	void* instance;
	FixtureNode* fixture;
} InstanceNode;

typedef struct symbolNode {
	struct symbolNode* next;
	char* name;
	char* value;
} SymbolNode;

struct StatementExecutor
{
	FixtureNode* fixtures;
	InstanceNode* instances;	
	SymbolTable * symbolTable;
	char message[120];
};



static void destroyInstances(InstanceNode*);
static void destroyFixtures(FixtureNode*);
static void destroyMethods(MethodNode*);
void replaceSymbols(SymbolTable*, SlimList*);
static char* replaceString(SymbolTable*, char*);
static char* replaceStringFrom(SymbolTable*, char*, char*);
static int lengthOfSymbol(char *);

StatementExecutor* StatementExecutor_create(void)
{
	StatementExecutor* self = malloc(sizeof(StatementExecutor));
	memset(self, 0, sizeof(StatementExecutor));
	self->symbolTable = SymbolTable_Create();
	return self;
}

InstanceNode* GetInstanceNode(StatementExecutor* executor, char * instanceName)
{
	InstanceNode* instanceNode;
	for (instanceNode = executor->instances; instanceNode; instanceNode = instanceNode->next) {
		if (strcmp(instanceNode->name, instanceName) == 0) {
			return instanceNode;
		}
	}
	return NULL;
}

void StatementExecutor_destroy(StatementExecutor* self)
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
		instanceNode->fixture->destructor(instanceNode->instance);
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

char* StatementExecutor_make(StatementExecutor* executor, char* instanceName, char* className){
	FixtureNode* fixtureNode;
	for (fixtureNode = executor->fixtures; fixtureNode; fixtureNode = fixtureNode->next) {
		if (strcmp(fixtureNode->name, className) == 0) {
			InstanceNode* instanceNode = malloc(sizeof(InstanceNode));
			instanceNode->next = executor->instances;
			executor->instances = instanceNode;
			instanceNode->name = instanceName;
			instanceNode->fixture = fixtureNode;
			instanceNode->instance = (fixtureNode->constructor)();	
			return "OK";
		}
	}
	char * formatString = "__EXCEPTION__:message:<<NO_CLASS %.32s.>>";
	snprintf(executor->message, 120, formatString, className);	
	return executor->message;	
}

char* StatementExecutor_call(StatementExecutor* executor, char* instanceName, char* methodName, SlimList* args){
	InstanceNode* instanceNode = GetInstanceNode(executor, instanceName);
	if (instanceNode)
	{
		MethodNode* node;
		for (node = instanceNode->fixture->methods; node; node = node->next) {
			if (strcmp(methodName, node->name) == 0) {
				replaceSymbols(executor->symbolTable, args);
				char* retval =  node->method(instanceNode->instance, args);
				return retval;
			}
		}
		char * formatString = "__EXCEPTION__:message:<<NO_METHOD_IN_CLASS %.32s[%d] %.32s.>>";
		snprintf(executor->message, 120, formatString, methodName, SlimList_getLength(args), instanceNode->fixture->name);
		return executor->message;
	}
	char * formatString = "__EXCEPTION__:message:<<NO_INSTANCE %.32s.>>";
	snprintf(executor->message, 120, formatString, instanceName);
	return executor->message;
}

void replaceSymbols(SymbolTable* symbolTable, SlimList* list) {
	int i;
	for (i=0; i<SlimList_getLength(list); i++) {
		char* string = SlimList_getStringAt(list, i);
		SlimList* embeddedList = SlimList_deserialize(string);
		if (embeddedList == NULL) {
			char* replacedString = replaceString(symbolTable, string);
			SlimList_replaceAt(list, i, replacedString);
			free(replacedString);
		} else {
			replaceSymbols(symbolTable, embeddedList);
			char* serializedReplacedList = SlimList_serialize(embeddedList);
			SlimList_replaceAt(list, i, serializedReplacedList);
			SlimList_destroy(embeddedList);
			free(serializedReplacedList);
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
			char * newString = malloc(bufferLength);
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
				return buyString(string);
				
			return replaceStringFrom(symbolTable, string, dollarSign+1);
		}
	}
	return buyString(string);
}

#include <ctype.h>

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

void* StatementExecutor_instance(StatementExecutor* executor, char* instanceName){
	
	InstanceNode* instanceNode = GetInstanceNode(executor, instanceName);
	if (instanceNode)
		return instanceNode->instance;
	return NULL;
}

void StatementExecutor_addFixture(StatementExecutor* executor, Fixture fixture) {
	fixture(executor);
}

void StatementExecutor_registerFixture(StatementExecutor* executor, char * className, Constructor constructor, Destructor destructor){
	FixtureNode* fixtureNode = malloc(sizeof(FixtureNode));
	fixtureNode->next = executor->fixtures;
	executor->fixtures = fixtureNode;
	fixtureNode->name = className;
	fixtureNode->constructor = constructor;
	fixtureNode->destructor = destructor;
	fixtureNode->methods = NULL;
}
void StatementExecutor_registerMethod(StatementExecutor* executor, char * className, char * methodName, Method method){
	FixtureNode* fixtureNode;
	for (fixtureNode = executor->fixtures; fixtureNode; fixtureNode = fixtureNode->next) {
		if (strcmp(fixtureNode->name, className) == 0) {
			MethodNode* node = malloc(sizeof(MethodNode));
			node->name = methodName;
			node->method = method;
			node->next = fixtureNode->methods;
			fixtureNode->methods = node;
			return;			
		}	
	}
	//TODO let fixture writer know he forgot to add the fixture
}

void StatementExecutor_setSymbol(StatementExecutor* self, char* symbol, char* value) {
	SymbolTable_SetSymbol(self->symbolTable, symbol, value);
}

