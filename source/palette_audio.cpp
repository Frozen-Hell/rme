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

#include "main.h"
#include <wx/clrpicker.h>
#include <wx/valnum.h>
#include <wx/regex.h>
#include "events.h"
#include "settings.h"
#include "gui.h"
#include "editor.h"
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
	EVT_CHECKBOX(PALETTE_AUDIO_REPETITIVE_CHECKBOX, AudioPalettePanel::OnIsRepetitiveCheckboxChanged)
	EVT_TEXT(PALETTE_AUDIO_PLAY_TIME_TEXT, AudioPalettePanel::OnRepetiviveParametersChanged)
	EVT_TEXT(PALETTE_AUDIO_PLAY_TIME_RANDOM_TEXT, AudioPalettePanel::OnRepetiviveParametersChanged)
	EVT_TEXT(PALETTE_AUDIO_PAUSE_TIME_TEXT, AudioPalettePanel::OnRepetiviveParametersChanged)
	EVT_TEXT(PALETTE_AUDIO_PAUSE_TIME_RANDOM_TEXT, AudioPalettePanel::OnRepetiviveParametersChanged)
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
	nameText->Bind(wxEVT_SET_FOCUS, &AudioPalettePanel::OnNameTextFocus, this);
	nameText->Bind(wxEVT_KILL_FOCUS, &AudioPalettePanel::OnNameTextBlur, this);
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
	
	wxStaticText * volumeLabel = newd wxStaticText(this, wxID_ANY, "Volume (%):");
	gridSizer->Add(volumeLabel, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxALIGN_CENTER_VERTICAL);
	++currentRow;
	hBox = newd wxBoxSizer(wxHORIZONTAL);
	volumeValue = round(settings.getFloat(Config::AUDIO_VOLUME) * 100);
	volumeSlider = newd wxSlider(this, PALETTE_AUDIO_VOLUME_SLIDER, volumeValue, 0, 200);
	hBox->Add(volumeSlider, 1, wxEXPAND);
	wxIntegerValidator <int> volumeValidator(&volumeValue, wxNUM_VAL_ZERO_AS_BLANK);
	volumeValidator.SetRange(0, 200);
	volumeText = newd wxTextCtrl(this, PALETTE_AUDIO_VOLUME_TEXT, i2ws(volumeValue), wxDefaultPosition, wxSize(50, -1), wxTE_CENTRE, volumeValidator);
	hBox->Add(volumeText, 0, wxLEFT, 5);
	gridSizer->Add(hBox, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	isRepetitiveCheckbox = newd wxCheckBox(this, PALETTE_AUDIO_REPETITIVE_CHECKBOX, "Is repetitive");
	bool repetitive = settings.getInteger(Config::AUDIO_REPETITIVE);
	isRepetitiveCheckbox->SetValue(repetitive);
	gridSizer->Add(isRepetitiveCheckbox, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxALIGN_CENTER_VERTICAL);
	++currentRow;
	
	playTimeLabel = newd wxStaticText(this, wxID_ANY, "Play time:");
	playTimeLabel->Enable(repetitive);
	gridSizer->Add(playTimeLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	playTimeValue = settings.getFloat(Config::AUDIO_PLAY_TIME);
	wxFloatingPointValidator <float> playTimeValidator(3, &playTimeValue, wxNUM_VAL_ZERO_AS_BLANK | wxNUM_VAL_NO_TRAILING_ZEROES);
	playTimeValidator.SetRange(0, 100.0f);
	playTimeText = newd wxTextCtrl(this, PALETTE_AUDIO_PLAY_TIME_TEXT, f2ws(playTimeValue), wxDefaultPosition, wxDefaultSize, 0, playTimeValidator);
	playTimeText->Enable(repetitive);
	gridSizer->Add(playTimeText, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	playTimeRandomLabel = newd wxStaticText(this, wxID_ANY, "Play random:");
	playTimeRandomLabel->Enable(repetitive);
	gridSizer->Add(playTimeRandomLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	playTimeRandomValue = settings.getFloat(Config::AUDIO_PLAY_TIME_RANDOM);
	wxFloatingPointValidator <float> playTimeRandomValidator(3, &playTimeRandomValue, wxNUM_VAL_ZERO_AS_BLANK | wxNUM_VAL_NO_TRAILING_ZEROES);
	playTimeRandomValidator.SetRange(0, 100.0f);
	playTimeRandomText = newd wxTextCtrl(this, PALETTE_AUDIO_PLAY_TIME_RANDOM_TEXT, f2ws(playTimeRandomValue), wxDefaultPosition, wxDefaultSize, 0, playTimeRandomValidator);
	playTimeRandomText->Enable(repetitive);
	gridSizer->Add(playTimeRandomText, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	pauseTimeLabel = newd wxStaticText(this, wxID_ANY, "Pause time:");
	pauseTimeLabel->Enable(repetitive);
	gridSizer->Add(pauseTimeLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	pauseTimeValue = settings.getFloat(Config::AUDIO_PAUSE_TIME);
	wxFloatingPointValidator <float> pauseTimeValidator(3, &pauseTimeValue, wxNUM_VAL_ZERO_AS_BLANK | wxNUM_VAL_NO_TRAILING_ZEROES);
	pauseTimeValidator.SetRange(0, 100.0f);
	pauseTimeText = newd wxTextCtrl(this, PALETTE_AUDIO_PAUSE_TIME_TEXT, f2ws(pauseTimeValue), wxDefaultPosition, wxDefaultSize, 0, pauseTimeValidator);
	pauseTimeText->Enable(repetitive);
	gridSizer->Add(pauseTimeText, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	pauseTimeRandomLabel = newd wxStaticText(this, wxID_ANY, "Pause random:");
	pauseTimeRandomLabel->Enable(repetitive);
	gridSizer->Add(pauseTimeRandomLabel, wxGBPosition(currentRow, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	pauseTimeRandomValue = settings.getFloat(Config::AUDIO_PLAY_TIME);
	wxFloatingPointValidator <float> pauseTimeRandomValidator(3, &pauseTimeRandomValue, wxNUM_VAL_ZERO_AS_BLANK | wxNUM_VAL_NO_TRAILING_ZEROES);
	pauseTimeRandomValidator.SetRange(0, 100.0f);
	pauseTimeRandomText = newd wxTextCtrl(this, PALETTE_AUDIO_PAUSE_TIME_RANDOM_TEXT, f2ws(pauseTimeRandomValue), wxDefaultPosition, wxDefaultSize, 0, pauseTimeRandomValidator);
	pauseTimeRandomText->Enable(repetitive);
	gridSizer->Add(pauseTimeRandomText, wxGBPosition(currentRow, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	wxButton * placeButton = newd wxButton(this, PALETTE_AUDIO_PLACE_BUTTON, "Place audio");
	gridSizer->Add(placeButton, wxGBPosition(currentRow, 0), wxGBSpan(1, 2), wxEXPAND | wxALIGN_CENTER_VERTICAL);
	++currentRow;

	gridSizer->AddGrowableCol(1);
	sideSizer->Add(gridSizer, 0, wxEXPAND);
	topSizer->Add(sideSizer, 0, wxEXPAND);
	SetSizerAndFit(topSizer);

	Mediator::subscribeEvent(RME_EVT_TILES_SELECTED, this);
	Mediator::subscribeEvent(RME_EVT_AUDIO_SELECTED, this);
}

PaletteType AudioPalettePanel::GetType() const
{
	return TILESET_AUDIO;
}

Brush * AudioPalettePanel::GetSelectedBrush() const
{
	return currentBrush;
}

void AudioPalettePanel::OnNameTextFocus(wxFocusEvent & event)
{
	event.Skip();
	gui.DisableHotkeys();
}

void AudioPalettePanel::OnNameTextBlur(wxFocusEvent & event)
{
	event.Skip();
	gui.EnableHotkeys();
}

void AudioPalettePanel::OnNameTextChanged(wxCommandEvent & event)
{
	if (!nameText) return;
	const wxString & name = nameText->GetValue();
	if (pointBrush) pointBrush->setAudioName(name);
	if (areaBrush) areaBrush->setAudioName(name);
	if (selectedAudio) selectedAudio->setName(name);
	settings.setString(Config::AUDIO_NAME, name.ToStdString());
}

void AudioPalettePanel::OnTypeRadioChanged(wxCommandEvent & event)
{
	if (isChangingWhileSelection) return;

	bool isAreaTypeSelected = typeAreaRadio->GetValue();
	colorLabel->Enable(isAreaTypeSelected);
	areaColorPicker->Enable(isAreaTypeSelected);
	if (isAreaTypeSelected)
	{
		currentBrush = areaBrush;
		gui.SetBrushSize(sizeSpin->GetValue());
		if (selectedAudio)
		{
			selectedAudio->setType(Audio::TYPE_AREA);
			// if previously audio was point, we need to set some color for the area
			// so we're picking last picked color
			if (selectedAudio->getAreaColor() == *wxBLACK)
			{
				selectedAudio->setAreaColor(areaColorPicker->GetColour());
			}
			gui.RefreshView();
		}
	}
	else
	{
		currentBrush = pointBrush;
		gui.SetBrushSize(0);
		if (selectedAudio)
		{
			selectedAudio->setType(Audio::TYPE_POINT);
			gui.RefreshView();
		}
	}
	gui.ActivatePalette(GetParentPalette());
	gui.SelectBrush();
	settings.setInteger(Config::AUDIO_TYPE, (int) isAreaTypeSelected);
}

void AudioPalettePanel::OnAreaColorChanged(wxColourPickerEvent & event)
{
	if (isChangingWhileSelection) return;

	const wxColour & color = event.GetColour();
	if (areaBrush)
	{
		areaBrush->setAreaColor(color);
		wxColour brushColor(color.Red(), color.Green(), color.Blue(), 128);
		areaBrush->setColor(brushColor);
		settings.setInteger(Config::AUDIO_COLOR, color.GetRGB());
	}
	if (selectedAudio && selectedAudio->getType() == Audio::TYPE_AREA)
	{
		selectedAudio->setAreaColor(color);
		gui.RefreshView();
	}
}

void AudioPalettePanel::OnSizeSpinChanged(wxSpinEvent & event)
{
	if (isChangingWhileSelection) return;

	int size = event.GetPosition();
	if (pointBrush) pointBrush->setAudioSize(size);
	if (areaBrush) areaBrush->setAudioSize(size);
	if (currentBrush == areaBrush) gui.SetBrushSize(size);
	if (selectedAudio)
	{
		selectedAudio->setSize(size);
		gui.RefreshView();
	}
	settings.setInteger(Config::AUDIO_SIZE, size);
}

void AudioPalettePanel::OnVolumeSliderScroll(wxScrollEvent & event)
{
	if (isChangingWhileSelection) return;

	int volumeValue = event.GetPosition();
	volumeText->ChangeValue(i2ws(volumeValue));
	float volume = volumeValue / 100.0f;
	if (pointBrush) pointBrush->setAudioVolume(volume);
	if (areaBrush) areaBrush->setAudioVolume(volume);
	if (selectedAudio) selectedAudio->setVolume(volume);
	settings.setFloat(Config::AUDIO_VOLUME, volume);
}

void AudioPalettePanel::OnVolumeTextChanged(wxCommandEvent & event)
{
	if (!volumeText) return;
	if (isChangingWhileSelection) return;

	int volumeValue = atoi(volumeText->GetValue());
	volumeSlider->SetValue(volumeValue);
	float volume = volumeValue / 100.0f;
	if (pointBrush) pointBrush->setAudioVolume(volume);
	if (areaBrush) areaBrush->setAudioVolume(volume);
	if (selectedAudio) selectedAudio->setVolume(volume);
	settings.setFloat(Config::AUDIO_VOLUME, volume);
}

void AudioPalettePanel::OnIsRepetitiveCheckboxChanged(wxCommandEvent & event)
{
	if (!isRepetitiveCheckbox) return;
	if (isChangingWhileSelection) return;

	bool repetitive = isRepetitiveCheckbox->GetValue();
	playTimeLabel->Enable(repetitive);
	playTimeText->Enable(repetitive);
	playTimeRandomLabel->Enable(repetitive);
	playTimeRandomText->Enable(repetitive);
	pauseTimeLabel->Enable(repetitive);
	pauseTimeText->Enable(repetitive);
	pauseTimeRandomLabel->Enable(repetitive);
	pauseTimeRandomText->Enable(repetitive);
	if (pointBrush) pointBrush->setAudioRepetitive(repetitive);
	if (areaBrush) areaBrush->setAudioRepetitive(repetitive);
	if (selectedAudio) selectedAudio->setRepetitive(repetitive);
	settings.setInteger(Config::AUDIO_REPETITIVE, (int) repetitive);
}

void AudioPalettePanel::OnRepetiviveParametersChanged(wxCommandEvent & event)
{
	if (!playTimeText || !playTimeRandomText || !pauseTimeText || !pauseTimeRandomText) return;
	if (isChangingWhileSelection) return;

	switch (event.GetId())
	{
		case PALETTE_AUDIO_PLAY_TIME_TEXT:
		{
			float playTime = atof(playTimeText->GetValue());
			if (pointBrush) pointBrush->setAudioPlayTime(playTime);
			if (areaBrush) areaBrush->setAudioPlayTime(playTime);
			if (selectedAudio) selectedAudio->setPlayTime(playTime);
			settings.setFloat(Config::AUDIO_PLAY_TIME, playTime);
		}
		break;
		case PALETTE_AUDIO_PLAY_TIME_RANDOM_TEXT:
		{
			float playTimeRandom = atof(playTimeRandomText->GetValue());
			if (pointBrush) pointBrush->setAudioPlayTimeRandom(playTimeRandom);
			if (areaBrush) areaBrush->setAudioPlayTimeRandom(playTimeRandom);
			if (selectedAudio) selectedAudio->setPlayTimeRandom(playTimeRandom);
			settings.setFloat(Config::AUDIO_PLAY_TIME_RANDOM, playTimeRandom);
		}
		break;
		case PALETTE_AUDIO_PAUSE_TIME_TEXT:
		{
			float pauseTime = atof(pauseTimeText->GetValue());
			if (pointBrush) pointBrush->setAudioPauseTime(pauseTime);
			if (areaBrush) areaBrush->setAudioPauseTime(pauseTime);
			if (selectedAudio) selectedAudio->setPauseTime(pauseTime);
			settings.setFloat(Config::AUDIO_PAUSE_TIME, pauseTime);
		}
		break;
		case PALETTE_AUDIO_PAUSE_TIME_RANDOM_TEXT:
		{
			float pauseTimeRandom = atof(pauseTimeRandomText->GetValue());
			if (pointBrush) pointBrush->setAudioPauseTimeRandom(pauseTimeRandom);
			if (areaBrush) areaBrush->setAudioPauseTimeRandom(pauseTimeRandom);
			if (selectedAudio) selectedAudio->setPauseTimeRandom(pauseTimeRandom);
			settings.setFloat(Config::AUDIO_PAUSE_TIME_RANDOM, pauseTimeRandom);
		}
		break;
	}
}

void AudioPalettePanel::OnClickPlaceButton(wxCommandEvent & event)
{
	const wxString & name = nameText->GetValue();
	int type = typeAreaRadio->GetValue();
	int size = sizeSpin->GetValue();
	unsigned int color = areaColorPicker->GetColour().GetRGB();
	float volume = atoi(volumeText->GetValue()) / 100.0f;
	bool repetitive = isRepetitiveCheckbox->GetValue();
	float playTime = atof(playTimeText->GetValue());
	float playTimeRandom = atof(playTimeRandomText->GetValue());
	float pauseTime = atof(pauseTimeText->GetValue());
	float pauseTimeRandom = atof(pauseTimeRandomText->GetValue());

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
	settings.setInteger(Config::AUDIO_REPETITIVE, (int) repetitive);
	settings.setFloat(Config::AUDIO_PLAY_TIME, playTime);
	settings.setFloat(Config::AUDIO_PLAY_TIME_RANDOM, playTimeRandom);
	settings.setFloat(Config::AUDIO_PAUSE_TIME, pauseTime);
	settings.setFloat(Config::AUDIO_PAUSE_TIME_RANDOM, pauseTimeRandom);

	if (type == 0)
	{
		pointBrush = newd AudioPointBrush();
		pointBrush->setAudioName(name);
		pointBrush->setAudioSize(size);
		pointBrush->setAudioVolume(volume);
		pointBrush->setAudioRepetitive(repetitive);
		pointBrush->setAudioPlayTime(playTime);
		pointBrush->setAudioPlayTimeRandom(playTimeRandom);
		pointBrush->setAudioPauseTime(pauseTime);
		pointBrush->setAudioPauseTimeRandom(pauseTimeRandom);
		currentBrush = pointBrush;
		gui.SetBrushSize(0);
	}
	else
	{
		areaBrush = newd AudioAreaBrush();
		areaBrush->setAudioName(name);
		const wxColour & areaColor = areaColorPicker->GetColour();
		areaBrush->setAreaColor(areaColor);
		areaBrush->setAudioSize(size);
		areaBrush->setAudioVolume(volume);
		areaBrush->setAudioRepetitive(repetitive);
		areaBrush->setAudioPlayTime(playTime);
		areaBrush->setAudioPlayTimeRandom(playTimeRandom);
		areaBrush->setAudioPauseTime(pauseTime);
		areaBrush->setAudioPauseTimeRandom(pauseTimeRandom);
		
		wxColour brushColor(areaColor.Red(), areaColor.Green(), areaColor.Blue(), 128);
		areaBrush->setColor(brushColor);
		currentBrush = areaBrush;
		gui.SetBrushSize(size);
	}

	gui.ActivatePalette(GetParentPalette());
	gui.SelectBrush();
}

void AudioPalettePanel::onEvent(unsigned int eventID, void * eventData)
{
	switch (eventID)
	{
		case RME_EVT_AUDIO_SELECTED:
		{
			Tile * tile = (Tile *) eventData;
			selectedAudio = tile->audio;
			isChangingWhileSelection = true;
			nameText->ChangeValue(selectedAudio->getName());
			Audio::Type type = selectedAudio->getType();
			typePointRadio->SetValue(type == Audio::TYPE_POINT);
			typeAreaRadio->SetValue(type == Audio::TYPE_AREA);
			if (type == Audio::TYPE_AREA)
			{
				areaColorPicker->SetColour(selectedAudio->getAreaColor());
			}
			else
			{
				areaColorPicker->Enable(false);
			}
			sizeSpin->SetValue(selectedAudio->getSize());
			int volume = selectedAudio->getVolume() * 100;
			volumeSlider->SetValue(volume);
			volumeText->ChangeValue(i2ws(volume));
			isRepetitiveCheckbox->SetValue(selectedAudio->isRepetitive());
			playTimeText->ChangeValue(f2ws(selectedAudio->getPlayTime()));
			playTimeRandomText->ChangeValue(f2ws(selectedAudio->getPlayTimeRandom()));
			pauseTimeText->ChangeValue(f2ws(selectedAudio->getPauseTime()));
			pauseTimeRandomText->ChangeValue(f2ws(selectedAudio->getPauseTimeRandom()));
			isChangingWhileSelection = false;

			if (type == Audio::TYPE_POINT)
			{
				if (!pointBrush) pointBrush = newd AudioPointBrush();
				pointBrush->setAudioName(selectedAudio->getName());
				pointBrush->setAudioSize(selectedAudio->getSize());
				pointBrush->setAudioVolume(selectedAudio->getVolume());
				pointBrush->setAudioRepetitive(selectedAudio->isRepetitive());
				pointBrush->setAudioPlayTime(selectedAudio->getPlayTime());
				pointBrush->setAudioPlayTimeRandom(selectedAudio->getPlayTimeRandom());
				pointBrush->setAudioPauseTime(selectedAudio->getPauseTime());
				pointBrush->setAudioPauseTimeRandom(selectedAudio->getPauseTimeRandom());
				currentBrush = pointBrush;
				gui.SetBrushSize(0);
			}
			else if (type == Audio::TYPE_AREA)
			{
				if (!areaBrush) areaBrush = newd AudioAreaBrush();
				areaBrush->setAudioName(selectedAudio->getName());
				const wxColour & areaColor = selectedAudio->getAreaColor();
				areaBrush->setAreaColor(areaColor);
				areaBrush->setAudioSize(selectedAudio->getSize());
				areaBrush->setAudioVolume(selectedAudio->getVolume());
				areaBrush->setAudioRepetitive(selectedAudio->isRepetitive());
				areaBrush->setAudioPlayTime(selectedAudio->getPlayTime());
				areaBrush->setAudioPlayTimeRandom(selectedAudio->getPlayTimeRandom());
				areaBrush->setAudioPauseTime(selectedAudio->getPauseTime());
				areaBrush->setAudioPauseTimeRandom(selectedAudio->getPauseTimeRandom());
		
				wxColour brushColor(areaColor.Red(), areaColor.Green(), areaColor.Blue(), 128);
				areaBrush->setColor(brushColor);
				currentBrush = areaBrush;
				gui.SetBrushSize(selectedAudio->getSize());
			}

			gui.ActivatePalette(GetParentPalette());
			gui.SelectBrush();
		}
		break;
		default:
		{
			selectedAudio = nullptr;
		}
	}
}

AudioPalettePanel::~AudioPalettePanel()
{
	Mediator::unsubscribeAllEvents(this);
}
