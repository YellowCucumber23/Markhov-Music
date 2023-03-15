# Markhov-Music
This application takes in a midi file and parses it to create sheet music with the a similar note distribution

Code in `./src` was written by me and everything else is the LilyPond Module used to compile the music into a readable pdf file

The program takes a midi file as an input and outputs the notes of that midi file. The following midi file has the corresponding output:
![test](Midi_Example_Input.png?raw=true "Title")
```c++
{'C', 'G', 'D', 'A', 'D', 'E', 'E', 'B'}
```


Here is an example output or open music.pdf in `./src` for the full sheet music:
![test](Example.png?raw=true "Title")
