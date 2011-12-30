#include "stdafx.h"
#include "Fixtures.h"
#include <string>
using namespace std;

class LoginDialogDriver : public SlimFixtureBase
{
    SLIM_DECLARE_DYNCREATE2(LoginDialogDriver)

private:
    string m_userName, m_password;
    string m_message;
    int m_loginAttempts;
    char m_result[32];
    
public:
    LoginDialogDriver(StatementExecutor* executor, char* userName, char* password) {
        m_userName = userName;
        m_password = password;
        m_loginAttempts = 0;
    }
    
    char* loginWithUsernameAndPassword(char* userName, char* password) {
        m_loginAttempts++;
        bool result = m_userName.compare(userName) == 0 && m_password.compare(password) == 0;
        if (result)
            m_message = m_userName + " logged in.";
        else
            m_message = m_userName + " not logged in.";
        return result? "true" : "false";
    }
        
    char* loginMessage() {
        return (char*)m_message.c_str();
    }
    
    char* numberOfLoginAttempts() {
        return _itoa(m_loginAttempts, m_result, 10);
    }
};

SLIM_IMPLEMENT_DYNCREATE2(LoginDialogDriver)

SLIM_CREATE_FIXTURE2(LoginDialogDriver)
    SLIM_FUNCTION2(loginWithUsernameAndPassword)
    SLIM_FUNCTION0(loginMessage)
    SLIM_FUNCTION0(numberOfLoginAttempts)
SLIM_END