////     File: Parameters_ddas.h
#ifndef __BGOPARAMETERS_H
#define __BGOPARAMETERS_H

#include <TreeParameter.h>

extern class BGOarray BGO;
extern class ioncounter IC;
extern class DdasDiagnostics ddasdiagnostics;
// Bit registers 

// define the total number of modules in the system
#define TOTMOD 1

//define the number of channels in a module
#define CHANPERMOD 16

#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif
//////////////////////////////// BGO /////////////////////////////////

class BGOarrayChanID
{
 private:
  BGOarray *top;
  
 public:
  CTreeParameter adc;
  CTreeParameter timehigh;
  CTreeParameter timelow;
  CTreeParameter timecfd;
  double time;
  CTreeParameter overflow;
  CTreeParameterArray esums;
  CTreeParameterArray qsums;

 public:
  void Initialize(BGOarray* top, STD(string) name);
  void Reset();
};
///////////////////////////////////////////////////////////////////////
class BGOarrayRaw
{
 private:
  BGOarray *top;
  
 public:
  BGOarrayChanID chanid[TOTMOD*CHANPERMOD];

  vector<int> chanidhit;

 public:
  void Initialize(BGOarray* top, STD(string) name);
  void Reset();
};
///////////////////////////////////////////////////////////////////////
class BGOarray
{
 public:
  
  CTreeParameterArray energy;
  CTreeParameterArray ecal;
  CTreeParameterArray time;
  CTreeParameterArray timecfd;
  CTreeParameterArray timelow;
  CTreeParameterArray timehigh;
  BGOarrayRaw raw;

 public:
  BGOarray(STD(string) name);
  void Reset();
};
///////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////
