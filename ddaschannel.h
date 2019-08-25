/* ddaschannel class definition header file */

#ifndef DDASCHANNEL_H
#define DDASCHANNEL_H

#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

#ifndef __TRANSLATORPOINTER_H 
#include <TranslatorPointer.h>
#endif 

// define bit masks to extract data from specific locations
// of pixie16 data
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

#define BIT31MASK          0x80000000  // Bit 31 
#define BIT30MASK          0x40000000  // Bit 30 
#define BIT30to29MASK      0x60000000  // Bits 30 through 29
#define BIT31to29MASK      0xE0000000  // Bits 31 through 29
#define BIT30to16MASK      0x7FFF0000  // Bits 30 through 16
#define BIT29to16MASK      0x3FFF0000  // Bits 29 through 16
#define BIT28to16MASK      0x1FFF0000  // Bits 28 through 16

// number of words added to pixie16 channel event header when energy sums
// and baselines are recorded
#define SIZEOFESUMS                 4
// number of words added to pixie16 channel event header when QDC sums
// are recorded
#define SIZEOFQDCSUMS               8


using namespace std;

/* Define a class that can accumulate ddas channels
   to be sorted into true events. */

class ddaschannel  {
public:

  /********** Variables **********/

  // ordering is important with regards to access and file size.  Should
  // always try to maintain order from largest to smallest data type
  // Double_t, Int_t, Short_t, Bool_t, pointers

  /* Channel events always have the following info. */
  double time;              //< assembled time including cfd
  double coarsetime;        //< assembled time without cfd
  double cfd;               //< cfd time only

  uint  energy;              //< energy of event
  uint  timehigh;            //< portion of timestamp
  uint  timelow;             //< portion of timestamp
  uint  timecfd;             //< portion of timestamp

  int channelnum;           //< obsolete
  int finishcode;           //< indicate whether event is 
  int channellength;        //<
  int channelheaderlength;
  int overflowcode;
  int chanid;
  int slotid;
  int crateid;
  int id;

  int cfdtrigsourcebit; //Added back in CP
  int cfdfailbit; //Added CP

  int tracelength;

  int ModMSPS;                //< Sampling rate of the module (MSPS)

  /* A channel may have extra information... */
  vector<uint > energySums;
  vector<ushort> qdcSums;
  
  /* A waveform (trace) may be stored too. */
  vector<ushort> trace;

  /********** Functions **********/
  ddaschannel();
  ddaschannel(const ddaschannel& obj);
  ddaschannel& operator=(const ddaschannel& obj);
  ~ddaschannel();
  void Reset();

  uint  GetEnergy() {return energy;}
  uint  GetTimeHigh() {return timehigh;}
  uint  GetTimeLow() {return timelow;}
  uint  GetCFDTime() {return timecfd;}
  double GetTime() {return time;}
  double GetCoarseTime() {return coarsetime;}
  double GetCFD() {return cfd;}
  uint  GetEnergySums(int i) {return energySums[i];}
  int GetChannelNum() {return channelnum;}
  int GetFinishCode() {return finishcode;}
  int GetChannelLength() {return channellength;}
  int GetChannelLengthHeader() {return channelheaderlength;}
  int GetOverflowCode() {return overflowcode;}
  int GetSlotID() {return slotid;}
  int GetCrateID() {return crateid;}
  int GetChannelID() {return chanid;}
  int GetID() {return id;}
  int GetModMSPS() {return ModMSPS;}

  vector<ushort> GetTrace() {return trace;}

  void SetChannelID(uint  data);
  void SetSlotID(uint  data);
  void SetCrateID(uint  data);
  void SetChannelHeaderLength(uint  data);
  void SetChannelLength(uint  data);
  void SetOverflowCode(uint  data);
  void SetFinishCode(uint  data);
  void SetID(uint  data);

  void SetTimeLow(uint  data);
  void SetTimeHigh(uint  data);
  void SetTimeCFD(uint  data);
  void SetTime();
  void SetCoarseTime();
  void SetEnergy(uint  data);
  void SetTraceLength(uint  data);

  void SetModMSPS(uint  data);

  void SetEnergySums(uint  data);
  void SetQDCSums(uint  data);
  void SetTraceValues(uint  data);

  //void UnpackChannelData(const uint32_t *data);
  void UnpackChannelData(TranslatorPointer<uint32_t>& );

};

#endif
