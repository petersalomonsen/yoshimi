//
//  Yoshimi.h
//	WAM port
//
//  Created by Jari Kleimola on 2018/10/12
//
#ifndef yoshimi_h
#define yoshimi_h

#include "MusicIO/MusicIO.h"
#include "deps/wamsdk/processor.h"
using namespace WAM;

class SynthEngine;

class Yoshimi : public MusicIO
{
public:
	Yoshimi(SynthEngine* engine);
	virtual ~Yoshimi();
	bool init(uint32_t bufsize, uint32_t sr);
	void set(string prop, double value);
	void set(string prop, string value);
	void onMidi(byte status, byte data1, byte data2);
	void render(AudioBus* audio, void* data);

	// -- MusicIO
	unsigned int getSamplerate(void) override { return _sampleRate; }
	int getBuffersize(void) override { return _bufferSize; }
	bool Start(void) override { synth->Unmute(); return true; }
	void Close(void) override { synth->Mute(); }
	bool openAudio() override { return true; }
	bool openMidi()  override { return true; }
	string audioClientName(void) override { return "Yoshimi-WAM"; }
	string midiClientName(void)  override { return "Yoshimi-WAM"; }
	int audioClientId(void) override { return 0; }
	int midiClientId(void)  override { return 0; }
	void registerAudioPort(int) override {}

private:
	SynthEngine* _synth;
	uint32_t _sampleRate;
	uint32_t _bufferSize;
	uint32_t _bufferPos;
	uint32_t _offsetPos;
	float _bFreeWheel;
	float* wamLeft  [NUM_MIDI_PARTS + 1];
	float* wamRight [NUM_MIDI_PARTS + 1];

	struct midi_event {
		midi_event() { time = 0; data[0] = data[1] = data[2] = data[3] = 0; }
		midi_event(uint64_t t, char b1, char b2, char b3, char b4=0) {
			time = t; data[0] = b1; data[1] = b2; data[2] = b3; data[3] = b4;
		}
		uint64_t time;	// in frames
		char data[4];
	};
	std::vector<midi_event> _midiDataPort;
	void processMidiMessage(const uint8_t * msg);
};

class Yoshimi_WAM : public Processor
{
public:
	Yoshimi_WAM();
	
 	// -- WAM::Processor
	const char* init(uint32_t bufsize, uint32_t sr, void* desc) override;
	void terminate() override;
	void onProcess(AudioBus* audio, void* data) override;
	void onMidi(byte status, byte data1, byte data2) override;
	void onPatch(void* patch, uint32_t size) override;
	void onParam(uint32_t idparam, double value) override;
	void onMessage(char* verb, char* res, double data) override;
	void onMessage(char* verb, char* res, char* data) override;
	void onMessage(char* verb, char* res, void* data, uint32_t size) override;
	
private:
	Yoshimi* yoshimi;
};

#endif // yoshimi_h
