What is this?
=============
Currently this directory will hold a makeshift way of managing our
more unpopular dependencies as they are unlikely to be packaged by major
distributions.

There will likely be a better way for this later on. For now, this has to do.

Getting the dependencies
========================
For now, you will manually have to get the externals. Please get these:

ogre-procedural
---------------

Download source:
hg clone https://code.google.com/p/ogre-procedural/ ogre-procedural

Build:
cd ogre-procedural
mkdir build && cd build
cmake .. && make -j3
