# OnlineDevelopment

This repo holds the basic prototype of the Eve-7 Mu2e Event Display. It is a standalone root script containing:

* A macro which must be run using root 6.22
* A source and header files to carry out the C++  functions

To run the example:

* If you are on Mu2e machine: ```setup root v6_22_02 -qe20:p383b:prof``` will give you access to the ROOT7 build

Then on the Mu2e machine do the following:

* run the Macro.C in the root terminal
* You should see a list of shown nodes appear in the terminal
* The Macro will open up a firefox browser
* After a few seconds you should see te geometry appear
* To access remotely, create an ssh tunnel ON YOUR OWN COMPUTER:

```ssh  -KXY -L WZYZ:mu2edaq12.fnal.gov:WXYZ username@mu2emachinename.fnal.gov``` replacing ```WXYZ``` with the number shown in the local terminal. Replace ```username``` with your name and ```mu2emachinename```with the name of the mu2e node.

* copy the URL (starts with localhost) from the Mu2e machine into the remote browser i.e the one on your own computer. You should see the same geometry and track.

# OTSDAQ

An Eve-7 has been added to the OTSDAQ Desktop this will broadcast the event display and allows user interaction.

The OTSDAQ page can be accessed from anywhere on the FNAL network, including using VPN. You can use this link ```https://mu2edaq01.fnal.gov:4016/urn:xdaq-application:lid=200``` if on the network.

Google chrome is recommended.

If you do not have a OTSDAQ account you can see the .mov video which shows what the user with access can see and do.


