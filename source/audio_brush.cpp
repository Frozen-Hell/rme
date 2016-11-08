#include "main.h"
#include "basemap.h"
#include "spawn.h"
#include "audio_brush.h"

AudioPointBrush::AudioPointBrush() : Brush()
{
}

AudioPointBrush::~AudioPointBrush()
{
}

bool AudioPointBrush::canDraw(BaseMap * map, Position pos) const
{
	return true;
}

void AudioPointBrush::undraw(BaseMap * map, Tile * tile)
{
	
}

void AudioPointBrush::draw(BaseMap * map, Tile * tile, void * parameter)
{
	
}

AudioAreaBrush::AudioAreaBrush() : Brush()
{
}

AudioAreaBrush::~AudioAreaBrush()
{
}

bool AudioAreaBrush::canDraw(BaseMap * map, Position pos) const
{
	return true;
}

void AudioAreaBrush::undraw(BaseMap * map, Tile * tile)
{
	
}

void AudioAreaBrush::draw(BaseMap * map, Tile * tile, void * parameter)
{
	
}

