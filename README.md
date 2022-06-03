# Embedded-Systems
This repository holds all of the coding work completed in NYU Tandon's Embedded Systems course.

The "Embedded_FinalProject_KierstenPage.ino" file is the code for the final project of the class.
The purpose of the project was to interface an Arduino Uno with a MPU-6050 such that upon a 
series of consecutive movements, there would be a key "unlock". The code allows for a user to 
first set the unlock sequence, and then attempt to "unlock" the key through repeating the same
sequence. Upon unlock, the begginging sequence of "Welcome to the Black Parade" is played using
a piezzo buzzer, and a series of LED's light up that correspond to the "note" being played. 
