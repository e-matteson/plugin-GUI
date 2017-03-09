#include "TestDataThread.h"
#include "TestDataThreadEditor.h"


TestDataThread::TestDataThread(SourceNode* sn) : DataThread(sn)
{
  // stores the number of samples written to the buffer
  sampleCounter = 0;

  // how long to sleep between writes - controls actual sample rate
  sleepMicrosecs = 100;
  reportedSampleRate = 10000;

  numChannels = 1;
  dataBuffer = new DataBuffer(numChannels, 1000000); // what should the size be?
  // dataBuffer = new DataBuffer(numChannels, 10000); // what should the size be?
}

TestDataThread::~TestDataThread()
{
}


/** Fills the DataBuffer with incoming data. This is the most important
    method for each DataThread.*/
bool TestDataThread::updateBuffer()
{
	uint64 eventCode = 0;

  // make the period of the sine wave scale with the reported sample rate
	float thisSample = sin(sampleCounter/reportedSampleRate);

	dataBuffer->addToBuffer(&thisSample, &sampleCounter, &eventCode, 1);
	sampleCounter++;

  // sleep, to reduce the actual sample rate
	std::this_thread::sleep_for(std::chrono::microseconds(sleepMicrosecs));

  // or just yield, to write samples as quickly as possible
	// std::this_thread::yield();

	return true;
}

/** Returns the sample rate of the data source.*/
float TestDataThread::getSampleRate()
{
  return reportedSampleRate;
	// return 1000000.0 / sleepMicrosecs;
}

  /** Returns true if the data source is connected, false otherwise.*/
bool TestDataThread::foundInputSource()
{
	return true;
}

  /** Initializes data transfer.*/
bool TestDataThread::startAcquisition()
{
  std::cout << "TestDataThread starting acquisition." << std::endl;
  startThread();

  return true;
}

  /** Stops data transfer.*/
bool TestDataThread::stopAcquisition()
{
  std::cout << "TestDataThread stopping acquisition." << std::endl;

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
int TestDataThread::getNumHeadstageOutputs()
{
	return 0;
}

  /** Returns the number of continuous aux channels the data source can provide.*/
int TestDataThread::getNumAuxOutputs()
{
	return 0;
}

  /** Returns the number of continuous ADC channels the data source can provide.*/
int TestDataThread::getNumAdcOutputs()
{
	return 1;
}


  /** Returns the volts per bit of the data source.*/
float TestDataThread::getBitVolts(Channel* chan)
{
	return 1;
}

  /** Returns the number of event channels of the data source.*/
int TestDataThread::getNumEventChannels()
{
	return 0;
}

  /** Notifies if the device is ready for acquisition */
bool TestDataThread::isReady()
{
	return true;
}

int TestDataThread::modifyChannelName(int channel, String newName)
{
  // Do something with ChannelCustomInfo, eventually
  return 0;
}

void TestDataThread::getEventChannelNames(StringArray& names)
{
  names.clear();
  for (int k = 0; k < numChannels; k++)
    {
      names.add("TEST"+String(k+1));
    }
}

int TestDataThread::modifyChannelGain(int channel, float gain)
{
  return 0;
}



bool TestDataThread::usesCustomNames()
{
  return false;
}


  /** Create the DataThread custom editor, if any*/
//GenericEditor* TestDataThread::createEditor(SourceNode* sn)
//{
//}


void TestDataThread::setDefaultChannelNames()
{
}
