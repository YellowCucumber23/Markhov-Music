#include "MidiFile.cpp"

int main(){
    MidiFile *track = new MidiFile("./miditest.mid");
    char *notes = track->get_notes();

    for(int i = 0; i < track -> get_note_array_size(); ++i){
        std::cout << notes[i] << " ";
    }

    
}