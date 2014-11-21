YAZZ
====

YAZZ (Yet Another Speech Recognizer) is my modest attempt to implement a text recognition application.
Also it is my first attempt to write something on C++ since the university... So, let's hope it will be successful! :)

**The project is still under development and doesn't have a stable build. See Milestone section for details.**


Dependences
-----------
Note this project has external dependencies:
* libpng;
* gtest;

It is tested to run on both Windows 7 (MinGw/GCC 4.8.1) and Ubuntu 12.04 (GCC 4.6.3).


Build
-----
This project uses CMake as a buid tool. So, all that you need are:

1. Make sure that you have CMake installed;
2. Install external dependencies: [libpng](http://www.libpng.org/pub/png/libpng.html), [gtest](http://code.google.com/p/tonatiuh/wiki/InstallingGoogleTestForWindows) (available through apt-get);

  a. Keep in mind that in case of using MinGW for Windows you may have to amend CMake's FindGTest module and fix/patch io.h header;
3. Run "build-cdt.sh";
4. Open Eclipse and import the new project from the "build" directory; 


Dictionary
----------
Current version deals with the following dictionary: odin, dva, tri, chetire, pyat, shest, sem, vosem, devyat, desyat.
I have samples of voices of 4 people (3 man and 1 women).


Milestone
---------
~~At the moment I'm working (sometimes) on implementing the basis (framework) of the application. It is supposed to be
a lightweight console application which can recognize the limited dictionary by MFCC / DTW.~~

~~The next step is replacing DTW solution (which is a temporary stub) with HHM. This should increase recognition quality
and make this project... finished? :)~~

Project is unfinished and moved into archive.


Command line
------------
See help for details (yazz -h).
