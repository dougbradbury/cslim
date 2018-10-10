#include "ListExecutor.h"
#include "SlimUtil.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(_MSC_VER) && (_MSC_VER <= 1800) // Visual Studio 2013
#define snprintf _snprintf
#endif

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

static void AddResult(SlimList* list, const char* id, const char* result)
{
	SlimList* pair = SlimList_Create();	
	SlimList_AddString(pair, id);
	SlimList_AddString(pair, result);
	SlimList_AddList(list, pair);
	SlimList_Destroy(pair);	
}

const char* InvalidCommand(SlimList* instruction) {
	const char* id = SlimList_GetStringAt(instruction, 0);
	const char* command = SlimList_GetStringAt(instruction, 1);
	static char msg[128];
	snprintf(msg, (size_t)128, "__EXCEPTION__:message:<<INVALID_STATEMENT: [\"%s\", \"%s\"].>>", id, command);
	return CSlim_BuyString(msg);
}

const char* MalformedInstruction(SlimList* instruction) {
	static char msg[128];
	
	const char* listAsAString = SlimList_ToString(instruction);
	snprintf(msg, (size_t)128, "__EXCEPTION__:message:<<MALFORMED_INSTRUCTION %s.>>", listAsAString);
	CSlim_DestroyString(listAsAString);

	return CSlim_BuyString(msg);
}

const char* Import() {
	return CSlim_BuyString("OK");
}

const char* Make(ListExecutor* self, SlimList* instruction) {
    const char *instanceName, *className, *result;
	instanceName = SlimList_GetStringAt(instruction, 2);
	className = SlimList_GetStringAt(instruction, 3);
	SlimList* args = SlimList_GetTailAt(instruction, 4);
	result = CSlim_BuyString(StatementExecutor_Make(self->executor, instanceName, className, args));
	SlimList_Destroy(args);
	return result;
}

const char* Call(ListExecutor* self, SlimList* instruction) {
  const char *instanceName, *methodName, *result;
  SlimList* args;
	if (SlimList_GetLength(instruction) < 4)
		return MalformedInstruction(instruction);
	instanceName = SlimList_GetStringAt(instruction, 2);
	methodName = SlimList_GetStringAt(instruction, 3);
	args = SlimList_GetTailAt(instruction, 4);
	result = CSlim_BuyString(StatementExecutor_Call(self->executor, instanceName, methodName, args));
	SlimList_Destroy(args);
	return result;
}

const char* CallAndAssign(ListExecutor* self, SlimList* instruction) {
  const char *symbolName, *instanceName, *methodName, *result;
  SlimList* args;
	if (SlimList_GetLength(instruction) < 5)
		return MalformedInstruction(instruction);
	symbolName = SlimList_GetStringAt(instruction, 2);
	instanceName = SlimList_GetStringAt(instruction, 3);
	methodName = SlimList_GetStringAt(instruction, 4);
	args = SlimList_GetTailAt(instruction, 5);
	result = CSlim_BuyString(StatementExecutor_Call(self->executor, instanceName, methodName, args));
	StatementExecutor_SetSymbol(self->executor, symbolName, result);
	SlimList_Destroy(args);
	return result;
}

const char* Dispatch(ListExecutor* self, SlimList* instruction) {
	const char* command = SlimList_GetStringAt(instruction, 1);
	if (strcmp(command, "import") == 0)
		return Import();
	else if (strcmp(command, "make") == 0)
		return Make(self, instruction);
	else if (strcmp(command, "call") == 0)
		return Call(self, instruction);
	else if (strcmp(command, "callAndAssign") == 0)
		return CallAndAssign(self, instruction);
	else
		return InvalidCommand(instruction);
}

SlimList* ListExecutor_Execute(ListExecutor* self, SlimList* instructions)
{
	SlimList* results = SlimList_Create();

	SlimListIterator* iterator = SlimList_CreateIterator(instructions);
	while (SlimList_Iterator_HasItem(iterator)) {
		SlimList* instruction = SlimList_Iterator_GetList(iterator);
		const char* id = SlimList_GetStringAt(instruction, 0);
		const char* result = Dispatch(self, instruction);
		AddResult(results, id, result);

		CSlim_DestroyString(result);
		SlimList_Iterator_Advance(&iterator);
	}

	return results;
}
