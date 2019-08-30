////////////////////////////////////////////////////////////////////
//Edits & Bug fixes for HABANERO: N. Rijal, 02/11/2019
//Edits for BGOs: S. Ayoub, 08/06/2019
////////////////////////////////////////////////////////////////////
#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

#include <config.h>
#include <Event.h> 
#include <stdint.h>
#include <iomanip>
#include <string>
#include <math.h>

#include "Parameters_ddas.h"
#include "Unpacker_ddas.h" 

#define BDECAY_PACKET 0x7100

#define BdecayID BDECAY_PACKET

#define ERROR_CHAN 0

#define CHANNELIDMASK             0xF  // Bits 0-3 inclusive
#define SLOTIDMASK               0xF0  // Bits 4-7 inclusive
#define CRATEIDMASK             0xF00  // Bits 8-11 inclusive

#define HEADERLENGTHMASK      0x1F000  // Bits 12-16 inclusive
#define CHANNELLENGTHMASK  0x3FFE0000  // Bits 17-29 inclusive
#define OVERFLOWMASK       0x40000000  // Bit 30 has overflow information (1 - overflow)
#define FINISHCODEMASK     0x80000000  // Bit 31 has pileup information (1 - pileup)

#define LOWER16BITMASK         0xFFFF  // Lower 16 bits
#define UPPER16BITMASK     0xFFFF0000  // Upper 16 bits
#define LOWER12BITMASK          0xFFF  // Lower 12 bits

// number of words added to pixie16 channel event header when energy sums and baselines are recorded
#define SIZEOFESUMS                 4
// number of words added to pixie16 channel event header when QDC sums
// are recorded
#define SIZEOFQDCSUMS               8

// (09/27/2010) DTM - reset last timestamp when something new is attached
int countingevt;
int badevt;
//////////////////////////////////////////////////////////////////////////////////////////////
Bool_t BGOUnpacker::OnAttach(CAnalyzer& rAnalyzer){
  cout << "attaching event processor to spectcl and zeroing last timestamp." << endl;
  m_lasttime_low = m_lasttime_high = 0;
  countingevt = 0; //zeroing evt count
  badevt = 0; //zeroing bad evt count 
  return kfTRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
Bool_t BGOUnpacker::OnBegin(CAnalyzer& rAnalyzer,CBufferDecoder& rDecoder){

  cout << " SpecTcl found data, on begin function " << endl;

  m_currenttime_low = 0;
  m_currenttime_high = 0;
  
  m_lasttime_low = 0;
  m_lasttime_high = 0;
  countingevt = 0;
  badevt = 0;
  m_timewindow = 200000; // Coincidence window in Pixie16 (100 MHz) clock ticks, 
                       // this parameter is defined twice

  //retrieve singleton spectcl
  mylocalspec->getInstance(); 
  numchans = 0;
  timeskip = 0;
  fragcount1 = 0;
  fragcount2 = 0;
  //eventbuilt = 1;
  return kfTRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void BGOUnpacker::ResetChannelList(){
  // cout << "Reseting channel list. Size = " << channellist.size() << endl;

  for(channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) {
    delete *channellist_it;
  }
  channellist.clear();
}
//////////////////////////////////////////////////////////////////////////////////////////////
Bool_t BGOUnpacker::operator()(const Address_t pEvent,
				    CEvent&         rEvent,
				    CAnalyzer&      rAnalyzer,
				    CBufferDecoder& rDecoder){
  //must check to see if total number of modules in system has been set
  //if this calculation is performed in the OnBegin method then SpecTcl MUST be started before 
  //readout commences so that SpecTcl can catch a begin run buffer
  //if this calculation is performed in the OnAttach method the initialization tcl scripts have
  //not been run so that the required varaibles are not set.

  int sourceid, sourceid1;
  m_timewindow = 200000;

  /* Translates pointer pEvent pointing to raw data into pointer p */
  TranslatorPointer<uint32_t> p(*(rDecoder.getBufferTranslator()), pEvent);
  CTclAnalyzer& rAna((CTclAnalyzer&)rAnalyzer);

  uint32_t totwords;
  totwords = *p/2;//totwords = (*(--p))/2; //converts total #bytes "words" into total number (inclusive) of 16-bits in event
  //cout<<"==Unpacking started "<<"  Event size, hex = "<<hex<<*p<<"  dec= "<<dec<<*p<< " (bytes) == "<<*p/2<<" words"<<endl;
 
  rAna.SetEventSize(/*nWords*/totwords*sizeof(uint16_t)); /*Set event size*/
  //cout << "tot words in the event = " << totwords << endl;
 
  ++p; //skip the total event size
  totwords = totwords - 2;

  while(totwords !=0){
    ////////////////////////////////////////////////////////////////
    p+=2;//skip 64-bit event builder fragment timestamp
    totwords = totwords - 4;

    sourceid = (*p & LOWER16BITMASK); //this gets the data source ID
    //cout<<"data sourceid = "<<hex<<sourceid<<endl;
    ++p; //skip source id
    totwords = totwords - 2;//2 subtract off source id

    ++p;//skip fragment payload (self exclusive)
    totwords = totwords - 2;//2 subtract off payload
     /////////////////////////////////////////////////////////////////
     
    ++p;//skip the barrier or ring-item header
    totwords = totwords - 2; //subtract off barrier or ring-item header
    
    p+=3;//skip ring item size, type and size of the body header
    totwords = totwords - 6; // subtract off ring item size, type and size of the body header
 
    p+=2;
    totwords = totwords - 4;// subtract offf 64-bit fragment timestamp
    
    sourceid1 = (*p & LOWER16BITMASK); //this gets the fragment source ID
    //cout<<"fragment sourceid1 = "<<hex<<sourceid1<<endl;
    ++p; //skip source id1
    totwords = totwords - 2;// subtract off source id1
    
    ++p;//skip fragment body header/ barrier (?)
    totwords = totwords - 2;// subtract off fragment body header/ barrier (?)
    /////////////////////////////////////////////////////////////////////////
    
    //cout<<" I am the size of the fragment "<<dec<<*p<<"  words"<<endl;
    uint32_t ChannelWordsCount = *p;//++; /* Word counter in this item*/
    //cout<<"  ChannelWordsCount  =  "<<ChannelWordsCount<<endl;
   

    // should in principle stick new stuff here
    chan = new ddaschannel();
    // Unpack the channel data into a vector until an event is accumulated
    chan->UnpackChannelData(p);

    totwords = totwords - ChannelWordsCount; //advance counter to new spot

    //////////////////////////////////////////////////////////////////////////
    // determine global channel number of this channel
    // number of channels in previous crate (if any)
    chan->id = 0;  
    
    chan->id += chan->chanid;
    //cout << "2: slotid =  " << chan->slotid <<"  channel_id = "<<chan->id<<endl;
    
    chan->id = (chan->id +16*((chan->slotid) -2));
    //cout << "3: slotid =  " << chan->slotid <<"  channel_id = "<<chan->id<<endl;
   
    //    cout << "2: " << chan->id << endl;
    m_currenttime_low = chan->timelow;
    m_currenttime_high = chan->timehigh;
    
    numchans++;
    
    //if events have already been built by NSCL DAQ just send for processing and 
    // clear vector.  If not don't do anything, it'll be handled by code below.
    
    ////if(nerov.ddas.evtbuilt && chan->slotid > 1){
      if(chan->slotid > 1){
      channellist.push_back(chan);
    }
  } // end while loop over event size.
  
  
    ddastospectcl(channellist, rEvent);
    ResetChannelList();
   
  return kfTRUE;  
}
/******************************************************************/
/******************** DDAS UNPACKING FUNCTION *********************/
/******************************************************************/

void BGOUnpacker::ddastospectcl(vector<ddaschannel *> &channellist, CEvent &rEvent){
  Int_t eventsize = channellist.size();
  //ddasdiagnostics.cmult = eventsize;

  double starttime_low, endtime_low;
  double starttime_high, endtime_high;
    
  channellist_it = channellist.begin();
  //cout<<"new event"<<endl; 
  //BGO.raw.chanidhit.clear();

  for (channellist_it = channellist.begin(); 
       channellist_it < channellist.end(); channellist_it++) {
    /* The time of an event will be taken as the time of the first 
       channel in the event */
    if(channellist_it == channellist.begin()){
	starttime_low = (*channellist_it)->timelow;
	starttime_high = (*channellist_it)->timehigh;
    }

    if ((*channellist_it)->id <16){    
      
      //Unpack the data according to the channel id number
      BGO.raw.chanidhit.push_back((*channellist_it)->id);
      //cout<<(*channellist_it)->id<<" "<<(*channellist_it)->energy<<endl;

      BGO.raw.chanid[(*channellist_it)->id].adc       = ((*channellist_it)->energy);

      BGO.raw.chanid[(*channellist_it)->id].timehigh  = ((*channellist_it)->timehigh);

      BGO.raw.chanid[(*channellist_it)->id].timelow   = ((*channellist_it)->timelow);

      BGO.raw.chanid[(*channellist_it)->id].timecfd   = ((*channellist_it)->timecfd);

      BGO.raw.chanid[(*channellist_it)->id].time      = ((*channellist_it)->time);
      BGO.raw.chanid[(*channellist_it)->id].overflow      = ((*channellist_it)->overflowcode);

      endtime_low = (*channellist_it)->timelow;
      endtime_high = (*channellist_it)->timehigh;
    }
    
    if ((*channellist_it)->id >= 16){
      //cout << "id=" << (*channellist_it)->id <<  "energy = " <<  ((*channellist_it)->energy) << endl;
      endtime_low = (*channellist_it)->timelow;
      endtime_high = (*channellist_it)->timehigh;
    }
    ////ddasdiagnostics.eventlength = (endtime_low - starttime_low) * 10;
  }
}
// Just printout something when an end of run is seen:
//////////////////////////////////////////////////////////////////////////////////////////////
Bool_t BGOUnpacker::OnEnd(CAnalyzer& rAnalyzer,CBufferDecoder& rDecoder){
  //cout << "Number of events = " << dec << countingevt << " and number of bad events = " << dec << badevt << "." << endl;
  cerr << "End of run encountered on data source\n";\
    return kfTRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
