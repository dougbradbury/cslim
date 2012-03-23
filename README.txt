VCSlim - FitNesse SliM plugin port for VC++ 2010
Author: Chih Von

************
* Fixtures *
************
Fixtures in VCSlim are sets of objects that implement functions used in Fitnesse tests.   See SlimExecutable/Decision.h

Each function returns a std::string which is different from original CSlim implementation. You can dispose of it once you are done with it, period.


****************
* Registration *
****************


********************
* VS 2010 Solution *
********************


***************************************
* Running the example acceptance test *
***************************************
You need to download FitNesse from fitnesse.org.  Example pages are in CSlim_VS2010/PageExamples.  Fire up fitnesse and create a new page like CslimExample.  Add this to your CslimExample page:

!contents -R2 -g -p -f -h

!define TEST_SYSTEM {slim}
!define TEST_RUNNER {<path>/cslim/Cslim_cslim}
!define COMMAND_PATTERN {%m}

You should be able to see the CounterTest, DivisionTest, etc.


**************
* Unit Tests *
**************
VCSlim was built using boost::unit_test.


******************
* Communications *
******************
VCSlim communicates with FitNesse over a stream socket.  A win32 socket implementation of sockets is provided.

