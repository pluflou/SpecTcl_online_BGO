/*************************************************************************** 
   SpecTcl HabaNERO for DDAS
   Based on standard BCS SpecTcl codes

   J.Pereira. Last version 8/29/2005
   S.N.Liddick/H.L.Crawford. Updated for DDAS 6/2/2010
   T.Ahn. Updated for HabaNERO 12/8/2015
   Edits & Bug fixes for HABANERO: N. Rijal, 02/11/2019
   Edited for BGO: S. Ayoub, 08/06/2019
***************************************************************************/
#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

#include <config.h>

#include <Event.h> 
#include <EventProcessor.h>
#include <TCLAnalyzer.h>
#include <vector>
#include <histotypes.h>
#include <string.h>
#include <stdio.h>
#include <Analyzer.h>
#include <TCLHistogrammer.h>
#include <Spectrum2DW.h>
#include <Spectrum1DW.h>
#include <Spectrum.h>
#include <SpectrumFactory.h>
#include <Parameter.h>
#include <TreeParameter.h>

#include "SpecTcl_ddas.h"
#include "Unpacker_ddas.h"
#include "Parameters_ddas.h"

BGOarray BGO("BGOarray");

// Create  unpacker (see Unpacker.cpp)
static BGOUnpacker UnpackBGO;

void BGOSpecTcl::CreateAnalysisPipeline(CAnalyzer& rAnalyzer)  
{ 
  RegisterEventProcessor(UnpackBGO, "UnpackB");
}  


// Constructors, destructors and other replacements for compiler cannonicals:
BGOSpecTcl::BGOSpecTcl ()
{   
}


// Destructor:
BGOSpecTcl::~BGOSpecTcl ( )
{
} 


void BGOSpecTcl::BindTCLVariables(CTCLInterpreter& rInterp)  
{ 
  CTclGrammerApp::BindTCLVariables(rInterp);
}  


void BGOSpecTcl::SourceLimitScripts(CTCLInterpreter& rInterpreter)  
{ 
  CTclGrammerApp::SourceLimitScripts(rInterpreter);
}  




void BGOSpecTcl::SetLimits()  
{ 
  CTclGrammerApp::SetLimits();
}  



void BGOSpecTcl::CreateHistogrammer()  
{ 
  CTclGrammerApp::CreateHistogrammer();
}  



void BGOSpecTcl::SelectDisplayer(UInt_t nDisplaySize, CHistogrammer& rHistogrammer)  
{ CTclGrammerApp::SelectDisplayer(nDisplaySize, rHistogrammer);
}  



void 
BGOSpecTcl::SetupTestDataSource()  
{ CTclGrammerApp::SetupTestDataSource();
}  



void 
BGOSpecTcl::CreateAnalyzer(CEventSink* pSink)  
{ CTclGrammerApp::CreateAnalyzer(pSink);
}  


void 
BGOSpecTcl::SelectDecoder(CAnalyzer& rAnalyzer)  
{ 
  CTclGrammerApp::SelectDecoder(rAnalyzer);
}  


void 
BGOSpecTcl::AddCommands(CTCLInterpreter& rInterp)  
{ 
  CTclGrammerApp::AddCommands(rInterp);
  //AddServerCommands(this, rInterp);
}  

void 
BGOSpecTcl::SetupRunControl()  
{ 
  CTclGrammerApp::SetupRunControl();
}  


void 
BGOSpecTcl::SourceFunctionalScripts(CTCLInterpreter& rInterp)  
{ 
  CTclGrammerApp::SourceFunctionalScripts(rInterp);
}  

int BGOSpecTcl::operator()()  
{ 
  return CTclGrammerApp::operator()();
}

BGOSpecTcl   myApp;

#ifdef SPECTCL_5_INIT
CTclGrammerApp* CTclGrammerApp::m_pInstance = &myApp; 
CTCLApplication* gpTCLApplication;

#else
CTclGrammerApp& app(myApp);	 // Create an instance of me.
CTCLApplication* gpTCLApplication=&app;  // Findable by the Tcl/tk framework.
#endif
