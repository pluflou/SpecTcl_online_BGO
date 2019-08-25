/*************************************************************************** 
   SpecTcl BCS Eventprocessor --> UNPACKER for Digital Data Acquisition
   Based on standard BCS SpecTcl codes
   S.N.Liddick/H.L.Crawford. Updated for DDAS 6/2/2010
   Edits & Bug fixes for HABANERO: N. Rijal, 02/11/2019
   Edits for BGOs: S. Ayoub, 08/06/2019
***************************************************************************/
#ifndef __BGOUNPACKER_H
#define __BGOUNPACKER_H

#ifndef __EVENTPROCESSOR_H
#include <EventProcessor.h>
#endif

#ifndef __TRANSLATORPOINTER_H
#include <TranslatorPointer.h>
#endif

#ifndef __TCLANALYZER_H
#include <TCLAnalyzer.h>
#endif

#include <vector>
#include "SpecTcl.h"
#include "ddaschannel.h"
using namespace std;

/* Define a class known only to Unpacker that can accumulate 
   ddas channels sorted into true events. */
////////////////////////////////////////////////////////////////////////////////////////////////
class BGOUnpacker : public  CEventProcessor{
  
 public:
  vector<ddaschannel*>  channellist;               //!< collection of channels within one event
  vector<ddaschannel*>::iterator channellist_it;   //!< iterator over channels in an event
  vector<ddaschannel*>  previousevent;             //!< obsolete
  vector<ddaschannel*>::iterator previousevent_it; //!< obsolete
 
  unsigned long m_currenttime_low;                 //!< portion of timestamp from current event
  unsigned long m_currenttime_high;                //!< portion of timestamp from current event
  
  unsigned long m_lasttime_low;                    //!< portion of timestamp from last event
  unsigned long m_lasttime_high;                   //!< portion of timestamp from last event

  unsigned long numchans, timeskip;                //!< number of channels in an event

  //  unsigned int eventbuilt;                             //!< flag, set to one to indicate data was
                                                   //!< built by an external program
  ddaschannel *chan;                               //!< pointer to a channel of ddas data
  UInt_t m_timewindow;                             //!< user defined channel coincidence window for event
  unsigned long fragcount1;
  unsigned long fragcount2;
  SpecTcl *mylocalspec;                            //!< obsolete
  CSpectrum *swaveform;                            //!< force spectcl to display traces

  virtual Bool_t operator()(const Address_t pEvent, CEvent& rEvent,
			    CAnalyzer& rAnalyzer, CBufferDecoder& rDecoder);
  
  virtual Bool_t OnAttach(CAnalyzer& rAnalyzer);
  virtual Bool_t OnBegin(CAnalyzer& rAnalyzer,CBufferDecoder& rDecoder);
  virtual Bool_t OnEnd(CAnalyzer& rAnalyzer,CBufferDecoder& rDecoder);
  
 protected:
  void ResetChannelList();
  void ResetPreviousEvent();
  void ddastospectcl(vector<ddaschannel*> &, CEvent &rEvent);

};
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////

