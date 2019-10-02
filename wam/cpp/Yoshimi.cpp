//
//  Yoshimi.cpp
//	WAM port
//
//  Created by Jari Kleimola on 2018/10/12
//

#include "Yoshimi.h"
#include "Misc/SynthEngine.h"

extern "C" { EMSCRIPTEN_KEEPALIVE void* createModule() { return new Yoshimi_WAM(); }}

Yoshimi_WAM::Yoshimi_WAM () {}

const char* Yoshimi_WAM::init(uint32_t bufsize, uint32_t sr, void* desc)
{
	WAM::Processor::init(128, sr, desc);
	SynthEngine* engine = new SynthEngine(0, NULL, true);
	engine->getRuntime().Setup(0, nullptr);

	yoshimi = new Yoshimi(engine);
	yoshimi->init(128, sr);

	return nullptr;
}

void Yoshimi_WAM::terminate()
{
	delete yoshimi;
}

void Yoshimi_WAM::onMessage(char* verb, char* res, void* data, uint32_t size)
{
}

void Yoshimi_WAM::onParam(uint32_t idparam, double value)
{
}

void Yoshimi_WAM::onPatch(void* patch, uint32_t numBytes)
{
}

void Yoshimi_WAM::onMessage(char* verb, char* res, double data)
{
	string sverb = verb;
	string sres = res;
	if (sverb == "set") yoshimi->set(sres, data);
}

void Yoshimi_WAM::onMessage(char* verb, char* res, char* data)
{
	string sverb = verb;
	string sprop = res;
	if (sverb == "set") yoshimi->set(sprop, string(data));
}

void Yoshimi_WAM::onMidi(byte status, byte data1, byte data2)
{
	yoshimi->onMidi(status, data1, data2);
}

void Yoshimi_WAM::onProcess(AudioBus* audio, void* data)
{
	yoshimi->render(audio, data);
}

// ==============================================================================================

Yoshimi::Yoshimi(SynthEngine* engine)
:	MusicIO(engine),
	_synth(engine),
	_sampleRate(0),
	_bufferSize(0),
	_bufferPos(0),
	_offsetPos(0),
	_bFreeWheel(0)
{
}

Yoshimi::~Yoshimi()
{
	Close();
	delete _synth;
}

#define FAIL(err) { printf(err); return false; }
extern SynthEngine *firstSynth;

bool Yoshimi::init(uint32_t bufsize, uint32_t sr)
{
	_sampleRate = sr;
	_bufferSize = bufsize;
	if (!prepBuffers()) FAIL("prepBuffers")

	if (!_synth->Init(_sampleRate, _bufferSize)) FAIL("Can't init synth engine")
	if (_synth->getUniqueId() == 0) firstSynth = _synth;
	_synth->getRuntime().showGui = false;

	memset(wamLeft,  0, sizeof(float *) * (NUM_MIDI_PARTS + 1));
	memset(wamRight, 0, sizeof(float *) * (NUM_MIDI_PARTS + 1));

	_synth->getRuntime().runSynth = true;
	Start();

	return true;
}

void Yoshimi::set(string prop, double value)
{
	if (prop == "freewheel")
		_bFreeWheel = value;
}

void Yoshimi::set(string prop, string value)
{
	if (prop == "patch") {
		int npart = 0;
		// _synth->resetAll(0);
		// _synth->partonoffLock(npart, -1);
		string s = value;
		_synth->part[npart]->loadXMLinstrument(s);
		// _synth->partonoffLock(npart, 2 - _synth->getRuntime().enable_part_on_voice_load);
	}
}

void Yoshimi::onMidi(byte status, byte data1, byte data2)
{
	_midiDataPort.push_back(midi_event(0, status, data1, data2 ));
}

void Yoshimi::render(AudioBus* audio, void* data)
{
	wamLeft[0]  = audio->outputs[0];
	wamRight[0] = audio->outputs[1];

	synth->sent_all_buffersize_f = _bufferSize;
	int real_sample_count = _bufferSize;
	int offs = 0;
	int next_frame = 0;
	int processed = 0;
	float *tmpLeft  [NUM_MIDI_PARTS + 1];
	float *tmpRight [NUM_MIDI_PARTS + 1];
	struct midi_event intMidiEvent;

	for (uint32_t i = 0; i < NUM_MIDI_PARTS + 1; ++i) {
    tmpLeft [i] = zynLeft [i];
    tmpRight [i] = zynRight [i];
	}

	for (auto& event : _midiDataPort) {
		next_frame = event.time;
		if (next_frame >= real_sample_count) continue;
		int to_process = next_frame - offs;

		if ((to_process > 0)
			 	&& (processed < real_sample_count)
			 	&& (to_process <= (real_sample_count - processed))) {
			int mastered = 0;
			offs = next_frame;
			while (to_process - mastered > 0) {
				int mastered_chunk = _synth->MasterAudio(tmpLeft, tmpRight, to_process - mastered);
				for (uint32_t i = 0; i < NUM_MIDI_PARTS + 1; ++i) {
					tmpLeft [i]  += mastered_chunk;
					tmpRight [i] += mastered_chunk;
				}
				mastered += mastered_chunk;
			}
			processed += to_process;
		}

		// -- process this midi event
		const uint8_t *msg = (const uint8_t*)(event.data);
		// if (_bFreeWheel != NULL)
			processMidiMessage(msg);
	}
	_midiDataPort.clear();

	if (processed < real_sample_count) {
		uint32_t to_process = real_sample_count - processed;
		int mastered = 0;
		offs = next_frame;
		while (to_process - mastered > 0) {
			int mastered_chunk = _synth->MasterAudio(tmpLeft, tmpRight, to_process - mastered);
			for (uint32_t i = 0; i < NUM_MIDI_PARTS + 1; ++i) {
				tmpLeft [i]  += mastered_chunk;
				tmpRight [i] += mastered_chunk;
			}
			mastered += mastered_chunk;
		}
		processed += to_process;
	}

	memcpy(wamLeft[0],  zynLeft[NUM_MIDI_PARTS],  128 * sizeof(float));
	memcpy(wamRight[0], zynRight[NUM_MIDI_PARTS], 128 * sizeof(float));
}

void Yoshimi::processMidiMessage(const uint8_t * msg)
{
	bool in_place = false; // _bFreeWheel ? ((*_bFreeWheel == 0) ? false : true) : false;
	setMidi(msg[0], msg[1], msg[2], in_place);
}

void mainRegisterAudioPort(SynthEngine *, int) {}
