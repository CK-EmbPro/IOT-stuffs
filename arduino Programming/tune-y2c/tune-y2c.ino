const int buzzerPin = 9;
const int songLength = 160;
char notes[] = "eeeeeeegcde fffffeeeeddedg eeeeeeegcde fffffeeeggedc pedcp pedcy yedcv ggede eeeeeeegcde fffffeeeeddedg eeeeeeegcde fffffeeeggedc"; // a space represents a rest
int beats[] = {1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,2,1,1,2,1,1,1,1,4,1,1,1,1,1,1,1,1,1,4,4,4,4,8};
int tempo = 300;
void setup()
{ pinMode(buzzerPin, OUTPUT); }
void loop()
{ int i, duration;
for (i = 0; i < songLength; i++) // step through the song arrays
{duration = beats[i] * tempo; // length of note/rest in ms
if (notes[i] == ' ')
// is this a rest?
{ delay(duration);
}
else
// otherwise, play the note
{tone(buzzerPin, frequency(notes[i]), duration);
delay(duration);
// wait for tone to finish
}
delay(tempo/10);
}
while(true){} }
int frequency(char note)
{ int i;
const int numNotes = 11; // number of notes we're storing
char names[] = { 'p', 'y', 'v', 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
int frequencies[] = {201, 222, 250, 262, 294, 330, 349, 392, 440, 494, 523};
for (i = 0; i < numNotes; i++)
{ if (names[i] == note)
{ return(fre
