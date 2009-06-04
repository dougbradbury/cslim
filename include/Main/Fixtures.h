void StatementExecutor_AddFixture(void*, void*);

#define FIXTURE(fixture) \
extern void fixture(void*);\
StatementExecutor_AddFixture(executor, fixture); 

#define FIXTURES void AddFixtures(void* executor)