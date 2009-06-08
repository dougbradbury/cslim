#ifdef __cplusplus
extern "C" {
#endif

#include "StatementExecutor.h"

#define SLIM_QUOTES(x)  #x
#define SLIM_FIXTURE(fixture) \
extern void fixture##_Register(StatementExecutor*);\
StatementExecutor_AddFixture(executor, fixture##_Register); 

#define SLIM_FIXTURES void AddFixtures(StatementExecutor* executor) \
{

#define SLIM_END }


#define SLIM_CREATE_FIXTURE(name) static char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	StatementExecutor_RegisterFixture(executor, #name, name##_Create, name##_Destroy);
#define SLIM_FUNCTION(name) StatementExecutor_RegisterMethod(executor, fixtureName, #name, name);	


#define SLIM_ABORT(reason) SLIM_QUOTES(__EXCEPTION__:ABORT_SLIM_TEST:message:<<reason.>>)

#define SLIM_EXCEPTION(reason) SLIM_QUOTES(__EXCEPTION__:message:<<reason.>>)

#define SLIM_CONSTRUCTOR_ERROR(errorHandler, reason) 		StatementExecutor_ConstructorError(errorHandler, reason);


#ifdef __cplusplus
}
#endif
