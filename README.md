lame_nacl
=========

Encode WAV files to MP3 using the LAME MP3 encoder and Chrome Native Client (NaCL)

Post message to the NaCL object with an array buffer containing a single channel wav file and you'll get an encoded MP3 back.

Compiles and links statically against newlib by default to use glibc and the release configuration (to strip debug symbols)

make TOOLCHAIN=glibc CONFIG=Release

TODO: Add working example based on http://webaudiodemos.appspot.com/AudioRecorder/index.html