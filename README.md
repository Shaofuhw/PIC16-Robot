# PIC16-Robot
Robot made for a university project with a PIC16F883 microcontroller in Assembler

The robot has a microphone and have a set of movements depending on the number of "claps" that it receives:

  - 1 Clap: Forward.
  - 2 Claps: Backwards.
  - 3 Claps: Left Turn.
  - 4 Claps: Right Turn.
  
If it receives one clap while moving, it will stop.
Furthermore, it has an ultrasonic sensor that prevents collision, and a LDR that turn on some frontal and back LEDs depending on the light condition of the environment.
