#pragma once 

#define SLIM_QUOTES(x)  #x
#define SLIM_FIXTURE(fixture) \
extern void fixture##_Register(Slim::StatementExecutor*);\
executor->AddFixture(fixture##_Register);

#define SLIM_FIXTURES void AddFixtures(Slim::StatementExecutor* executor) \
{

#define SLIM_END }

#define SLIM_BEGIN_FIXTURE(classname) \
typedef classname FixtureClass; \
typedef classname##::tSuper FixtureBaseClass; \
void classname##_Register(Slim::StatementExecutor* executor) \
{ \
  executor->RegisterFixture(FixtureBaseClass::m_fixtureName.c_str(), FixtureBaseClass::CreateInstance); \
} \
std::string FixtureBaseClass::m_fixtureName = #classname; \
FixtureClass::FixtureMethod FixtureBaseClass::m_lookupTable[] = \
{

#define SLIM_FUNCTION(name) { #name, &FixtureClass::name },

#define SLIM_END_FIXTURE }; \
size_t FixtureBaseClass::m_methodCount = _countof(FixtureBaseClass::m_lookupTable);


#define SLIM_ABORT(reason) SLIM_QUOTES(__EXCEPTION__:ABORT_SLIM_TEST:message:<<reason.>>)

#define SLIM_EXCEPTION(reason) SLIM_QUOTES(__EXCEPTION__:message:<<reason.>>)

#define SLIM_CONSTRUCTOR_ERROR(errorHandler, reason) 		StatementExecutor_ConstructorError(errorHandler, reason);
