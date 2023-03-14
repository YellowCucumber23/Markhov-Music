#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream midi_file("test.mid", ios::binary); // open the MIDI file in binary mode
    if (!midi_file) {
        cerr << "Error: could not open MIDI file." << endl;
        return 1;
    }

    // skip the header chunk
    char header[14];
    midi_file.read(header, 14);

    // process the track chunks
    while (!midi_file.eof()) {
        char chunk_type[4];
        midi_file.read(chunk_type, 4);
        if (strncmp(chunk_type, "MTrk", 4) != 0) {
            cerr << "Error: invalid chunk type." << endl;
            return 1;
        }

        // read the length of the track chunk
        char length_bytes[4];
        midi_file.read(length_bytes, 4);
        int length = (length_bytes[0] << 24) | (length_bytes[1] << 16) | (length_bytes[2] << 8) | length_bytes[3];

        // process the track events
        int time = 0;
        while (time < length) {
            char delta_time = 0;
            while (true) {
                char c;
                midi_file.read(&c, 1);
                delta_time = (delta_time << 7) | (c & 0x7f);
                if (!(c & 0x80)) break;
            }
            time += delta_time;

            char event_type;
            midi_file.read(&event_type, 1);

            if ((event_type & 0xf0) == 0x80 || (event_type & 0xf0) == 0x90 || (event_type & 0xf0) == 0xa0 ||
                (event_type & 0xf0) == 0xb0 || (event_type & 0xf0) == 0xe0) {
                // process channel events
                char data[2];
                midi_file.read(data, 2);
            } else if ((event_type & 0xf0) == 0xc0 || (event_type & 0xf0) == 0xd0) {
                // process channel events with one data byte
                char data;
                midi_file.read(&data, 1);
            } else if (event_type == 0xff) {
                // process meta events
                char meta_type;
                midi_file.read(&meta_type, 1);

                char length_bytes[4];
                midi_file.read(length_bytes, 4);
                int length = (length_bytes[0] << 24) | (length_bytes[1] << 16) | (length_bytes[2] << 8) | length_bytes[3];

                char* data = new char[length];
                midi_file.read(data, length);
                delete[] data;
            } else {
                cerr << "Error: unknown event type." << endl;
                return 1;
            }
        }
    }

    midi_file.close();
    return 0;
}
