#include "StatementExecutor.h"
#include "SlimList.h"
#include "SlimListDeserializer.h"
#include "SlimListSerializer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimUtil.h"
// #include <regex.h>
#include "SymbolTable.h"
#include "assert.h"
#include <ctype.h>


typedef struct methodNode {
	struct methodNode* next;
	char const* name;
	Method method;
} MethodNode;

typedef struct fixtureNode {
	struct fixtureNode* next;
	Constructor constructor;
	Destructor destructor;
	MethodNode* methods;
	char const* name;
} FixtureNode;

typedef struct instanceNode {
	struct instanceNode* next;
	char const* name;
	void* instance;
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
static void Null_Destroy(void* self);
static void* Null_Create(StatementExecutor* executor, SlimList* args);


StatementExecutor* StatementExecutor_Create(void)
{
	StatementExecutor* self = (StatementExecutor*)malloc(sizeof(StatementExecutor));
	memset(self, 0, sizeof(StatementExecutor));
	self->symbolTable = SymbolTable_Create();
	return self;
}

static int compareNamesIgnoreUnderScores(const char * name1, const char * name2)
{
	while (*name1 && *name2)
	{
		if (*name1 == *name2) { name1++; name2++;}
		else if ((*name1) == '_') name1++;
		else if (*name2 == '_') name2++;
		else return 0;
	}
	if (*name1 != *name2)
		return 0;
	return 1;
}

InstanceNode* GetInstanceNode(StatementExecutor* executor, char const* instanceName)
{
	InstanceNode* instanceNode;
	for (instanceNode = executor->instances; instanceNode; instanceNode = instanceNode->next) {
//			if (strcmp(instanceNode->name, instanceName) == 0) {
		if (compareNamesIgnoreUnderScores(instanceNode->name, instanceName)) {
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
		instanceNode->instance = (fixtureNode->constructor)(executor, args);
		if (instanceNode->instance != NULL) {	
			return "OK";
		} else {
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
			//			if (strcmp(methodName, node->name) == 0) {
			if (compareNamesIgnoreUnderScores(methodName, node->name)) {
				replaceSymbols(executor->symbolTable, args);
				char* retval =  node->method(instanceNode->instance, args);
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
    SlimListIterator* iterator = SlimList_CreateIterator(list);
    while (SlimList_Iterator_HasItem(iterator)) {
		char* string = SlimList_Iterator_GetString(iterator);
		SlimList* embeddedList = SlimList_Deserialize(string);
		if (embeddedList == NULL) {
			char* replacedString = replaceString(symbolTable, string);
			SlimList_Iterator_Replace(iterator, replacedString);
			free(replacedString);
		} else {
			replaceSymbols(symbolTable, embeddedList);
			char* serializedReplacedList = SlimList_Serialize(embeddedList);
			SlimList_Iterator_Replace(iterator, serializedReplacedList);
			SlimList_Destroy(embeddedList);
			SlimList_Release(serializedReplacedList);
		}
		SlimList_Iterator_Advance(&iterator);
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

void StatementExecutor_RegisterFixture(StatementExecutor* executor, char const * className, Constructor constructor, Destructor destructor){
	FixtureNode* fixtureNode = findFixture(executor, className);
	if (!fixtureNode)
	{
		fixtureNode = (FixtureNode*)malloc(sizeof(FixtureNode));
		fixtureNode->next = executor->fixtures;
		executor->fixtures = fixtureNode;
		fixtureNode->name = className;		
		fixtureNode->methods = NULL;
	}
		
	fixtureNode->constructor = constructor;
	fixtureNode->destructor = destructor;
}

static FixtureNode * findFixture(StatementExecutor* executor, char const* className)
{
	FixtureNode* fixtureNode = NULL;
	for (fixtureNode = executor->fixtures; fixtureNode; fixtureNode = fixtureNode->next) {
//		if (strcmp(fixtureNode->name, className) == 0) {
		if (compareNamesIgnoreUnderScores(fixtureNode->name, className)) {
			break;
		}
	}
	return fixtureNode;
}

void StatementExecutor_RegisterMethod(StatementExecutor* executor, char const * className, char const * methodName, Method method){
	FixtureNode* fixtureNode = findFixture(executor, className);
	if (fixtureNode == NULL) {
		StatementExecutor_RegisterFixture(executor, className, Null_Create, Null_Destroy);
		fixtureNode = findFixture(executor, className);
	}
		
	MethodNode* node = (MethodNode*)malloc(sizeof(MethodNode));
	node->name = methodName;
	node->method = method;
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

static void* Null_Create(StatementExecutor* executor, SlimList* args)
{
	return NULL;
}

static void Null_Destroy(void* self)
{
}


