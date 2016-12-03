#include "main.h"
#include "audio.h"

Audio::Audio(const wxString & name, Audio::Type type, const wxColor & areaColor, int size, float volume, bool repetitive,
             float playTime, float playTimeRandom, float pauseTime, float pauseTimeRandom)
	: name(name), type(type), areaColor(areaColor), size(size), volume(volume), repetitive(repetitive),
	  playTime(playTime), playTimeRandom(playTimeRandom), pauseTime(pauseTime), pauseTimeRandom(pauseTimeRandom)
{
}

Audio * Audio::deepCopy() const
{
	Audio * copy = newd Audio(name, type, areaColor, size, volume, repetitive, playTime, playTimeRandom, pauseTime, pauseTimeRandom);
	copy->position = position;
	copy->selected = selected;
	return copy;
}
