# myCNoisySynthesizer
Synthesizer with noise and keyboard input

## Features

- Noise bandpass difference digital synthesizer;
- Source-set mono 96kHz output;
- 24-note keyboard input;
- 14-octave range (A0-G#13);
- Note toggle and note pluck modes;
- Bandpass frequency width control;

## Setup

### Requirements

- PortAudio: required for the live processing option.
  - Install via `sudo dnf install portaudio-devel` for Debian/Ubuntu.
  - Install via `sudo apt install portaudio19-dev` for Debian/Ubuntu.

### Compiling

Compile the project with
```sh
make clean build/cnsynth
```

## Usage

### Running

You can run `build/cnsynth` if it is compiled or build and run with
```
make run
```

### UI

Once `build/cnsynth` is running, the control keys for a pt-Br ABNT2 keyboard and the actual state of the adjustable parameters are on-screen.
