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

#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <wx/valnum.h>
#include <wx/regex.h>
#include "settings.h"
#include "gui.h"
#include "spawn_brush.h"
#include "audio_brush.h"
#include "palette_audio.h"

BEGIN_EVENT_TABLE(AudioPalettePanel, PalettePanel)
	EVT_TEXT(PALETTE_AUDIO_NAME_TEXT, AudioPalettePanel::OnNameTextChanged)
	EVT_RADIOBUTTON(PALETTE_AUDIO_TYPE_POINT_CHECKBOX, AudioPalettePanel::OnTypeRadioChanged)
	EVT_RADIOBUTTON(PALETTE_AUDIO_TYPE_AREA_CHECKBOX, AudioPalettePanel::OnTypeRadioChanged)
	EVT_COLOURPICKER_CHANGED(PALETTE_AUDIO_AREA_COLOR_PICKER, AudioPalettePanel::OnAreaColorChanged)
	EVT_SPINCTRL(PALETTE_AUDIO_SIZE_SPIN, AudioPalettePanel::OnSizeSpinChanged)
	EVT_COMMAND_SCROLL(PALETTE_AUDIO_VOLUME_SLIDER, AudioPalettePanel::OnVolumeSliderScroll)
	EVT_TEXT(PALETTE_AUDIO_VOLUME_TEXT, AudioPalettePanel::OnVolumeTextChanged)
	EVT_CHECKBOX(PALETTE_AUDIO_LOOPING_CHECKBOX, AudioPalettePanel::OnIsLoopingCheckboxChanged)
	EVT_TEXT(PALETTE_AUDIO_PAUSE_INTERVAL_TEXT, AudioPalettePanel::OnPauseIntervalTextChanged)
	EVT_BUTTON(PALETTE_AUDIO_PLACE_BUTTON, AudioPalettePanel::OnClickPlaceButton)
END_EVENT_TABLE()

AudioPalettePanel::AudioPalettePanel(wxWindow * parent, wxWindowID id) : PalettePanel(parent, id)
{
	wxSizer * topSizer = newd wxBoxSizer(wxVERTICAL);
	wxSizer * sideSizer = newd wxStaticBoxSizer(wxVERTICAL, this, wxT("Audio"));

	wxGridBagSizer * gridSizer = newd wxGridBagSizer(10, 10);
	
	int currentRow = 0;
	wxStaticText * nameLabel = newd wxStaticText(this, wxID_ANY, "Name:");
	gridSizer->Add(nameLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	nameText = newd wxTextCtrl(this, PALETTE_AUDIO_NAME_TEXT, settings.getString(Config::AUDIO_NAME));
	gridSizer->Add(nameText, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;
	
	wxStaticText * typeLabel = newd wxStaticText(this, wxID_ANY, "Type:");
	gridSizer->Add(typeLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	wxBoxSizer * hBox = newd wxBoxSizer(wxHORIZONTAL);
	int type = settings.getInteger(Config::AUDIO_TYPE);
	typePointRadio = newd wxRadioButton(this, PALETTE_AUDIO_TYPE_POINT_CHECKBOX, "point", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	typePointRadio->SetValue(type == 0);
	hBox->Add(typePointRadio, 0);
	typeAreaRadio = newd wxRadioButton(this, PALETTE_AUDIO_TYPE_AREA_CHECKBOX, "area");
	typeAreaRadio->SetValue(type == 1);
	hBox->Add(typeAreaRadio, 0, wxLEFT, 5);
	gridSizer->Add(hBox, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;
	
	colorLabel = newd wxStaticText(this, wxID_ANY, "Area color:");
	colorLabel->Enable(type == 1);
	gridSizer->Add(colorLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	wxColour color;
	color.SetRGB(settings.getInteger(Config::AUDIO_COLOR));
	areaColorPicker = newd wxColourPickerCtrl(this, PALETTE_AUDIO_AREA_COLOR_PICKER, color);
	areaColorPicker->Enable(type == 1);
	gridSizer->Add(areaColorPicker, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	wxStaticText * sizeLabel = newd wxStaticText(this, wxID_ANY, "Size:");
	gridSizer->Add(sizeLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	int size = settings.getInteger(Config::AUDIO_SIZE);
	sizeSpin = newd wxSpinCtrl(this, PALETTE_AUDIO_SIZE_SPIN, i2ws(size), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 128, size);
	gridSizer->Add(sizeSpin, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;
	
	wxStaticText * volumeLabel = newd wxStaticText(this, wxID_ANY, "Volume:");
	gridSizer->Add(volumeLabel, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxALIGN_CENTER_VERTICAL);
	++currentRow;
	hBox = newd wxBoxSizer(wxHORIZONTAL);
	volumeValue = settings.getFloat(Config::AUDIO_VOLUME);
	volumeSlider = newd wxSlider(this, PALETTE_AUDIO_VOLUME_SLIDER, round(volumeValue * 100), 0, 100);
	hBox->Add(volumeSlider, 1, wxEXPAND);
	wxFloatingPointValidator <float> volumeValidator(2, &volumeValue, wxNUM_VAL_ZERO_AS_BLANK);
	volumeValidator.SetRange(0, 1.0f);
	volumeText = newd wxTextCtrl(this, PALETTE_AUDIO_VOLUME_TEXT, f2ws(volumeValue), wxDefaultPosition, wxSize(50, -1), wxTE_CENTRE, volumeValidator);
	hBox->Add(volumeText, 0, wxLEFT, 5);
	gridSizer->Add(hBox, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	isLoopingCheckbox = newd wxCheckBox(this, PALETTE_AUDIO_LOOPING_CHECKBOX, "is looping");
	isLoopingCheckbox->SetValue(settings.getInteger(Config::AUDIO_LOOPING));
	gridSizer->Add(isLoopingCheckbox, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxALIGN_CENTER_VERTICAL);
	++currentRow;
		
	wxStaticText * pauseIntervalLabel = newd wxStaticText(this, wxID_ANY, "Pause interval:");
	gridSizer->Add(pauseIntervalLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	pauseIntervalValue = settings.getFloat(Config::AUDIO_PAUSE_INTERVAL);
	wxFloatingPointValidator <float> pauseIntervalValidator(3, &pauseIntervalValue, wxNUM_VAL_ZERO_AS_BLANK);
	pauseIntervalValidator.SetRange(0, 100.0f);
	pauseIntervalText = newd wxTextCtrl(this, PALETTE_AUDIO_PAUSE_INTERVAL_TEXT, f2ws(pauseIntervalValue), wxDefaultPosition, wxDefaultSize, 0, pauseIntervalValidator);
	gridSizer->Add(pauseIntervalText, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	wxButton * placeButton = newd wxButton(this, PALETTE_AUDIO_PLACE_BUTTON, "Place audio");
	gridSizer->Add(placeButton, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	gridSizer->AddGrowableCol(1);
	sideSizer->Add(gridSizer, 0, wxEXPAND);
	topSizer->Add(sideSizer, 0, wxEXPAND);
	SetSizerAndFit(topSizer);
}

PaletteType AudioPalettePanel::GetType() const
{
	return TILESET_AUDIO;
}

void AudioPalettePanel::OnNameTextChanged(wxCommandEvent & event)
{
	if (!nameText) return;
	const wxString & name = nameText->GetValue();
	if (pointBrush) pointBrush->setAudioName(name);
	if (areaBrush) areaBrush->setAudioName(name);
	settings.setString(Config::AUDIO_NAME, name.ToStdString());
}

void AudioPalettePanel::OnTypeRadioChanged(wxCommandEvent & event)
{
	bool isAreaTypeSelected = typeAreaRadio->GetValue();
	colorLabel->Enable(isAreaTypeSelected);
	areaColorPicker->Enable(isAreaTypeSelected);
	if (isAreaTypeSelected)
	{
		currentBrush = areaBrush;
		gui.SetBrushSize(sizeSpin->GetValue());
	}
	else
	{
		currentBrush = pointBrush;
		gui.SetBrushSize(0);
	}
	gui.ActivatePalette(GetParentPalette());
	gui.SelectBrush();
	settings.setInteger(Config::AUDIO_TYPE, (int) isAreaTypeSelected);
}

void AudioPalettePanel::OnAreaColorChanged(wxColourPickerEvent & event)
{
	if (areaBrush)
	{
		const wxColour & color = event.GetColour();
		areaBrush->setAreaColor(color);
		wxColour brushColor(color.Red(), color.Green(), color.Blue(), 128);
		areaBrush->setColor(brushColor);
		settings.setInteger(Config::AUDIO_COLOR, color.GetRGB());
	}
}

void AudioPalettePanel::OnSizeSpinChanged(wxSpinEvent & event)
{
	int size = event.GetPosition();
	if (pointBrush) pointBrush->setAudioSize(size);
	if (areaBrush) areaBrush->setAudioSize(size);
	if (currentBrush == areaBrush) gui.SetBrushSize(size);
	settings.setInteger(Config::AUDIO_SIZE, size);
}

void AudioPalettePanel::OnVolumeSliderScroll(wxScrollEvent & event)
{
	float volume = event.GetPosition() / 100.0f;
	volumeText->ChangeValue(f2ws(volume));
	if (pointBrush) pointBrush->setAudioVolume(volume);
	if (areaBrush) areaBrush->setAudioVolume(volume);
	settings.setFloat(Config::AUDIO_VOLUME, volume);
}

void AudioPalettePanel::OnVolumeTextChanged(wxCommandEvent & event)
{
	if (!volumeText) return;
	float volume = atof(volumeText->GetValue());
	volumeSlider->SetValue(volume * 100);
	if (pointBrush) pointBrush->setAudioVolume(volume);
	if (areaBrush) areaBrush->setAudioVolume(volume);
	settings.setFloat(Config::AUDIO_VOLUME, volume);
}

void AudioPalettePanel::OnIsLoopingCheckboxChanged(wxCommandEvent & event)
{
	if (!isLoopingCheckbox) return;
	bool looping = isLoopingCheckbox->GetValue();
	if (pointBrush) pointBrush->setAudioLooping(looping);
	if (areaBrush) areaBrush->setAudioLooping(looping);
	settings.setInteger(Config::AUDIO_LOOPING, (int) looping);
}

void AudioPalettePanel::OnPauseIntervalTextChanged(wxCommandEvent & event)
{
	if (!pauseIntervalText) return;
	float pauseInterval = atof(pauseIntervalText->GetValue());
	if (pointBrush) pointBrush->setAudioPauseInterval(pauseInterval);
	if (areaBrush) areaBrush->setAudioPauseInterval(pauseInterval);
	settings.setFloat(Config::AUDIO_PAUSE_INTERVAL, pauseInterval);
}

void AudioPalettePanel::OnClickPlaceButton(wxCommandEvent & event)
{
	const wxString & name = nameText->GetValue();
	int type = typeAreaRadio->GetValue();
	int size = sizeSpin->GetValue();
	unsigned int color = areaColorPicker->GetColour().GetRGB();
	float volume = atof(volumeText->GetValue());
	bool looping = isLoopingCheckbox->GetValue();
	float pauseInterval = atof(pauseIntervalText->GetValue());

	if (name.IsEmpty())
	{
		wxMessageBox("\"Name\" field cannot be empty", "Error", wxOK | wxICON_ERROR);
		return;
	}
	wxRegEx nameRegex("^[a-zA-Z0-9_]+$");
	if (!nameRegex.Matches(name))
	{
		wxMessageBox("\"Name\" field contains incorrect characters", "Error", wxOK | wxICON_ERROR);
		return;
	}

	// at first, saving current input values
	settings.setString(Config::AUDIO_NAME, name.ToStdString());
	settings.setInteger(Config::AUDIO_TYPE, type);
	settings.setInteger(Config::AUDIO_COLOR, color);
	settings.setInteger(Config::AUDIO_SIZE, size);
	settings.setFloat(Config::AUDIO_VOLUME, volume);
	settings.setInteger(Config::AUDIO_LOOPING, (int) looping);
	settings.setFloat(Config::AUDIO_PAUSE_INTERVAL, pauseInterval);

	if (type == 0)
	{
		pointBrush = newd AudioPointBrush();
		pointBrush->setAudioName(name);
		pointBrush->setAudioSize(size);
		pointBrush->setAudioVolume(volume);
		pointBrush->setAudioLooping(looping);
		pointBrush->setAudioPauseInterval(pauseInterval);
		currentBrush = pointBrush;
		gui.SetBrushSize(0);
	}
	else
	{
		areaBrush = newd AudioAreaBrush();
		areaBrush->setAudioName(name);
		areaBrush->setAreaColor(areaColorPicker->GetColour());
		areaBrush->setAudioSize(size);
		areaBrush->setAudioVolume(volume);
		areaBrush->setAudioLooping(looping);
		areaBrush->setAudioPauseInterval(pauseInterval);
		
		wxColour brushColor;
		brushColor.SetRGBA(color | 0x80000000);
		areaBrush->setColor(brushColor);
		currentBrush = areaBrush;
		gui.SetBrushSize(size);
	}

	gui.ActivatePalette(GetParentPalette());
	gui.SelectBrush();
}

Brush * AudioPalettePanel::GetSelectedBrush() const
{
	return currentBrush;
}

AudioPalettePanel::~AudioPalettePanel()
{
}

