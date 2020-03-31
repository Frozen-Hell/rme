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

#include "mediator.h"
#include "palette_common.h"

class wxColourPickerCtrl;
class wxColourPickerEvent;
class AudioPointBrush;
class AudioAreaBrush;

class AudioPalettePanel : public PalettePanel, public IEventReceiver
{
public:
	AudioPalettePanel(wxWindow * parent, wxWindowID id = wxID_ANY);
	
	PaletteType GetType() const;
	Brush * GetSelectedBrush() const;
	void OnNameTextFocus(wxFocusEvent & event);
	void OnNameTextBlur(wxFocusEvent & event);
	void OnNameTextChanged(wxCommandEvent & event);
	void OnTypeRadioChanged(wxCommandEvent & event);
	void OnAreaColorChanged(wxColourPickerEvent & event);
	void OnSizeSpinChanged(wxSpinEvent & event);
	void OnVolumeSliderScroll(wxScrollEvent & event);
	void OnVolumeTextChanged(wxCommandEvent & event);
	void OnIsRepetitiveCheckboxChanged(wxCommandEvent & event);
	void OnRepetiviveParametersChanged(wxCommandEvent & event);
	void OnClickPlaceButton(wxCommandEvent & event);

	virtual void onEvent(unsigned int eventID, void * eventData);
	
	virtual ~AudioPalettePanel();

protected:
	int volumeValue = 0;
	float playTimeValue = 0, playTimeRandomValue = 0, pauseTimeValue = 0, pauseTimeRandomValue = 0;
	bool isChangingWhileSelection = false;

	wxTextCtrl * nameText = nullptr;
	wxRadioButton * typePointRadio = nullptr, * typeAreaRadio = nullptr;
	wxStaticText * colorLabel = nullptr;
	wxColourPickerCtrl * areaColorPicker = nullptr;
	wxSpinCtrl * sizeSpin = nullptr;
	wxSlider * volumeSlider = nullptr;
	wxTextCtrl * volumeText = nullptr;
	wxCheckBox * isRepetitiveCheckbox = nullptr;
	wxStaticText * playTimeLabel = nullptr, * playTimeRandomLabel = nullptr;
	wxTextCtrl * playTimeText = nullptr, * playTimeRandomText = nullptr;
	wxStaticText * pauseTimeLabel = nullptr, * pauseTimeRandomLabel = nullptr;
	wxTextCtrl * pauseTimeText = nullptr, * pauseTimeRandomText = nullptr;

	AudioPointBrush * pointBrush = nullptr;
	AudioAreaBrush * areaBrush = nullptr;
	Brush * currentBrush = nullptr;
	Audio * selectedAudio = nullptr;

	DECLARE_EVENT_TABLE();
};

#endif // RME_PALETTE_AUDIO_H_

