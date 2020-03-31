#ifndef RME_AUDIO_BRUSH_H
#define RME_AUDIO_BRUSH_H

#include "brush.h"

using namespace std;

class AudioPointBrush : public Brush
{
public:
	AudioPointBrush();
	virtual ~AudioPointBrush();

	virtual bool canDraw(BaseMap * map, Position pos) const;
	virtual void draw(BaseMap * map, Tile * tile, void * parameter);
	virtual void undraw(BaseMap * map, Tile * tile);

	inline virtual int getLookID() const { return 0; }
	inline virtual string getName() const { return "Audio Point Brush"; }
	inline virtual bool canDrag() const { return true; }
	inline virtual bool canSmear() const { return false; }
	inline virtual bool oneSizeFitsAll() const { return true; }

	inline const wxString & getAudioName() { return audioName; }
	inline void setAudioName(const wxString & audioName) { this->audioName = audioName; }
	inline int getAudioSize() { return audioSize; }
	inline void setAudioSize(int audioSize) { this->audioSize = audioSize; }
	inline float getAudioVolume() { return audioVolume; }
	inline void setAudioVolume(float audioVolume) { this->audioVolume = audioVolume; }
	inline bool isAudioRepetitive() { return audioRepetitive; }
	inline void setAudioRepetitive(bool repetitive) { this->audioRepetitive = repetitive; }
	inline float getAudioPlayTime() { return audioPlayTime; }
	inline void setAudioPlayTime(float playTime) { this->audioPlayTime = playTime; }
	inline float getAudioPlayTimeRandom() { return audioPlayTimeRandom; }
	inline void setAudioPlayTimeRandom(float playTimeRandom) { this->audioPlayTimeRandom = playTimeRandom; }
	inline float getAudioPauseTime() { return audioPauseTime; }
	inline void setAudioPauseTime(float pauseTime) { this->audioPauseTime = pauseTime; }
	inline float getAudioPauseTimeRandom() { return audioPauseTimeRandom; }
	inline void setAudioPauseTimeRandom(float pauseTimeRandom) { this->audioPauseTimeRandom = pauseTimeRandom; }

protected:
	wxString audioName;
	int audioSize = 0;
	float audioVolume = 0;
	bool audioRepetitive = false;
	float audioPlayTime = 0, audioPlayTimeRandom = 0, audioPauseTime = 0, audioPauseTimeRandom = 0;
};

class AudioAreaBrush : public Brush
{
public:
	AudioAreaBrush();
	virtual ~AudioAreaBrush();

	virtual bool canDraw(BaseMap * map, Position pos) const;
	virtual void draw(BaseMap * map, Tile * tile, void * parameter);
	virtual void undraw(BaseMap * map, Tile * tile);

	virtual int getLookID() const { return 0; }
	virtual string getName() const { return "Audio Area Brush"; }
	inline virtual bool canDrag() const { return true; }
	inline virtual bool canSmear() const { return false; }
	inline virtual bool oneSizeFitsAll() const { return true; }

	inline const wxString & getAudioName() { return audioName; }
	inline void setAudioName(const wxString & audioName) { this->audioName = audioName; }
	inline const wxColor & getAreaColor() { return areaColor; }
	inline void setAreaColor(const wxColor & areaColor) { this->areaColor = areaColor; }
	inline const wxColor & getColor() { return brushColor; }
	inline void setColor(const wxColor & color) { this->brushColor = color; }
	inline int getAudioSize() { return audioSize; }
	inline void setAudioSize(int audioSize) { this->audioSize = audioSize; }
	inline float getAudioVolume() { return audioVolume; }
	inline void setAudioVolume(float volume) { this->audioVolume = volume; }
	inline bool isAudioRepetitive() { return audioRepetitive; }
	inline void setAudioRepetitive(bool repetitive) { this->audioRepetitive = repetitive; }
	inline float getAudioPlayTime() { return audioPlayTime; }
	inline void setAudioPlayTime(float playTime) { this->audioPlayTime = playTime; }
	inline float getAudioPlayTimeRandom() { return audioPlayTimeRandom; }
	inline void setAudioPlayTimeRandom(float playTimeRandom) { this->audioPlayTimeRandom = playTimeRandom; }
	inline float getAudioPauseTime() { return audioPauseTime; }
	inline void setAudioPauseTime(float pauseTime) { this->audioPauseTime = pauseTime; }
	inline float getAudioPauseTimeRandom() { return audioPauseTimeRandom; }
	inline void setAudioPauseTimeRandom(float pauseTimeRandom) { this->audioPauseTimeRandom = pauseTimeRandom; }

protected:
	wxString audioName;
	wxColor areaColor, brushColor;
	int audioSize = 0;
	float audioVolume = 0;
	bool audioRepetitive = false;
	float audioPlayTime = 0, audioPlayTimeRandom = 0, audioPauseTime = 0, audioPauseTimeRandom = 0;
};

#endif // RME_AUDIO_BRUSH_H

