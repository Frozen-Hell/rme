#include "main.h"
#include "basemap.h"
#include "audio.h"
#include "audio_brush.h"

AudioPointBrush::AudioPointBrush() : Brush()
{
}

AudioPointBrush::~AudioPointBrush()
{
}

bool AudioPointBrush::canDraw(BaseMap * map, Position pos) const
{
	Tile * tile = map->getTile(pos);
	if (tile && tile->audio)
	{
		return false;
	}
	return true;
}

void AudioPointBrush::undraw(BaseMap * map, Tile * tile)
{
	delete tile->audio;
	tile->audio = nullptr;
}

void AudioPointBrush::draw(BaseMap * map, Tile * tile, void * parameter)
{
	ASSERT(tile);
	if (!tile->audio)
	{
		tile->audio = newd Audio(audioName, Audio::TYPE_POINT, *wxBLACK, audioSize, audioVolume, audioRepetitive,
		                         audioPlayTime, audioPlayTimeRandom, audioPauseTime, audioPauseTimeRandom);
		tile->audio->setPosition(tile->getPosition());
	}
}

AudioAreaBrush::AudioAreaBrush() : Brush()
{
}

AudioAreaBrush::~AudioAreaBrush()
{
}

bool AudioAreaBrush::canDraw(BaseMap * map, Position pos) const
{
	Tile * tile = map->getTile(pos);
	if (tile && tile->audio)
	{
		return false;
	}
	return true;
}

void AudioAreaBrush::undraw(BaseMap * map, Tile * tile)
{
	delete tile->audio;
	tile->audio = nullptr;
}

void AudioAreaBrush::draw(BaseMap * map, Tile * tile, void * parameter)
{
	ASSERT(tile);
	if (!tile->audio)
	{
		tile->audio = newd Audio(audioName, Audio::TYPE_AREA, areaColor, audioSize, audioVolume, audioRepetitive,
		                         audioPlayTime, audioPlayTimeRandom, audioPauseTime, audioPauseTimeRandom);
		tile->audio->setPosition(tile->getPosition());
	}
}
