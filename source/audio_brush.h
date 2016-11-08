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

	inline const wxColor & getColor() const { return color; }
	inline void setColor(const wxColor & color) { this->color = color; }

protected:
	wxColor color;
};

#endif // RME_AUDIO_BRUSH_H

