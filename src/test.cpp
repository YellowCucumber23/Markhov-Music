#include "MidiFile.cpp"

int main(){
    MidiFile *track = new MidiFile("./miditest.mid");
    vector<MidiNote> temp = track -> get_notes();
    vector<char> notes = {'C', 'd', 'D', 'e', 'E', 'F', 'g', 'G', 'a', 'A', 'b', 'B'};

    for(auto i : temp){
        std::cout << notes[i.nKey%12] << " ";
    }
}