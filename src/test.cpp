#include "MidiFile.cpp"

vector<vector<double>> create_transition_matrix(char*& notes_arr, int arr_length){
    vector<double> inner_arr(12,0.0);
    vector<vector<double>> transition_matrix(12, inner_arr);

    for(int i = 0; i < arr_length; ++i){

    }
}


int main(){
    //This prints the notes correctly
    MidiFile *track = new MidiFile("./miditest.mid");
    char *notes = track->get_notes();
    int note_size = track->get_note_array_size();

    for(int i = 0; i < note_size; ++i){
    }
    
}




// #include <iostream>
// #include <vector>

// using namespace std;

// const int NUM_NOTES = 5; // Number of possible notes
// const int SEQ_LENGTH = 10; // Length of sequence


// int main()
// {
//     // Create a 2D array to represent the transition matrix
//     double transitionMatrix[NUM_NOTES][NUM_NOTES] = {0};

//     // Define a sequence of notes
//     vector<int> notes = {1, 3, 2, 4, 1, 3, 4, 2, 1, 4};

//     // Calculate transition probabilities and update matrix
//     for (int i = 0; i < SEQ_LENGTH - 1; i++)
//     {
//         int currentNote = notes[i];
//         int nextNote = notes[i+1];
//         transitionMatrix[currentNote-1][nextNote-1]++;
//     }

//     // Normalize the transition matrix to get transition probabilities
//     for (int i = 0; i < NUM_NOTES; i++){
//         double sum = 0;
//         for (int j = 0; j < NUM_NOTES; j++){
//             sum += transitionMatrix[i][j];
//         }
//         for (int j = 0; j < NUM_NOTES; j++){
//             transitionMatrix[i][j] /= sum;
//         }
//     }

//     // Print the transition matrix
//     for (int i = 0; i < NUM_NOTES; i++){
//         for (int j = 0; j < NUM_NOTES; j++){
//             cout << transitionMatrix[i][j] << " ";
//         }
//         cout << endl;
//     }

//     return 0;
// }
