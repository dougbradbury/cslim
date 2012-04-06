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
