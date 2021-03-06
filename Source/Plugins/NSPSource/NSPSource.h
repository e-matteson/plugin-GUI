// TODO Should the include guard have some string appended to it?
#ifndef __TESTDATATHREAD_H
#define __TESTDATATHREAD_H


#include <DataThreadHeaders.h>

#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <math.h>
#include "cbsdk.h"
#include "Blackrock.h"
#include <iostream>
#include <fstream>

class NSPSource : public DataThread

{
public:

  // TODO should the constructor do anything more specific?
  NSPSource(SourceNode* sn);
  ~NSPSource();

  void run(); // this isn't virtual, we can use the parent's if we want


  /** Fills the DataBuffer with incoming data. This is the most important
      method for each DataThread.*/
  bool updateBuffer();


  /** Returns true if the data source is connected, false otherwise.*/
  bool foundInputSource();

  /** Initializes data transfer.*/
  bool startAcquisition();

  /** Stops data transfer.*/
  bool stopAcquisition();

  /** Returns the number of continuous headstage channels the data source can provide.*/
  int getNumHeadstageOutputs();

  /** Returns the number of continuous aux channels the data source can provide.*/
  int getNumAuxOutputs();

  /** Returns the number of continuous ADC channels the data source can provide.*/
  int getNumAdcOutputs();

  /** Returns the sample rate of the data source.*/
  float getSampleRate();

  /** Returns the volts per bit of the data source.*/
  float getBitVolts(Channel* chan);

  /** Returns the number of event channels of the data source.*/
  int getNumEventChannels();

  /** Notifies if the device is ready for acquisition */
  bool isReady();

  int modifyChannelName(int channel, String newName);

  int modifyChannelGain(int channel, float gain);

  /*  virtual void getChannelsInfo(StringArray &Names, Array<ChannelType> &type, Array<int> &stream, Array<int> &originalChannelNumber, Array<float> &gains)
      {
      }*/

  void getEventChannelNames(StringArray& names);

  bool usesCustomNames();


  /** Create the DataThread custom editor, if any*/
 /* GenericEditor* createEditor(SourceNode* sn);*/

private:

  void setDefaultChannelNames();

  UINT32*** m_spikesInt32;
  double*** m_spikesDouble;
  double* m_rasterHolder;
  CBlackrock* m_blackrock;
  static const int m_noOfNeuroCh = 96;
  double** rasterWindow;
  static const int Buffer_length = 15;
  double Raster_Time[Buffer_length];
  static const int m_Fs = 30000;
  float firingRates[m_noOfNeuroCh+1];
  std::ofstream debugFile;
  std::string debugPath = "NSP_debug.txt";
  int i_win;
  int loopCount;
  INT64 cbtime1;
  int64 elapsedTime;

};

#endif
