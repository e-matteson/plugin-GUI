
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
#include "MotorVelocityDecoder.h"
#include <direct.h>  
#include <stdlib.h>  
#include <stdio.h>  

//If the processor uses a custom editor, it needs its header to instantiate it
//#include "MotorVelocityDecoderEditor.h"

MotorVelocityDecoder::MotorVelocityDecoder()
    : GenericProcessor("Motor Velocity Decoder") //, threshold(200.0), state(true)

{
	//Without a custom editor, generic parameter controls can be added
    //parameters.add(Parameter("thresh", 0.0, 500.0, 200.0, 0));

	parameterFile = fopen(parameterFilename.c_str(), "r");

	parameters = new double[96*(history+1)];
	histBuffer = new double*[history + 1];
	for (int i = 0; i < (history + 1); i++)
	{
		histBuffer[i] = new double[96]();
	}

	for (int i = 0; i < 1056; i++)
	{
		fscanf(parameterFile, "%le", &parameters[i]);
	}
	
	fclose(parameterFile);

	for (int iHist = 0; iHist < 11; iHist++)
	{
		for (int chan = 0; chan < 96; chan++)
		{
			histBuffer[iHist][chan] = 0;
		}
	}

	iBuffer = 0;
}

MotorVelocityDecoder::~MotorVelocityDecoder()
{
	delete[] parameters;

	for (int i = 0; i < (history+1); ++i) {
		delete[] histBuffer[i];
	}
	delete[] histBuffer;
}

/**
	If the processor uses a custom editor, this method must be present.
*/
/*
AudioProcessorEditor* MotorVelocityDecoder::createEditor()
{
	editor = new MotorVelocityDecoderEditor(this, true);

	//std::cout << "Creating editor." << std::endl;

	return editor;
}
*/

void MotorVelocityDecoder::setParameter(int parameterIndex, float newValue)
{

    //Parameter& p =  parameters.getReference(parameterIndex);
    //p.setValue(newValue, 0);

    //threshold = newValue;

    //std::cout << float(p[0]) << std::endl;
    editor->updateParameterButtons(parameterIndex);
}

void MotorVelocityDecoder::process(AudioSampleBuffer& buffer,
                               MidiBuffer& events)
{
	int nChannels = buffer.getNumChannels();

	float angularVelocity = 0;
	int nSamples = getNumSamples(0);

	for (int chan = 0; chan < 96; chan++)
	{

		assert(nSamples == getNumSamples(chan));

		const float* readPtr = buffer.getReadPointer(chan, 0);

		int iCurrentHist = iBuffer;
		for (int iSample = 0; iSample < nSamples; iSample++)
		{
			histBuffer[iCurrentHist][chan] = *readPtr;

			if (iCurrentHist == 9)
			{
				iCurrentHist = 0;
			}
			else {
				iCurrentHist++;
			}
		}

		if (chan == 95)
		{
			iBuffer = iCurrentHist;
		}
	}

	int buffIndex = 0;
	int histind = 0;
	int currentind = iBuffer;
	while (histind<11)
	{
		for (int chan = 0; chan < 96; chan++)
		{
			angularVelocity += histBuffer[currentind][chan] * parameters[buffIndex];
			buffIndex++;
		}

		if (currentind == 9)
		{
			currentind = 0;
		}
		else {
			currentind++;
		}

		histind++;
	}

	float* writePtr = buffer.getWritePointer(0, 0);
	
	if (isnan(angularVelocity))
	{
		angularVelocity = 0;
	}
	else if (angularVelocity > 10)
	{
		angularVelocity = 0;
	}
	else if (angularVelocity < -10)
	{
		angularVelocity = 0;
	}

	for (int i = 0; i < nSamples; i++)
	{
		*(writePtr + i) = angularVelocity;
	}

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
