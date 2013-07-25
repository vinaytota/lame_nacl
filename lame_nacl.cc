// Copy pasta of Hello Tutorial from Nacl Examples adapted to encode single channel WAV files into MP3s using lame

#include <cstdio>
#include <string>
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array_buffer.h"
#include <lame/lame.h>


class LameNaclInstance : public pp::Instance {
 public:
  explicit LameNaclInstance(PP_Instance instance) : pp::Instance(instance)
  {}
  virtual ~LameNaclInstance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    pp::VarArrayBuffer array_buffer_var = static_cast<pp::VarArrayBuffer>(var_message);
    short int* pcm_buffer = static_cast<short int*>(array_buffer_var.Map());
    uint32_t byte_length = array_buffer_var.ByteLength();

    const int PCM_SIZE = byte_length/2;    

    lame_t lame = lame_init();
    lame_set_in_samplerate(lame, 44100);
    lame_set_VBR(lame, vbr_default);
    lame_init_params(lame);

    const int MP3_SIZE = byte_length/2;
    unsigned char* mp3_buffer = new unsigned char[MP3_SIZE];
    int mp3_buffer_write_size;
    mp3_buffer_write_size = lame_encode_buffer(lame, pcm_buffer, pcm_buffer, byte_length/2, mp3_buffer, MP3_SIZE);

    unsigned char mp3_end_buffer[8192];
    int mp3_end_buffer_write_size;
    mp3_end_buffer_write_size = lame_encode_flush(lame, mp3_end_buffer, 8192);

    lame_close(lame);

    pp::VarArrayBuffer* response_buffer = new pp::VarArrayBuffer(mp3_buffer_write_size + mp3_end_buffer_write_size);
    char* data = static_cast<char*>(response_buffer->Map());
    for (uint32_t i = 0; i < response_buffer->ByteLength(); ++i) {
      if(i < mp3_buffer_write_size) {
        data[i] = mp3_buffer[i];
      } else {
        data[i] = mp3_end_buffer[i - mp3_buffer_write_size];
      }
    }

    delete mp3_buffer;

    PostMessage(*response_buffer);
  }
};

/// The Module class.  The browser calls the CreateInstance() method to create
/// an instance of your NaCl module on the web page.  The browser creates a new
/// instance for each <embed> tag with type="application/x-nacl".
class LameNaclModule : public pp::Module {
 public:
  LameNaclModule() : pp::Module() {}
  virtual ~LameNaclModule() {}

  /// Create and return a HelloTutorialInstance object.
  /// @param[in] instance The browser-side instance.
  /// @return the plugin-side instance.
  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new LameNaclInstance(instance);
  }
};

namespace pp {
/// Factory function called by the browser when the module is first loaded.
/// The browser keeps a singleton of this module.  It calls the
/// CreateInstance() method on the object you return to make instances.  There
/// is one instance per <embed> tag on the page.  This is the main binding
/// point for your NaCl module with the browser.
Module* CreateModule() {
  return new LameNaclModule();
}
}  // namespace pp
