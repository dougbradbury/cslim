void StatementExecutor_AddFixture(void*, void*);
void StatementExecutor_RegisterFixture(void*, void *, void *, void *);
void StatementExecutor_RegisterMethod(void*, void * , void *, void *);
void StatementExecutor_ConstructorError(void *, char *);

#define FIXTURE(fixture) \
extern void fixture##_Register(void*);\
StatementExecutor_AddFixture(executor, fixture##_Register); 

#define FIXTURES void AddFixtures(void* executor) \
{

#define END }


#define CREATE_FIXTURE(name) static char * fixtureName = #name; \
void name##_Register(void* executor) \
{ \
	StatementExecutor_RegisterFixture(executor, #name, name##_Create, name##_Destroy);
#define FUNCTION(name) StatementExecutor_RegisterMethod(executor, fixtureName, #name, name);	
#define END }

#define QUOTES(x)  #x
#define SLIM_ABORT(reason) QUOTES(__EXCEPTION__:ABORT_SLIM_TEST:message:<<reason.>>)

#define SLIM_EXCEPTION(reason) QUOTES(__EXCEPTION__:message:<<reason.>>)

#define SLIM_CONSTRUCTOR_ERROR(errorHandler, reason) 		StatementExecutor_ConstructorError(errorHandler, reason);