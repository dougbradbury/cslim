#ifndef D_StatementExecutor_H
#define D_StatementExecutor_H

#include "SlimList.h"
#include <map>
#include <string>

typedef struct StatementExecutor StatementExecutor;
typedef void(*Fixture)(StatementExecutor*);

StatementExecutor* StatementExecutor_Create(void);
void StatementExecutor_Destroy(StatementExecutor*);

void StatementExecutor_AddFixture(StatementExecutor* executor, Fixture);

char* StatementExecutor_Make(StatementExecutor*, char const* instanceName, char const* className, SlimList* args);
char* StatementExecutor_Call(StatementExecutor*, char const* instanceName, char const* methodName, SlimList*);
void* StatementExecutor_Instance(StatementExecutor*, char const* instanceName);
void StatementExecutor_SetSymbol(StatementExecutor*, char const* symbol, char const* value);

void StatementExecutor_ConstructorError(StatementExecutor* executor, char const* message);
char* StatementExecutor_FixtureError(char const* message);

class SlimFixtureBase;
struct ClassInfo;
bool Register(ClassInfo* ci);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SlimBaseCreateObjectx type definitions
//
typedef SlimFixtureBase*(*SlimBaseCreateObject0)(StatementExecutor*);
typedef SlimFixtureBase*(*SlimBaseCreateObject1)(StatementExecutor*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject2)(StatementExecutor*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject3)(StatementExecutor*, char*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject4)(StatementExecutor*, char*, char*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject5)(StatementExecutor*, char*, char*, char*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject6)(StatementExecutor*, char*, char*, char*, char*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject7)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject8)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject9)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*, char*);
typedef SlimFixtureBase*(*SlimBaseCreateObject10)(StatementExecutor*, char*, char*, char*, char*, char*, char*, char*, char*, char*, char*);
typedef SlimFixtureBase*(*ConstructorInvoker)(char const*, StatementExecutor*, SlimList*);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SlimBaseMethodx type definitions
//
typedef char*(SlimFixtureBase::*SlimBaseMethod0)();
typedef char*(SlimFixtureBase::*SlimBaseMethod1)(char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod2)(char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod3)(char*, char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod4)(char*, char*, char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod5)(char*, char*, char*, char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod6)(char*, char*, char*, char*, char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod7)(char*, char*, char*, char*, char*, char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod8)(char*, char*, char*, char*, char*, char*, char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod9)(char*, char*, char*, char*, char*, char*, char*, char*, char*);
typedef char*(SlimFixtureBase::*SlimBaseMethod10)(char*, char*, char*, char*, char*, char*, char*, char*, char*, char*);
typedef char*(*MethodInvoker)(SlimBaseMethod0, void*, SlimList*);

void StatementExecutor_RegisterFixture(StatementExecutor*, char const*, int);
void StatementExecutor_RegisterMethod(StatementExecutor*, char const*, char const*, SlimBaseMethod0, int);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper structure for dynamic object creation
//
struct ClassInfo
{
public:
    std::string type;
    SlimBaseCreateObject0 funcCreate;
    ClassInfo(std::string type, SlimBaseCreateObject0 funcCreate)
    {
        this->type = type;
        this->funcCreate = funcCreate;
        Register(this);
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SlimFixtureBase class
// All C++ fixture class must derive from this base class
//
class SlimFixtureBase
{
public:
    virtual ~SlimFixtureBase() {};  // This virtual dtor ensures casting of member function pointer to work, 
                                    // and ALSO ensures derived object get deleted
    static bool Register(ClassInfo* classInfo);
    static SlimFixtureBase* CreateObject0(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject1(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject2(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject3(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject4(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject5(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject6(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject7(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject8(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject9(char const* className, StatementExecutor* executor, SlimList* args);
    static SlimFixtureBase* CreateObject10(char const* className, StatementExecutor* executor, SlimList* args);
 
    static char* Method0(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method1(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method2(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method3(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method4(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method5(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method6(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method7(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method8(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method9(SlimBaseMethod0 method, void* instance, SlimList* args);
    static char* Method10(SlimBaseMethod0 method, void* instance, SlimList* args);

private:
    static std::map<std::string, ClassInfo*>& ClassInfoMap();
};

#endif
