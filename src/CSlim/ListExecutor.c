#include "ListExecutor.h"
#include "SlimUtil.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


struct ListExecutor
{
	StatementExecutor* executor;
};

ListExecutor* ListExecutor_Create(StatementExecutor* executor)
{
	ListExecutor* self = (ListExecutor*)malloc(sizeof(ListExecutor));
	memset(self, 0, sizeof(ListExecutor));
	self->executor = executor;
	return self;
}

void ListExecutor_Destroy(ListExecutor* self)
{
    free(self);
}

static void AddResult(SlimList* list, char* id, char* result)
{
	SlimList* pair = SlimList_Create();	
	SlimList_AddString(pair, id);
	SlimList_AddString(pair, result);
	SlimList_AddList(list, pair);
	SlimList_Destroy(pair);	
}

char* InvalidCommand(ListExecutor* self, SlimList* instruction) {
	char* id = SlimList_GetStringAt(instruction, 0);
	char* command = SlimList_GetStringAt(instruction, 1);
	static char msg[128];
	snprintf(msg, (size_t)128, "__EXCEPTION__:message:<<INVALID_STATEMENT: [\"%s\", \"%s\"].>>", id, command);
	return CSlim_BuyString(msg);
}

char* MalformedInstruction(ListExecutor* self, SlimList* instruction) {
	static char msg[128];
	snprintf(msg, (size_t)128, "__EXCEPTION__:message:<<MALFORMED_INSTRUCTION %s.>>", SlimList_ToString(instruction));
	return CSlim_BuyString(msg);
}

char* Import(ListExecutor* self, SlimList* instruction) {
	return CSlim_BuyString("OK");
}

char* Make(ListExecutor* self, SlimList* instruction) {
	char* instanceName = SlimList_GetStringAt(instruction, 2);
	char* className = SlimList_GetStringAt(instruction, 3);
	SlimList* args = SlimList_GetTailAt(instruction, 4);
	char * result = CSlim_BuyString(StatementExecutor_Make(self->executor, instanceName, className, args));
	SlimList_Destroy(args);
	return result;
}

char* Call(ListExecutor* self, SlimList* instruction) {
	if (SlimList_GetLength(instruction) < 4)
		return MalformedInstruction(self, instruction);
	char* instanceName = SlimList_GetStringAt(instruction, 2);
	char* methodName = SlimList_GetStringAt(instruction, 3);
	SlimList* args = SlimList_GetTailAt(instruction, 4);
	char* result = CSlim_BuyString(StatementExecutor_Call(self->executor, instanceName, methodName, args));
	SlimList_Destroy(args);
	return result;
}

char* CallAndAssign(ListExecutor* self, SlimList* instruction) {
	if (SlimList_GetLength(instruction) < 5)
		return MalformedInstruction(self, instruction);
	char* symbolName = SlimList_GetStringAt(instruction, 2);
	char* instanceName = SlimList_GetStringAt(instruction, 3);
	char* methodName = SlimList_GetStringAt(instruction, 4);
	SlimList* args = SlimList_GetTailAt(instruction, 5);
	char* result = CSlim_BuyString(StatementExecutor_Call(self->executor, instanceName, methodName, args));
	StatementExecutor_SetSymbol(self->executor, symbolName, result);
	SlimList_Destroy(args);
	return result;
}

char* Dispatch(ListExecutor* self, SlimList* instruction) {
	char* command = SlimList_GetStringAt(instruction, 1);
	if (strcmp(command, "import") == 0)
		return Import(self, instruction);
	else if (strcmp(command, "make") == 0)
		return Make(self, instruction);
	else if (strcmp(command, "call") == 0)
		return Call(self, instruction);
	else if (strcmp(command, "callAndAssign") == 0)
		return CallAndAssign(self, instruction);
	else
		return InvalidCommand(self, instruction);
}

SlimList* ListExecutor_Execute(ListExecutor* self, SlimList* instructions)
{
	SlimList* results = SlimList_Create();
	int numberOfInstructions = SlimList_GetLength(instructions);
	int n;
	for (n=0; n<numberOfInstructions; n++) {
		SlimList* instruction = SlimList_GetListAt(instructions, n);
		char* id = SlimList_GetStringAt(instruction, 0);
		char* result = Dispatch(self, instruction);
		AddResult(results, id, result);
		if (result)
			free(result);
	}

	return results;
}
