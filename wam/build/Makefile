# Web Audio Modules
# wasm makefile for Yoshimi
# Jari Kleimola 2018-19

TARGET = ./yoshimi.js

SRC  = ../../src
DSP  = $(SRC)/DSP
FX   = $(SRC)/Effects
IF   = $(SRC)/Interface
CLI  = $(SRC)/CLI
Misc = $(SRC)/Misc
Synth   = $(SRC)/Synth
Params  = $(SRC)/Params
MusicIO = $(SRC)/MusicIO
WAM  = ../cpp
DEPS = $(WAM)/deps

SOURCES =  $(WAM)/Yoshimi.cpp
SOURCES += $(WAM)/deps/wamsdk/processor.cpp

SOURCES += $(DSP)/AnalogFilter.cpp
SOURCES += $(DSP)/FFTwrapper.cpp
SOURCES += $(DSP)/Filter.cpp
SOURCES += $(DSP)/FormantFilter.cpp
SOURCES += $(DSP)/SVFilter.cpp
SOURCES += $(DSP)/Unison.cpp
SOURCES += $(FX)/Alienwah.cpp
SOURCES += $(FX)/Chorus.cpp
SOURCES += $(FX)/Distorsion.cpp
SOURCES += $(FX)/DynamicFilter.cpp
SOURCES += $(FX)/Echo.cpp
SOURCES += $(FX)/Effect.cpp
SOURCES += $(FX)/EffectLFO.cpp
SOURCES += $(FX)/EffectMgr.cpp
SOURCES += $(FX)/EQ.cpp
SOURCES += $(FX)/Phaser.cpp
SOURCES += $(FX)/Reverb.cpp
SOURCES += $(IF)/InterChange.cpp
SOURCES += $(IF)/MidiDecode.cpp
SOURCES += $(IF)/MidiLearn.cpp
SOURCES += $(IF)/Data2Text.cpp
SOURCES += $(IF)/RingBuffer.cpp
#SOURCES += $(IF)/TextLists.cpp
#SOURCES += $(CLI)/CmdInterface.cpp
#SOURCES += $(CLI)/ConfigCLI.cpp
#SOURCES += $(CLI)/CmdInterpreter.cpp
SOURCES += $(Misc)/Bank.cpp
SOURCES += $(Misc)/Config.cpp
SOURCES += $(Misc)/Microtonal.cpp
SOURCES += $(Misc)/Part.cpp
SOURCES += $(Misc)/Splash.cpp
SOURCES += $(Misc)/SynthEngine.cpp
SOURCES += $(Misc)/WavFile.cpp
SOURCES += $(Misc)/XMLwrapper.cpp
SOURCES += $(MusicIO)/MusicClient.cpp
SOURCES += $(MusicIO)/MusicIO.cpp
SOURCES += $(Params)/ADnoteParameters.cpp
SOURCES += $(Params)/Controller.cpp
SOURCES += $(Params)/EnvelopeParams.cpp
SOURCES += $(Params)/FilterParams.cpp
SOURCES += $(Params)/LFOParams.cpp
SOURCES += $(Params)/SUBnoteParameters.cpp
SOURCES += $(Params)/PADnoteParameters.cpp
SOURCES += $(Params)/Presets.cpp
SOURCES += $(Params)/PresetsStore.cpp
SOURCES += $(Params)/UnifiedPresets.cpp
SOURCES += $(Synth)/ADnote.cpp
SOURCES += $(Synth)/Envelope.cpp
SOURCES += $(Synth)/LFO.cpp
SOURCES += $(Synth)/OscilGen.cpp
SOURCES += $(Synth)/PADnote.cpp
SOURCES += $(Synth)/Resonance.cpp
SOURCES += $(Synth)/SUBnote.cpp

INC = -I$(WAM) -I$(SRC) -I$(DEPS) -I$(DEPS)/kissfft -I$(DEPS)/mxml
LIBS = mxml.bc kissfft.bc

EXPORTS = "[\
  '_createModule','_wam_init','_wam_terminate','_wam_resize', \
  '_wam_onprocess', '_wam_onmidi', '_wam_onsysex', '_wam_onparam', \
  '_wam_onmessageN', '_wam_onmessageS', '_wam_onmessageA', '_wam_onpatch', \
  '_yoshimi_getParametersXML' \
  ]"

WARN = -Wall -Wno-unused-private-field -Wno-parentheses-equality -Wno-unused-variable

YFLAGS  = -DYOSHIMI=\"yoshimi\" -DYOSHIMI_VERSION=\"1.6.0\" -DMIN_CONFIG_MAJOR=1 -DMIN_CONFIG_MINOR=5
YCONFIG = -DDEFAULT_AUDIO="audio_drivers::no_audio" -DDEFAULT_MIDI="midi_drivers::no_midi"
CFLAGS  = -DWASM -DAUDIOWORKLET $(YFLAGS) $(YCONFIG) -std=c++11 $(WARN) $(INC)
LDFLAGS = -O2 -s EXPORTED_FUNCTIONS=$(EXPORTS)
EMFLAGS = -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s BINARYEN_ASYNC_COMPILATION=0 -s EXPORT_NAME="'AudioWorkletGlobalScope.WAM.YOSHIMI'" -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap","setValue"]'

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(EMFLAGS) $(LIBS) -o $@ $(SOURCES)
