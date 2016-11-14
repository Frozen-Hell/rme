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

#ifndef RME_AUDIO_H_
#define RME_AUDIO_H_

#include <wx/wx.h>

class Audio
{
public:
	enum Type { TYPE_POINT, TYPE_AREA };

	Audio(const wxString & name, Type type, const wxColor & areaColor, int size, float volume, bool looping, float pauseInterval);
	
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
	inline bool isLooping() { return looping; }
	inline void setLooping(bool looping) { this->looping = looping; }
	inline float getPauseInterval() { return pauseInterval; }
	inline void setPauseInterval(float pauseInterval) { this->pauseInterval = pauseInterval; }
	
	inline bool isSelected() const { return selected; }
	inline void select() { selected = true; }
	inline void deselect() { selected = false; }

protected:
	wxString name;
	Type type = TYPE_POINT;
	wxColor areaColor;
	int size = 0;
	float volume = 0;
	bool looping = false;
	float pauseInterval = 0;
	bool selected = false;
};

#endif // RME_AUDIO_H_

