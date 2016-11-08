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

// This is custom pallette by @dtroitskiy made for "Frontiers of Fantasy" project specific needs.

#ifndef RME_PALETTE_AUDIO_H_
#define RME_PALETTE_AUDIO_H_

#include "palette_common.h"

class wxColourPickerCtrl;
class wxColourPickerEvent;
class AudioPointBrush;
class AudioAreaBrush;

class AudioPalettePanel : public PalettePanel
{
public:
	AudioPalettePanel(wxWindow * parent, wxWindowID id = wxID_ANY);
	
	PaletteType GetType() const;
	Brush * GetSelectedBrush() const;
	void OnTypeRadioChanged(wxCommandEvent & event);
	void OnAreaColorChanged(wxColourPickerEvent & event);
	void OnSizeSpinChanged(wxSpinEvent & event);
	void OnVolumeSliderScroll(wxScrollEvent & event);
	void OnVolumeTextChanged(wxCommandEvent & event);
	void OnClickPlaceButton(wxCommandEvent & event);
	
	virtual ~AudioPalettePanel();

protected:
	float volumeValue = 0, pauseIntervalValue = 0;

	wxTextCtrl * nameText = NULL;
	wxRadioButton * typePointRadio = NULL, * typeAreaRadio = NULL;
	wxStaticText * colorLabel = NULL;
	wxColourPickerCtrl * areaColorPicker = NULL;
	wxSpinCtrl * sizeSpin = NULL;
	wxSlider * volumeSlider = NULL;
	wxTextCtrl * volumeText = NULL;
	wxCheckBox * isLoopingCheckbox = NULL;
	wxTextCtrl * pauseIntervalText = NULL;

	AudioPointBrush * pointBrush = NULL;
	AudioAreaBrush * areaBrush = NULL;
	Brush * currentBrush = NULL;

	DECLARE_EVENT_TABLE();
};

#endif // RME_PALETTE_AUDIO_H_

