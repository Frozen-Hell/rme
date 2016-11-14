#include "main.h"
#include "audio.h"

Audio::Audio(const wxString & name, Audio::Type type, const wxColor & areaColor, int size, float volume, bool looping, float pauseInterval)
	: name(name), type(type), areaColor(areaColor), size(size), volume(volume), looping(looping), pauseInterval(pauseInterval)
{
}

Audio * Audio::deepCopy() const
{
	Audio * copy = newd Audio(name, type, areaColor, size, volume, looping, pauseInterval);
	copy->selected = selected;
	return copy;
}

