# OnlineDevelopment

This repo holds the basic prototype of the Eve-7 Mu2e Event Display. It is a standalone root script containing:

* A macro which must be run using root 6.22
* A source and header files to carry out the C++  functions

To run the example:

* If you are on Mu2e machine: ```setup root v6_22_02 -qe20:p383b:prof``` will give you access to the ROOT7 build

Then:

* run the Macro.C in the root terminal
* You should see a list of shown nodes appear in the terminal
* The Macro will open up a firefox browser
* After a few seconds you should see te geometry appear
* To access remotely, create an ssh tunnel:

```ssh  -KXY -L WZYZ:mu2edaq11.fnal.gov:WXYZ mu2etrig@mu2edaq01.fnal.gov``` replacing ```WXYZ``` with the number shown in the local terminal.

* copy the URL (starts with localhost) into remote browser. You should see the same geometry and track.

