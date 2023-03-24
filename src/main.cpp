//funky monkey
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "distribution.h"

using namespace std;

vector<double>multiply_matrix(vector<vector<double>> arr1, vector<double> arr2){
    int arr1_size = arr1.size();
    int inner_arr_size = arr1[0].size();

    vector<double> new_arr (inner_arr_size, 0.0);  //New array after muliplication

    for(int i = 0; i < arr1_size; ++i){
        for(int j = 0; j < inner_arr_size; ++j){
            new_arr[i] += arr1[i][j] * arr2[j];
        }
    }

    return new_arr;
}


int main(){
    // Define the matrices

    vector<vector<double>> note_trans = {{0.31, 0.01, 0.08, 0.01, 0.10, 0.02, 0.11, 0.04, 0.02, 0.10, 0.06, 0.12},
                                         {0.02, 0.34, 0.08, 0.03, 0.07, 0.03, 0.12, 0.03, 0.06, 0.05, 0.02, 0.14},
                                         {0.08, 0.07, 0.35, 0.00, 0.08, 0.02, 0.11, 0.08, 0.02, 0.05, 0.05, 0.09},
                                         {0.04, 0.07, 0.01, 0.28, 0.14, 0.04, 0.13, 0.04, 0.06, 0.08, 0.03, 0.09},
                                         {0.05, 0.04, 0.10, 0.03, 0.35, 0.01, 0.11, 0.08, 0.04, 0.06, 0.03, 0.09},
                                         {0.06, 0.07, 0.07, 0.03, 0.10, 0.26, 0.12, 0.05, 0.05, 0.09, 0.04, 0.07},
                                         {0.04, 0.03, 0.06, 0.03, 0.10, 0.02, 0.34, 0.07, 0.03, 0.08, 0.07, 0.12},
                                         {0.04, 0.02, 0.05, 0.01, 0.09, 0.01, 0.16, 0.36, 0.02, 0.08, 0.05, 0.11},
                                         {0.04, 0.12, 0.04, 0.03, 0.10, 0.04, 0.10, 0.04, 0.25, 0.34, 0.04, 0.12},
                                         {0.07, 0.03, 0.06, 0.01, 0.05, 0.02, 0.11, 0.10, 0.04, 0.34, 0.04, 0.12},
                                         {0.06, 0.02, 0.10, 0.01, 0.07, 0.02, 0.18, 0.09, 0.03, 0.09, 0.22, 0.12},
                                         {0.04, 0.06, 0.07, 0.01, 0.09, 0.01, 0.11, 0.06, 0.03, 0.11, 0.05, 0.35}};

    vector<vector<double>> duration_trans = {{0.0, 0.0, 0.75, 0.25}, {0.0, 0.333, 0.0, 0.666}, {0.0, 0.5, 0.0, 0.5}, {0.0, 0.1666, 0.0, 0.8333}};
    vector<double> note_weight = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<double> duration_weight = {0, 0, 0, 1};
    vector<char> notes = {'C', 'd', 'D', 'e', 'E', 'F', 'g', 'G', 'a', 'A', 'b', 'B'};
    vector<int> duration = {16, 8, 4, 2};

    int num_notes = 100;

    // Output the result
    ofstream myfile;

    myfile.open ("music.ly");
    myfile << "\\version \"2.24.1\" \n\n";
    myfile << "\\fixed c' \{\n\t";

    for(int j = 0; j < num_notes; ++j){
        note_weight = multiply_matrix(note_trans, note_weight);
        duration_weight = multiply_matrix(duration_trans, duration_weight);

        Distribution *note_dist = new Distribution(note_weight);  //Pointer to distribution with weights
        Distribution *dur_dist = new Distribution(duration_weight);
        
        int note_index = note_dist->pick_index();
        int dur_index = dur_dist->pick_index();
        cout << notes[note_index] << " ";
        cout << duration[dur_index] << " ";

        //First if to set the octave
        if(j == 0){
            if(notes[note_index] >= 0x61 && notes[note_index] <= 0x7A){                 //if note is a flat(lowercase)
                myfile << notes[note_index] << "es\'" << " ";
                myfile << duration[dur_index] << " ";
            } else {                                                                    //if uppercase(natural)
                notes[note_index] ^= 0b00100000;
                myfile << notes[note_index] << "\'" << " ";
                myfile << duration[dur_index] << " ";
            }
        } else {
            //Write to rest of the file
            if(notes[note_index] >= 0x61 && notes[note_index] <= 0x7A){                 //if note is a flat(lowercase)
                myfile << notes[note_index] << "es" << " ";
                myfile << duration[dur_index] << " ";
            } else {
                notes[note_index] ^= 0b00100000;
                myfile << notes[note_index] << " ";
                myfile << duration[dur_index] << " ";
            }
        }

        cout << endl;

        delete note_dist;
        delete dur_dist;
    }

    myfile << "\n}";
    myfile.close();

    system("./compile_music.sh");

    return 0;
}