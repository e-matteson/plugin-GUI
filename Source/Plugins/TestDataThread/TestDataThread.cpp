#include "TestDataThread.h"
#include "TestDataThreadEditor.h"

// We can optionally override run() to do some initialization routine
//  before updateBuffer() starts running in a loop
void TestDataThread::run()
{
  // TODO initialize the NSP here? or do that in startAcquisition()?

  // Call the base class's run
  DataThread::run();
}


/** Fills the DataBuffer with incoming data. This is the most important
    method for each DataThread.*/
bool TestDataThread::updateBuffer()
{


}

  /** Returns true if the data source is connected, false otherwise.*/
bool TestDataThread::foundInputSource()
{
}

  /** Initializes data transfer.*/
bool TestDataThread::startAcquisition()
{
  std::cout << "TestDataThread starting acquisition." << std::endl;
  startThread();
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
}

  /** Returns the number of continuous headstage channels the data source can provide.*/
int TestDataThread::getNumHeadstageOutputs()
{
}

  /** Returns the number of continuous aux channels the data source can provide.*/
int TestDataThread::getNumAuxOutputs()
{
}

  /** Returns the number of continuous ADC channels the data source can provide.*/
int TestDataThread::getNumAdcOutputs()
{
}

  /** Returns the sample rate of the data source.*/
float TestDataThread::getSampleRate()
{
}

  /** Returns the volts per bit of the data source.*/
float TestDataThread::getBitVolts(Channel* chan)
{
}

  /** Returns the number of event channels of the data source.*/
int TestDataThread::getNumEventChannels()
{
}

  /** Notifies if the device is ready for acquisition */
bool TestDataThread::isReady()
{
}

int TestDataThread::modifyChannelName(int channel, String newName)
{
}

int TestDataThread::modifyChannelGain(int channel, float gain)
{
}


void TestDataThread::getEventChannelNames(StringArray& names)
{
}

bool TestDataThread::usesCustomNames()
{
}


  /** Create the DataThread custom editor, if any*/
GenericEditor* TestDataThread::createEditor(SourceNode* sn)
{
}


void TestDataThread::setDefaultChannelNames()
{
}
