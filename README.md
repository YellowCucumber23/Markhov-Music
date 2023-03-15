# Markhov-Music
This application takes in a midi file and parses it to create sheet music with the a similar note distribution

Code in `./src` was written by me and everything else is the LilyPond Module used to compile the music into a readable pdf file

The program takes a midi file as an input and outputs the notes of that midi file. The following midi file has the corresponding output:
![test](Midi_Example_Input.png?raw=true "Title")
```c++
{'C', 'G', 'D', 'A', 'D', 'E', 'E', 'B'}
```

It then takes the input and transforms the note distribution into a transition matrix to be used in a Markhov Chain

```c++
{{0.31, 0.01, 0.08, 0.01, 0.10, 0.02, 0.11, 0.04, 0.02, 0.10, 0.06, 0.12},
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

 Where each column is represented by the following notes
 {"C" , "Db",  "D", "Eb",  "E",  "F", "Gb",  "G", "Ab",  "A", "Bb",  "B"}
 ```
We can use another Markhov Chain to determine the rhythms, below is a following matrix:

```c++
{{0.0, 0.0, 0.75, 0.25}
 {0.0, 0.333, 0.0, 0.666}
 {0.0, 0.5, 0.0, 0.5}
 {0.0, 0.1666, 0.0, 0.8333}};

Where each column is corresponds with the following rhythms
{"sixteenth", "eighth", "quarter", "half"}
```

Here is an example output or open music.pdf in `./src` for the full sheet music:
![test](Example.png?raw=true "Title")
