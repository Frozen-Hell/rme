//////////////////////////////////////////////////////////////////////
// This file is part of Remere's Map Editor
//////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////

// This is custom class by @dtroitskiy made for "Frontiers of Fantasy" project specific needs.

#ifndef RME_AUDIO_H_
#define RME_AUDIO_H_

#include <wx/wx.h>
#include "position.h"

class Audio
{
public:
	enum Type { TYPE_POINT, TYPE_AREA };

	Audio(const wxString & name, Type type, const wxColor & areaColor, int size, float volume, bool repetitive,
	      float playTime, float playTimeRandom, float pauseTime, float pauseTimeRandom);
	
	Audio * deepCopy() const;

	inline const wxString & getName() { return name; }
	inline void setName(const wxString & name) { this->name = name; }
	inline Type getType() { return type; }
	inline void setType(Type type) { this->type = type; }
	inline const wxColor & getAreaColor() { return areaColor; }
	inline void setAreaColor(const wxColor & areaColor) { this->areaColor = areaColor; }
	inline int getSize() { return size; }
	inline void setSize(int size) { this->size = size; }
	inline float getVolume() { return volume; }
	inline void setVolume(float volume) { this->volume = volume; }
	inline bool isRepetitive() { return repetitive; }
	inline void setRepetitive(bool repetitive) { this->repetitive = repetitive; }
	inline float getPlayTime() { return playTime; }
	inline void setPlayTime(float playTime) { this->playTime = playTime; }
	inline float getPlayTimeRandom() { return playTimeRandom; }
	inline void setPlayTimeRandom(float playTimeRandom) { this->playTimeRandom = playTimeRandom; }
	inline float getPauseTime() { return pauseTime; }
	inline void setPauseTime(float pauseTime) { this->pauseTime = pauseTime; }
	inline float getPauseTimeRandom() { return pauseTimeRandom; }
	inline void setPauseTimeRandom(float pauseTimeRandom) { this->pauseTimeRandom = pauseTimeRandom; }
	inline const Position & getPosition() { return position; }
	inline void setPosition(const Position & pos) { this->position = pos; }
	
	inline bool isSelected() const { return selected; }
	inline void select() { selected = true; }
	inline void deselect() { selected = false; }

protected:
	wxString name;
	Type type = TYPE_POINT;
	wxColor areaColor;
	int size = 0;
	float volume = 0;
	bool repetitive = false;
	float playTime = 0, playTimeRandom = 0, pauseTime = 0, pauseTimeRandom = 0;
	Position position;
	bool selected = false;
};

typedef std::list <Audio *> Audios;

#endif // RME_AUDIO_H_
