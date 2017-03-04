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
