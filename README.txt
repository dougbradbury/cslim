VCSlim - FitNesse SliM plugin port for VC++ 2010
Author: Chih Von

************
* Fixtures *
************
Fixtures in VCSlim are sets of objects that implement functions used in Fitnesse tests and fixtures should derive from Slim::FixtureBase in following fashion:
class Division : public Slim::FixtureBase<Division>
{
public:
  // REQUIRED by VCSlim machinery.
  typedef Slim::FixtureBase<Division>  tSuper;

  // FixtureIntf overrides.
  virtual ~Division();
  virtual void Destroy();

private:
  // REQUIRED by VCSlim machinery.
  friend tSuper;
  static Slim::FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string setNumerator(Slim::SlimList* args);
  . . .

  // Fixture states
  . . .
};
See SlimExecutable/Division.cpp for example.


Each fixture's registered function returns an std::string object by value which cleans up itself nicely when it goes out its scope.

VCSlim calls your fixture's registered functions throgh FixtureIntf::Execute method.  The functions also take a pointer to a SlimList that contains all the arguments were passed to your function.  These arguments will be strings or other SlimLists, and you'll have to just know which are which.  You should get familiar with the SlimList interface.  It's not very exciting, but you'll be using it a lot.  Take a close look at the SlimExecutable/Query.cpp.  It makes considerable use of SlimList, and the related function SlimList::Serialize;

You can cause a slim exception in any function by returning a message with the SLIM_EXCEPTION(reason) macro.  This will turn the cell being executed yellow.  To cause an exception in a Create function (if, for example the wrong number of arguments have been given to it), use the SLIM_CONSTRUCTOR_ERROR(errorHandler, reason) macro.  See SlimExecutable/Exceptions.cpp for examples of create exceptions.


****************
* Registration *
****************
Notice the SLIM_BEGIN_FIXTURE business at the top of the SlimExecutable/Division.cpp file.  
SLIM_BEGIN_FIXTURE(Division) 
  SLIM_FUNCTION(setNumerator)
  SLIM_FUNCTION(setDenominator)
  SLIM_FUNCTION(Quotient)
SLIM_END

This creates a function lookup table that will be used when VCSlim calls your fixture object's Execute method.  It is your responsibility to register your fixture and your functions with these Macros.   

You register a fixture by calling SLIM_BEGIN_FIXTURE(FixtureName).  The base class Slim::FixtureBase that your fixture derives from provides the factory method for this macro to work.  You then must define a static Create function and a virtual Destroy function for VCSlim machinery to work.  The create function is used to create a new fixture instance.  The destroy function is your destructor, you use it to release any resource that the create function acquired.  If you don't like the naming convention, you can write the registration function yourself.  Have a look at the macro definitions in CSlim/Include/CSlim/Fixtures.h.  

You register the functions you want to expose to Fitnesse with the SLIM_FUNCTION macro and they match the names used by Fitnesse tests. 

Now look at the SlimExecutable/Fixturess.cpp file.  This is where you tell the main Slim Server program about all your fixtures.  The SLIM_FIXTURES and SLIM_FIXTURE macros make this easy.  The fixture name you use in the FIXTURE macro in Fixtures.cpp must match the name you used in the SLIM_BEGIN_FIXTURE macro in your fixture file.


********************
* VS 2010 Solution *
********************
The only thing left is the VS2010 solution file.  The solution provided will build the CSlim library and its unit tests.  It will also build an executable from everything in /SlimExecutable.  

VCSlim depends on Boost library, you have to download Boost from ww.boost.org and install it.  Since the build will run unit tests automatically you will need boost::unit_test dll, not just headers.

After installed Boost you need to change the Boost include path in "Additional Include Directories" for all projects in the solution, you also need to change the Boost::unit_test dll path in "AAdditional Library Directories" for project CSlimTests.


***************************************
* Running the example acceptance test *
***************************************
You need to download FitNesse from fitnesse.org.  Example pages are in CSlim_VS2010/PageExamples.  Fire up fitnesse and create a new page like CslimExample.  Add this to your CslimExample page:

!contents -R2 -g -p -f -h

!define TEST_SYSTEM {slim}
!define TEST_RUNNER {<path>/cslim/Cslim_cslim}
!define COMMAND_PATTERN {%m}

You should be able to see the CounterTest, DivisionTest, etc.


******************
* Communications *
******************
VCSlim communicates with FitNesse over a stream socket.  A win32 socket implementation of sockets is provided.

