# SubtitleBroadcast
This project consists in a set of tools to create and distribute subtitles and TTS-generated audio information.

## Overview

This set of projects compose a solution to provide accessibility content in Theaters for poeple with visual and/or audible limitations. 

Essentially it includes:
* **Subtitle editor:** to edit SRT subtitles and generete audio files for each one of these entries using Festival Text-to-speech.
* **Subtitle & audio server:** that broadcast text and audio content, over a local wireless network to the several mobile devices 
in the Theatre.
* **Mobile client:** used by the Theatre guests, that given a selected language, allowed them to receive text and/or the audio 
content being broadcasted in sync with the play.

The following technologies are used:
* Qt: Used in both the creator and server.
* Android: (v1.6).

## Credits: 

This protoype (developed over 8 weeks in the summer of 2011) was the result of the MSc. Thesis of **Oscar Fernandez** 
in **Multimedia Technologies** @ Universitat Autonoma de Barcelona.
