#include "NSPSource.h"
#include "NSPSourceEditor.h"


NSPSource::NSPSource(SourceNode* sn) : DataThread(sn)
{
  looptime = 33;
  num_channels = 8;
  samplecounter = 0;
  dataBuffer = new DataBuffer(2, 10000);

  m_blkrckInstance = 0;
  cbSdkConnection con = cbSdkConnection();
  cbSdkConnectionType conType = CBSDKCONNECTION_DEFAULT;
  ResNSP = cbSdkOpen(m_blkrckInstance, conType, con);
}

NSPSource::~NSPSource()
{
	ResNSP = cbSdkClose(m_blkrckInstance);
}

// We could optionally override run() to do some initialization routine
//  before updateBuffer() starts running in a loop
void NSPSource::run()
{
  // TODO initialize the NSP here? or do that in startAcquisition()?

  // Call the base class's run
  DataThread::run();

}


/** Fills the DataBuffer with incoming data. This is the most important
    method for each DataThread.*/
bool NSPSource::updateBuffer()
{

	UINT32 cbtime1 = 0;
	UINT32 cbtime2 = 0;
	UINT32 prev_time;

	// PULL DATA
	prev_time = cbtime1;
	cbtime2 = cbtime1;
	while (cbtime2 == cbtime1)		//wait until new data is ready, it is approx 10 ms
	{
		cbtime2 = m_blackrock->getTime();
		Sleep(1);
	}

	m_blackrock->get_spikes(m_spikesInt32, m_spikesDouble, m_rasterHolder);
	cbtime1 = m_blackrock->getTime();
	cbSdkResult resTest2;

	return true;

}

  /** Returns true if the data source is connected, false otherwise.*/
bool NSPSource::foundInputSource()
{
	return true;
}

  /** Initializes data transfer.*/
bool NSPSource::startAcquisition()
{
  std::cout << "NSPSource starting acquisition." << std::endl;

  UINT16 uBegChan = 0;
  UINT32 uBegMask = 0;
  UINT32 uBegVal = 0;
  UINT16 uEndChan = 0;
  UINT32 uEndMask = 0;
  UINT32 uEndVal = 0;
  bool   bDouble = false;
  bool   bAbsolute = false;
  UINT32 uWaveforms = 0;
  UINT32 uConts = cbSdk_CONTINUOUS_DATA_SAMPLES;
  UINT32 uEvents = 0;
  UINT32 uComments = 0;
  UINT32 uTrackings = 0;
  UINT32 bWithinTrial = false;
  UINT32 bActive = 1; // 0 leave buffer intact, 1 clear the buffer

  ResNSP = cbSdkSetTrialConfig(m_blkrckInstance
	  , bActive
	  , uBegChan
	  , uBegMask
	  , uBegVal
	  , uEndChan
	  , uEndMask
	  , uEndVal
	  , bDouble
	  , uWaveforms
	  , uConts
	  , uEvents
	  , uComments
	  , uTrackings
	  , bAbsolute);

  ResNSP = cbSdkInitTrialData(m_blkrckInstance, NULL, &m_trialCont, NULL, NULL);

  startThread();

  return true;
}

  /** Stops data transfer.*/
bool NSPSource::stopAcquisition()
{
  std::cout << "NSPSource stopping acquisition." << std::endl;

  if (isThreadRunning())
    {
      signalThreadShouldExit();

    }

  if (waitForThreadToExit(500))
    {
      std::cout << "Thread exited." << std::endl;
    }
  else
    {
      std::cout << "Thread failed to exit, continuing anyway..." << std::endl;
    }

  return true;
}


  /** Returns the number of continuous headstage channels the data source can provide.*/
int NSPSource::getNumHeadstageOutputs()
{
	return 0;
}

  /** Returns the number of continuous aux channels the data source can provide.*/
int NSPSource::getNumAuxOutputs()
{
	return 0;
}

  /** Returns the number of continuous ADC channels the data source can provide.*/
int NSPSource::getNumAdcOutputs()
{
	return 1;
}

  /** Returns the sample rate of the data source.*/
float NSPSource::getSampleRate()
{
  return 30000;
	// return 1000000.0 / looptime;
}

  /** Returns the volts per bit of the data source.*/
float NSPSource::getBitVolts(Channel* chan)
{
	return 1;
}

  /** Returns the number of event channels of the data source.*/
int NSPSource::getNumEventChannels()
{
	return 0;
}

  /** Notifies if the device is ready for acquisition */
bool NSPSource::isReady()
{
	return true;
}

int NSPSource::modifyChannelName(int channel, String newName)
{
  // Do something with ChannelCustomInfo, eventually
  return 0;
}

void NSPSource::getEventChannelNames(StringArray& names)
{
  names.clear();
  for (int k = 0; k < num_channels; k++)
    {
      names.add("TEST"+String(k+1));
    }
}

int NSPSource::modifyChannelGain(int channel, float gain)
{
  return 0;
}



bool NSPSource::usesCustomNames()
{
  return false;
}


  /** Create the DataThread custom editor, if any*/
//GenericEditor* NSPSource::createEditor(SourceNode* sn)
//{
//}


void NSPSource::setDefaultChannelNames()
{
}
