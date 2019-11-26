#### drifter core ####
##### a collection of utilities for generative art #####

here's a list of a few things built with this code:

* stochastic triggering and mixing of audio clips
* "drifting" effects for audio and video mixing
* networked IPC with web applications, Ableton live, Max/MSP, etc

##### dependencies #####

* [libcinder 0.9.1](https://libcinder.org/)
* [Cinder-Asio](https://github.com/BanTheRewind/Cinder-Asio)
* CMake ^3.15.4

##### TODOs and known issues #####

* uses Cinder's ci_make_app CMake function which is limited to building executables.  ideally, we would build a library here and projects linking against it would live in a separate repository
* note that several classes (VideoData, AudioData) that have not been thoroughly tested since upgrading to Cinder 0.9