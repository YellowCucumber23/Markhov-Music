#include <iostream>
#include <vector>

struct MidiEvent{
    enum class Type{
        NoteOn,
        NoteOff,
        Other
    } event_type;
    MidiEvent(Type event_type_, uint8_t pitch_, uint8_t velocity_, uint32_t delta_tick_)
                : event_type(event_type_), pitch(pitch_), velocity(velocity_), delta_tick(delta_tick_){}
    
    MidiEvent(Type event_type_) : event_type(event_type_){}
    
    uint8_t pitch = 0;
    uint8_t velocity = 0;
    uint32_t delta_tick = 0;
};


struct MidiNote{
    MidiNote(uint8_t key, uint8_t velocity, uint32_t start_time, uint32_t duration) 
                : nKey(key), nVelocity(velocity), nStartTime(start_time),nDuration(duration){}
    uint8_t nKey = 0;
    uint8_t nVelocity = 0;
    uint32_t nStartTime = 0;
    uint32_t nDuration = 0;
};

struct MidiTrack{
    std::string sName;
    std::string sInstrument;
    std::vector<MidiEvent> midi_events;
    std::vector<MidiNote> midi_notes;
    uint8_t nMaxNote = 64;
    uint8_t nMinNote = 64;
};