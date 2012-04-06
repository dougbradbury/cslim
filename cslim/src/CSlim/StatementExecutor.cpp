// Fitnesse Slim plugin port for VC++ 2010
// 
// Copyright (C) 2012  Chih C. Von
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "StatementExecutor.h"

#include "Fixture.h"
#include "SymbolTable.h"
#include "SlimList.h"

#include <boost/format.hpp>

#include <assert.h>
#include <iostream>
#include <map>

namespace Slim
{
  namespace Detail
  {
    //
    class Fixtures
    {
    public:
      void Register(char const* className, Slim::Constructor const& constructor);
      Slim::Constructor const* GetConstructor(char const* className) const;

    private:
      std::map<std::string, Slim::Constructor> m_fixtures;
    };

    inline void Fixtures::Register(char const* className, Slim::Constructor const& constructor)
    {
      m_fixtures[className] = constructor;
    }

    inline Slim::Constructor const* Fixtures::GetConstructor(char const* className) const
    {
      auto it = m_fixtures.find(className);
      return it != m_fixtures.end() ? &(it->second) : 0;
    }

    //
    class Instances
    {
    public:
      ~Instances();

      void SetInstance(char const* instanceName, Slim::FixtureIntf* instance);
      Slim::FixtureIntf* GetInstance(char const* instanceName) const;
      void RemoveInstance(char const* instanceName);

    private:
      std::map<std::string, Slim::FixtureIntf*> m_instances;
    };

    Instances::~Instances()
    {
      std::for_each(m_instances.begin(), m_instances.end(),
        [](std::pair<std::string, Slim::FixtureIntf*> const& instance)
        {
          instance.second->Destroy();
        });
    }

    inline void Instances::SetInstance(char const* instanceName, Slim::FixtureIntf* instance)
    {
      assert(instance);
      if (m_instances[instanceName])
      {
        m_instances[instanceName]->Destroy();
      }
      m_instances[instanceName] = instance;
    }

    inline Slim::FixtureIntf* Instances::GetInstance(char const* instanceName) const
    {
      auto it = m_instances.find(instanceName);
      return it != m_instances.end() ? it->second : 0;
    }

    inline void Instances::RemoveInstance(char const* instanceName)
    {
      m_instances.erase(instanceName);
    }
  }


  //
  StatementExecutor::StatementExecutor()
    : symbolTable(new SymbolTable())
    , fixtures(new Detail::Fixtures())
    , instances(new Detail::Instances())
  {
  }

  StatementExecutor::~StatementExecutor()
  {
    delete symbolTable;
    delete fixtures;
    delete instances;
  }

  std::string StatementExecutor::Make(char const* instanceName, char const* className, SlimList* args)
  {
    Constructor const* fixtureConstructor = fixtures->GetConstructor(className);
    if (!fixtureConstructor) 
    {
      instances->RemoveInstance(instanceName);
      return (boost::format("__EXCEPTION__:message:<<NO_CLASS %1%.>>") % className).str();
    }

    symbolTable->ReplaceSymbols(args);
    FixtureIntf* instance = (*fixtureConstructor)(this, args);
    if (!instance)
    {
      return (boost::format("__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR %1% %2%.>>") % className % (userMessage ? userMessage : "")).str();
    }

    instances->SetInstance(instanceName, instance);
    userMessage = NULL;
    return "OK";
  }

  std::string StatementExecutor::Call(char const* instanceName, char const* methodName, SlimList* args)
  {
    FixtureIntf* instance = instances->GetInstance(instanceName);
    if (!instance)
    {
      return (boost::format("__EXCEPTION__:message:<<NO_INSTANCE %1%.>>") % instanceName).str();
    }

    try
    {
      symbolTable->ReplaceSymbols(args);
      return instance->Execute(methodName, args);
    }
    catch (std::exception const& e)
    {
      return (boost::format("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS %1%.>>") % e.what()).str();
    }
  }

  void StatementExecutor::AddFixture(AddFixtureFunc const& addFixture) 
  {
    addFixture(this);
  }

  void StatementExecutor::RegisterFixture(char const* className, Constructor const& constructor)
  {
    fixtures->Register(className, constructor);
  }

  void StatementExecutor::SetSymbol(char const* symbol, char const* value) 
  {
    symbolTable->SetValue(symbol, value);
  }

  void StatementExecutor::ConstructorError(char const* message) 
  {
    userMessage = message;	
  }

  std::string StatementExecutor::FixtureError(char const* message) 
  {
    return (boost::format("__EXCEPTION__:message:<<%1%.>>") % message).str();
  }

  static void* Null_Create(StatementExecutor* executor, SlimList* args)
  {
    return NULL;
  }

  static void Null_Destroy(void* self)
  {
  }
}
