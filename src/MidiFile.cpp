#include <iostream>
#include <fstream>
#include <vector>
#include "MidiStructs.h"

using namespace std;

class MidiFile{
    //Private members
    private:
        vector<MidiTrack> midi_tracks;
        uint32_t m_nTempo = 0;
        uint32_t m_nBPM = 0;

    //Constructor and public functions
    public:
        MidiFile(const string &file_name){
            parse_file(file_name);
        }

        enum EventName : uint8_t{					
            note_off = 0x80,
            note_on = 0x90,
            after_touch = 0xA0,
            controller = 0xB0,
            program_change = 0xC0,
            channel_after_touch = 0xD0,
            pitch_bend = 0xE0,
            system = 0xF0,		
        };

        enum MetaEventName : uint8_t
        {
            MetaSequence = 0x00,
            MetaText = 0x01,
            MetaCopyright = 0x02,
            MetaTrackName = 0x03,
            MetaInstrumentName = 0x04,
            MetaLyrics = 0x05,
            MetaMarker = 0x06,
            MetaCuePoint = 0x07,
            MetaChannelPrefix = 0x20,
            MetaEndOfTrack = 0x2F,
            MetaSetTempo = 0x51,
            MetaSMPTEOffset = 0x54,
            MetaTimeSignature = 0x58,
            MetaKeySignature = 0x59,
            MetaSequencerSpecific = 0x7F,
        };
        
    bool parse_file(const string &file_name){
        //Open Midi file as MidiStream
        ifstream input;
        input.open(file_name, ifstream::in | ios::binary);

        if(!input.is_open()){
            std::cout << "Couldn't Open File";
        }

        //Parse Header
        uint32_t temp32 = 0;
        uint16_t temp16 = 0;

        //Recognize midi files
		input.read((char*)&temp32, sizeof(uint32_t));
		uint32_t fileID = swap_bit_order32(temp32);

        //Length of header
		input.read((char*)&temp32, sizeof(uint32_t));
		uint32_t header_length = swap_bit_order32(temp32);

        //Midi file format
		input.read((char*)&temp16, sizeof(uint16_t));
		uint16_t format = swap_bit_order16(temp16);

        //Number of track chunks
		input.read((char*)&temp16, sizeof(uint16_t));
		uint16_t track_chunks = swap_bit_order16(temp16);

        //Number of divisions
		input.read((char*)&temp16, sizeof(uint16_t));
		uint16_t division = swap_bit_order16(temp16);

        //Iterate through all tracks
        for(int i = 0; i < track_chunks; ++i){
            // cout << "================ NEW TRACK ================"<< endl;

            midi_tracks.push_back(MidiTrack());


            //Track header
            input.read((char*) &temp32, sizeof(uint32_t));
            uint16_t trackID = swap_bit_order16(temp32);

            input.read((char*) &temp32, sizeof(uint32_t));
            uint16_t track_length = swap_bit_order16(temp32);

            bool track_end = false;
            uint8_t prev_status = 0;


            //Read midi stream
            while(!input.eof() && !track_end){
                uint32_t time_status_delta = 0;
                uint8_t status = 0;

                time_status_delta = read_value(input);
                status = input.get();

                if(status < 0x80){
                    status = prev_status;
                    input.seekg(-1, ios_base::cur);
                }

                if((status & 0xF0) == EventName::note_off){
                    prev_status = status;
                    uint8_t channel = status & 0x0F;
                    uint8_t pitch = input.get();
                    uint8_t velocity = input.get();

                    MidiEvent event(MidiEvent::Type::NoteOff, pitch, velocity, time_status_delta);
                    midi_tracks[i].midi_events.push_back(event);
                    
                } else if((status & 0xF0) == EventName::note_on){
                    prev_status = status;
                    uint8_t channel = status & 0x0F;
                    uint8_t pitch = input.get();
                    uint8_t velocity = input.get();

                    if(velocity == 0){
                        MidiEvent event(MidiEvent::Type::NoteOff, pitch, velocity, time_status_delta);
                        midi_tracks[i].midi_events.push_back(event);
                    } else {
                        MidiEvent event(MidiEvent::Type::NoteOn, pitch, velocity, time_status_delta);
                        midi_tracks[i].midi_events.push_back(event);
                    }

                    // std::cout << pitch << std::endl;

                } else if((status & 0xF0) == EventName::after_touch){
                    prev_status = status;
                    uint8_t channel = status & 0x0F;
                    uint8_t pitch = input.get();
                    uint8_t velocity = input.get();

                    MidiEvent event(MidiEvent::Type::Other);
                    midi_tracks[i].midi_events.push_back(event);

                } else if((status & 0xF0) == EventName::controller){
                    prev_status = status;
                    uint8_t channel = status & 0x0F;
                    uint8_t pitch = input.get();
                    uint8_t velocity = input.get();

                    MidiEvent event(MidiEvent::Type::Other);
                    midi_tracks[i].midi_events.push_back(event);
                } else if((status & 0xF0) == EventName::program_change){
                    prev_status = status;
                    uint8_t channel = status & 0x0F;
                    uint8_t program_ID = input.get();

                    MidiEvent event(MidiEvent::Type::Other);
                    midi_tracks[i].midi_events.push_back(event);
                } else if((status & 0xF0) == EventName::channel_after_touch){
                    prev_status = status;
                    uint8_t channel = status & 0x0F;
                    uint8_t channel_pressure = input.get();
                } else if((status & 0xF0) == EventName::pitch_bend){
					prev_status = status;
					uint8_t channel = status & 0x0F;
					uint8_t nLS7B = input.get();
					uint8_t nMS7B = input.get();

                    MidiEvent event(MidiEvent::Type::Other);
                    midi_tracks[i].midi_events.push_back(event);
                } else if((status & 0xF0) == EventName::system){
                    
                    prev_status = 0;

                    if(status == 0xF0){
                        std::cout << "System Exclusive Begin: " << read_string(input, read_value(input)) << std::endl;
                    } 

                    if(status == 0xF7){
                        std::cout << "System Exclusive End: " << read_string(input, read_value(input)) << std::endl;
                    }

                    if(status == 0xFF){
                        // Meta Message
						uint8_t nType = input.get();
						uint8_t length = read_value(input);

						switch (nType){
                            case MetaSequence:
                                std::cout << "Sequence Number: " << input.get() << input.get() << std::endl;
                                break;
                            case MetaText:
                                std::cout << "Text: " << read_string(input, length) << std::endl;
                                break;
                            case MetaCopyright:
                                std::cout << "Copyright: " << read_string(input, length) << std::endl;
                                break;
                            case MetaTrackName:
                                midi_tracks[i].sName = read_string(input, length);
                                std::cout << "Track Name: " << midi_tracks[i].sName << std::endl;							
                                break;
                            case MetaInstrumentName:
                                midi_tracks[i].sInstrument = read_string(input, length);
                                std::cout << "Instrument Name: " << midi_tracks[i].sInstrument << std::endl;
                                break;
                            case MetaLyrics:
                                std::cout << "Lyrics: " << read_string(input, length) << std::endl;
                                break;
                            case MetaMarker:
                                std::cout << "Marker: " << read_string(input, length) << std::endl;
                                break;
                            case MetaCuePoint:
                                std::cout << "Cue: " << read_string(input, length) << std::endl;
                                break;
                            case MetaChannelPrefix:
                                std::cout << "Prefix: " << input.get() << std::endl;
                                break;
                            case MetaEndOfTrack:
                                track_end = true;
                                break;
                            case MetaSetTempo:
                                // Tempo is in microseconds per quarter note	
                                if (m_nTempo == 0)
                                {
                                    (m_nTempo |= (input.get() << 16));
                                    (m_nTempo |= (input.get() << 8));
                                    (m_nTempo |= (input.get() << 0));
                                    m_nBPM = (60000000 / m_nTempo);
                                    std::cout << "Tempo: " << m_nTempo << " (" << m_nBPM << "bpm)" << std::endl;
                                }
                                break;
                            case MetaSMPTEOffset:
                                std::cout << "SMPTE: H:" << input.get() << " M:" << input.get() << " S:" << input.get() << " FR:" << input.get() << " FF:" << input.get() << std::endl;
                                break;
                            case MetaTimeSignature:
                                std::cout << "Time Signature: " << input.get() << "/" << (2 << input.get()) << std::endl;
                                std::cout << "ClocksPerTick: " << input.get() << std::endl;

                                // A MIDI "Beat" is 24 ticks, so specify how many 32nd notes constitute a beat
                                std::cout << "32per24Clocks: " << input.get() << std::endl;
                                break;
                            case MetaKeySignature:
                                std::cout << "Key Signature: " << input.get() << std::endl;
                                std::cout << "Minor Key: " << input.get() << std::endl;
                                break;
                            case MetaSequencerSpecific:
                                std::cout << "Sequencer Specific: " << read_string(input, length) << std::endl;
                                break;
                            default:
                                std::cout << "Unrecognised MetaEvent: " << nType << std::endl;
						}
					}
                }else {
                    std::cout << "Unrecognized Bit" << std::endl;
                }
            }
        }
        return true;
    }


    vector<MidiNote> get_notes(){
        vector<MidiNote> notes_list;
        for (auto& track : midi_tracks){
			uint32_t nWallTime = 0;

			for (auto& event : track.midi_events)
			{
				nWallTime += event.delta_tick;

				if (event.event_type == MidiEvent::Type::NoteOn)
				{
					// New Note
                    MidiNote note(event.pitch, event.velocity, nWallTime, 0);
					notes_list.push_back(note);
				}
            }
        }
        return notes_list;
    }

    //Private Functions
    private:
        //Swap bits 32 bit integer
        uint32_t swap_bit_order32(uint32_t n){
            return ((n >> 24) & 0xff) | // move byte 3 to byte 0
                    ((n << 8) & 0xff0000) | // move byte 1 to byte 2
                    ((n >> 8) & 0xff00) | // move byte 2 to byte 1
                    ((n << 24) & 0xff000000); // byte 0 to byte 3
        }
        //Swap bits of 16 bit integer
        uint16_t swap_bit_order16(uint16_t n){
            return (n >> 8) | (n << 8);
        }

        //Read bytes of specific length from file stream
        string read_string(ifstream &ifs, uint32_t length){
            string s;

            for(int i = 0; i < length; ++i){
                s += ifs.get();
            }
            return s;
        }

        uint32_t read_value(ifstream &ifs){
            uint32_t value;
            uint8_t temp;

            value = ifs.get();

            if(value & 0x80){
                value &= 0x7F;
                do{
                    //Read next byte, use & to get rid of MSB 
                    temp = (ifs.get()) & 0x7F;

                    //Move value over to combine with temp bit
                    value = (value << 7) | temp;
                }while(temp & 0x80);
            }
            return value;
        }
};

// int main(){
//     std::cout << "Done";
// }