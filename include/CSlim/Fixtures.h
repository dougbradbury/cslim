#include "StatementExecutor.h"

#define SLIM_QUOTES(x)  #x
#define SLIM_FIXTURE(fixture) \
extern void fixture##_Register(StatementExecutor*);\
StatementExecutor_AddFixture(executor, fixture##_Register);

#define SLIM_FIXTURES void AddFixtures(StatementExecutor* executor) \
{

#define SLIM_END }

#define PRIVATE_REGISTER_FIXTURE(name, argc) StatementExecutor_RegisterFixture(executor, #name, argc);

#define SLIM_CREATE_EMPTY_FIXTURE(name)  \
	void name##_Register(StatementExecutor* executor) \
	{ \
		PRIVATE_REGISTER_FIXTURE(name)\
	}

#define SLIM_ABORT(reason) SLIM_QUOTES(__EXCEPTION__:ABORT_SLIM_TEST:message:<<reason.>>)

#define SLIM_EXCEPTION(reason) SLIM_QUOTES(__EXCEPTION__:message:<<reason.>>)

#define SLIM_CONSTRUCTOR_ERROR(errorHandler, reason) 		StatementExecutor_ConstructorError(errorHandler, reason);
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLIM_CREATE_FIXTUREx macros
//
// The (*co) variable initialization below is to ensure SLIM_IMPLEMENT_DYNCREATEx macro is written; 
// it will cause a linking error if not
// The f variable declaration below is to ensure constructor signature is as expected; it will cause 
// a compile error if not
//
#define SLIM_CREATE_FIXTURE0(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 0) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*) = &name::CreateObject; \
    name f(executor);

#define SLIM_CREATE_FIXTURE1(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 1) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*) = &name::CreateObject; \
    name f(executor, "a1");

#define SLIM_CREATE_FIXTURE2(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 2) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2");

#define SLIM_CREATE_FIXTURE3(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 3) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3");

#define SLIM_CREATE_FIXTURE4(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 4) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3", "a4");

#define SLIM_CREATE_FIXTURE5(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 5) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3", "a4", "a5");

#define SLIM_CREATE_FIXTURE6(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 6) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3", "a4", "a5", "a6");

#define SLIM_CREATE_FIXTURE7(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 7) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3", "a4", "a5", "a6", "a7");

#define SLIM_CREATE_FIXTURE8(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 8) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8");

#define SLIM_CREATE_FIXTURE9(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 9) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9");

#define SLIM_CREATE_FIXTURE10(name) static const char * fixtureName = #name; \
void name##_Register(StatementExecutor* executor) \
{ \
	PRIVATE_REGISTER_FIXTURE(name, 10) \
    typedef name ThisClass; \
    SlimFixtureBase*(*co)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*) = &name::CreateObject; \
    name f(executor, "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "a10");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLIM_FUNCTIONx macros
//  
// The m variable initialization below is to ensure method signature is as expected; 
// it will cause a compile error if not
//
#define SLIM_FUNCTION0(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 0); \
    { char*(ThisClass::*m)() = &ThisClass::name; } 

#define SLIM_FUNCTION1(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 1); \
    { char*(ThisClass::*m)(char*) = &ThisClass::name; } 

#define SLIM_FUNCTION2(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 2); \
    { char*(ThisClass::*m)(char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION3(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 3); \
    { char*(ThisClass::*m)(char*, char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION4(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 4); \
    { char*(ThisClass::*m)(char*, char*, char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION5(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 5); \
    { char*(ThisClass::*m)(char*, char*, char*, char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION6(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 6); \
    { char*(ThisClass::*m)(char*, char*, char*, char*, char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION7(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 7); \
    { char*(ThisClass::*m)(char*, char*, char*, char*, char*, char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION8(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 8); \
    { char*(ThisClass::*m)(char*, char*, char*, char*, char*, char*, char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION9(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 9); \
    { char*(ThisClass::*m)(char*, char*, char*, char*, char*, char*, char*, char*, char*) = &ThisClass::name; } 

#define SLIM_FUNCTION10(name) \
    StatementExecutor_RegisterMethod(executor, fixtureName, #name, (SlimBaseMethod0)&ThisClass::name, 10); \
    { char*(ThisClass::*m)(char*, char*, char*, char*, char*, char*, char*, char*, char*, char*) = &ThisClass::name; } 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLIM_DECLARE_DYNCREATEx macros
//  
#define SLIM_DECLARE_DYNCREATE0(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*);

#define SLIM_DECLARE_DYNCREATE1(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*);

#define SLIM_DECLARE_DYNCREATE2(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*);

#define SLIM_DECLARE_DYNCREATE3(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*);

#define SLIM_DECLARE_DYNCREATE4(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*, char*);

#define SLIM_DECLARE_DYNCREATE5(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*, char*, char*);

#define SLIM_DECLARE_DYNCREATE6(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*, char*, char*, char*);

#define SLIM_DECLARE_DYNCREATE7(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*);

#define SLIM_DECLARE_DYNCREATE8(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*);

#define SLIM_DECLARE_DYNCREATE9(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*, char*);

#define SLIM_DECLARE_DYNCREATE10(name) \
private: \
    static ClassInfo m_cInfo; \
public: \
    static SlimFixtureBase* CreateObject(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLIM_IMPLEMENT_DYNCREATEx macros
//  
#define SLIM_IMPLEMENT_DYNCREATE0(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor) \
{ return new name(executor); }

#define SLIM_IMPLEMENT_DYNCREATE1(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1) \
{ return new name(executor, a1); }

#define SLIM_IMPLEMENT_DYNCREATE2(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2) \
{ return new name(executor, a1, a2); }

#define SLIM_IMPLEMENT_DYNCREATE3(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3) \
{ return new name(executor, a1, a2, a3); }

#define SLIM_IMPLEMENT_DYNCREATE4(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3, char* a4) \
{ return new name(executor, a1, a2, a3, a4); }

#define SLIM_IMPLEMENT_DYNCREATE5(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3, char* a4, char* a5) \
{ return new name(executor, a1, a2, a3, a4, a5); }

#define SLIM_IMPLEMENT_DYNCREATE6(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3, char* a4, char* a5, char* a6) \
{ return new name(executor, a1, a2, a3, a4, a5, a6); }

#define SLIM_IMPLEMENT_DYNCREATE7(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3, char* a4, char* a5, char* a6, char* a7) \
{ return new name(executor, a1, a2, a3, a4, a5, a6, a7); }

#define SLIM_IMPLEMENT_DYNCREATE8(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3, char* a4, char* a5, char* a6, char* a7, char* a8) \
{ return new name(executor, a1, a2, a3, a4, a5, a6, a7, a8); }

#define SLIM_IMPLEMENT_DYNCREATE9(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3, char* a4, char* a5, char* a6, char* a7, char* a8, char* a9) \
{ return new name(executor, a1, a2, a3, a4, a5, a6, a7, a8, a9); }

#define SLIM_IMPLEMENT_DYNCREATE10(name) \
ClassInfo name::m_cInfo(#name, (SlimBaseCreateObject0)(&name::CreateObject)); \
SlimFixtureBase* name::CreateObject(StatementExecutor* executor, char* a1, char* a2, char* a3, char* a4, char* a5, char* a6, char* a7, char* a8, char* a9, char* a10) \
{ return new name(executor, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }

