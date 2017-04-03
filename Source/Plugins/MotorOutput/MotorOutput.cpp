
/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include <stdio.h>
#include "MotorOutput.h"

//If the processor uses a custom editor, it needs its header to instantiate it
//#include "MotorOutput.h"

MotorOutput::MotorOutput()
    : GenericProcessor("ApMo Motor Output") //, threshold(200.0), state(true)

{
	debugFile.open(debugPath);

	//Without a custom editor, generic parameter controls can be added
    //parameters.add(Parameter("thresh", 0.0, 500.0, 200.0, 0));
	
	// Open serial port
	comPort = "COM3";
	std::string portString = "\\\\.\\" + comPort;

	serialPort = CreateFile(portString.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	// Do some basic settings
	DCB serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);

	GetCommState(serialPort, &serialParams);
	serialParams.BaudRate = CBR_9600;
	serialParams.ByteSize = 8;
	serialParams.StopBits = 1;
	serialParams.Parity = NOPARITY;

	SetCommState(serialPort, &serialParams);

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 50;
	timeout.WriteTotalTimeoutConstant = 30;
	timeout.WriteTotalTimeoutMultiplier = 2;

	//SetCommTimeouts(serialPort, &timeout);

	// Initialize motor
	BOOL writeSuccess = WriteFile(serialPort, "CC3\r", 4, &byteswritten, NULL);
	writeSuccess = WriteFile(serialPort, "JS0.00001\r", 10, &byteswritten, NULL);
	writeSuccess = WriteFile(serialPort, "CJ\r", 3, &byteswritten, NULL);

	Sleep(10);
}

MotorOutput::~MotorOutput()
{
	debugFile.close();
	BOOL writeSuccess = WriteFile(serialPort, "SJ\r", 3, &byteswritten, NULL);
	CloseHandle(serialPort);
}

/**
	If the processor uses a custom editor, this method must be present.
*/
/*
AudioProcessorEditor* MotorOutput::createEditor()
{
	editor = new MotorOutputEditor(this, true);

	//std::cout << "Creating editor." << std::endl;

	return editor;
}
*/

void MotorOutput::setParameter(int parameterIndex, float newValue)
{

    //Parameter& p =  parameters.getReference(parameterIndex);
    //p.setValue(newValue, 0);

    //threshold = newValue;

    //std::cout << float(p[0]) << std::endl;
    editor->updateParameterButtons(parameterIndex);
}

void MotorOutput::process(AudioSampleBuffer& buffer,
                               MidiBuffer& events)
{
	/**
	Generic structure for processing buffer data 
	*/
	int nChannels = buffer.getNumChannels();

	int nSamplesInput = getNumSamples(0);
	const float* pRead;

	pRead = buffer.getReadPointer(0, 0);
	debugFile << std::to_string(*pRead) << " ";
	float speed = *pRead;

	pRead = buffer.getReadPointer(96, 0);
	nSamplesInput = getNumSamples(96);

	for (int i = 0; i < nSamplesInput; i++)
	{
		debugFile << std::to_string(*(pRead + i)) << " ";
	}

	std::string output = "CS";
	output += std::to_string(speed) + "\r";

	BOOL writeSuccess = WriteFile(serialPort, output.c_str(), output.size(), &byteswritten, NULL);

	debugFile << "\n";
		/**
		Do something here.
		
		To obtain a read-only pointer to the n sample of a channel:
		float* samplePtr = buffer.getReadPointer(chan,n);
		
		To obtain a read-write pointer to the n sample of a channel:
		float* samplePtr = buffer.getWritePointer(chan,n);

		All the samples in a channel are consecutive, so this example is valid:
		float* samplePtr = buffer.getWritePointer(chan,0);
		for (i=0; i < nSamples; i++)
		{
			*(samplePtr+i) = (*samplePtr+i)+offset;
		}
		
		See also documentation and examples for buffer.copyFrom and buffer.addFrom to operate on entire channels at once.

		To add a TTL event generated on the n-th sample:
		addEvents(events, TTL, n);

		
		*/

	/** Simple example that creates an event when the first channel goes under a negative threshold

    for (int i = 0; i < getNumSamples(channels[0]->sourceNodeId); i++)
    {
        if ((*buffer.getReadPointer(0, i) < -threshold) && !state)
        {
    
	        // generate midi event
            addEvent(events, TTL, i);
    
	        state = true;
    
	    } else if ((*buffer.getReadPointer(0, i) > -threshold + bufferZone)  && state)
        {
            state = false;
        }
    
	}
	*/


}
