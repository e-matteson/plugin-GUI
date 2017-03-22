==================================
Welcome to the Open Ephys project!
==================================

This GUI was designed to provide a fast and flexible interface for acquiring and visualizing data from extracellular electrodes.

This version of the GUI is based around a *true plugin architecture*, meaning the data processing modules are compiled separately from the main application. This should simplify the process of adding new functionality, since new modules can be created without needing to re-compile the entire application.

Everything is written in C++, with the help of the Juce_ library. Juce is lightweight, easy to learn, and completely cross-platform. What's more, it includes a variety of classes for audio processing, which have been co-opted to process neural data. It might be necessary to create custom data processing classes in the future, but for now, Juce takes care of a lot of the messy bits involved in analyzing many parallel data streams.

Installation instructions specific for the plugin GUI can be found here: https://open-ephys.atlassian.net/wiki/display/OEW/Plugin+architecture

The two most important branches are **master**, for stable releases of tested versions, and **development** for newer, less tested versions (pull request belong here). If you don't need the latest features use the master branch or, equivalently, download the binaries.

We recommend reading through the wiki_ before attempting to make any changes. For pull requests look at our guidelines regarding the `different branches`_.

If you want to add files to the core GUI, you'll have to do that through the "Introjucer," using the "open-ephys.jucer" file. The Introjucer makefiles are located in the JuceLibraryCode/Introjucer/Builds folder, or as part of the Juce source_.

New plugin developers can publish links to their work in `this list`_ to make them available to the general public.

.. _source: https://github.com/julianstorer/juce
.. _JUCE: http://www.rawmaterialsoftware.com/juce.php
.. _wiki: http://open-ephys.atlassian.net
.. _different branches: https://open-ephys.atlassian.net/wiki/display/OEW/Using+Git
.. _this list: https://open-ephys.atlassian.net/wiki/display/OEW/Third-party+plugin+repositories

==================================
Borton Lab Notes
==================================

Figuring out program structure
----------------------------

- anything that's a ChangeListener gets updates from the GUI
- MainWindow makes a new ProcessorGraph, and passes it to a new AudioComponent, which then wraps it in an AudioProcessPlayer.

- The ProcessorGraph is a child of juce_AudioProcessorGraph, and it contains all the processors added to the toolchain in the GUI (plus some default processors). Each processor in the graph is called a node. There can be connections between them.

- A processor graph is serialized down to a list of AudioGraphRenderingOp objects, which are executed sequentially whenever AudioProcessorGraph::processBlock() is called.

- AudioProcessorGraph has an AudioSampleBuffer called renderingBuffers that stores the data while it is read or modified by each node in the graph. Specifically, when a ProcessBufferOp is performed for some specific processor, it takes a reference to renderingBuffer and creates a new AudioSampleBuffer that contains pointers to the same data stored in renderingBuffer. It passes that buffer to the AudioProcessor itself, which can use the pointers to read and modify the original data in renderingBuffer.

- AudioProcessorGraph is not itself multithreaded. Individual processors/nodes can contain multiple threads. For example, sources will often have a separate DataThread that gets data from some external hardware (like the acquisition board's FPGA) and stores it in a buffer. When AudioProcessorGraph call its process() callback, it immediately reads some recent data out of that buffer and returns it.

- DataThreads can be defined and used in ProcessorGraphs in open ephys. The open ephys ProcessorManager automatically creates a Processor that wraps around the DataThread (so you can write plugins that are just DataThreads, no need to include a Processor with it, this is how the FPGA source is implemented) and accesses the DataThread's internal buffer, InputBuffer, when called. The DataThread then just has to handle putting data into this InputBuffer, and open ephys will handle interfacing that with the ProcessorGraph.

- The InputBuffer is an instance of the DataBuffer class, which we assume will handle making reading and writing to the buffer thread safe (we read some description about locking).

- In DataThread, run() calls updatebuffer() in a loop until threadShouldExit() returns true, or updatebuffer() returns false. In the FPGA code, the threadShouldExit will return true once stopAquisition() is called.


Questions
----------------------------
In line 1452 of RHD2000Thread, under stopAquisition(), it calls signalThreadShouldExit(), why doesn't it use stopthread()?

For making sources using DataThread, what happens if the aquisition is slow and the GUI requests data faster than the InputBuffer is filled? What will the GUI receive?

We would like to know how openEphys is handling starting/stopping threads vs starting/stopping acquisition. That is, when stopping and restarting aquisition, does it always delete and recreate the thread? Does it maintain all connections to devices between pauses in acquisition or does it need to re-initialize with hardware each time startAqusition() is called?

blackrock:
----------------------------

What does bFlushBuffer do? Should we leave it as true? [[file:~/Dropbox%20(Brown)/Borton%20lab/Transfer%20space/China%20Stimulation%20Code/AutoRecruitmentCurves/Blackrock.cpp::bool%20bFlushBuffer%20=%20true;]]
