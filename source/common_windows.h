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
// $URL: http://svn.rebarp.se/svn/RME/trunk/source/common_windows.h $
// $Id: common_windows.h 298 2010-02-23 17:09:13Z admin $

#ifndef RME_MAP_WINDOWS_H_
#define RME_MAP_WINDOWS_H_

#include "main.h"

#include "dcbutton.h"

class GameSprite;
class MapTab;

/**
 * A toggle button with an item on it.
 */
class ItemToggleButton : public DCButton 
{
public:
	ItemToggleButton(wxWindow* parent, RenderSize size, int lookid, wxWindowID id = wxID_ANY) :
		DCButton(parent, id, wxDefaultPosition, DC_BTN_TOGGLE, size, lookid) {}
	virtual ~ItemToggleButton() {}
};

/**
 * A button with an item on it.
 */
class ItemButton : public DCButton 
{
public:
	ItemButton(wxWindow* parent, RenderSize size, uint16_t lookid, wxWindowID id = wxID_ANY) :
		DCButton(parent, id, wxDefaultPosition, DC_BTN_NORMAL, size, lookid) {}
	virtual ~ItemButton() {}
};

/**
 * The map properties window
 * Change map size, protocol etc.
 */
class MapPropertiesWindow : public wxDialog 
{
public:
	MapPropertiesWindow(wxWindow* parent, MapTab* tab, Editor& editor);
	virtual ~MapPropertiesWindow();

	void OnChangeVersion(wxCommandEvent&);

	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);

protected:
	void UpdateProtocolList();

	MapTab * view = NULL;
	Editor & editor;
	wxSpinCtrl * height_spin = NULL;
	wxSpinCtrl * width_spin = NULL;
	wxChoice * version_choice = NULL;
	wxChoice * protocol_choice = NULL;
	wxTextCtrl * description_ctrl = NULL;
	wxTextCtrl * house_filename_ctrl = NULL;
	wxTextCtrl * spawn_filename_ctrl = NULL;
	wxTextCtrl * audioFilenameText = NULL;

	DECLARE_EVENT_TABLE();
};

/**
 * The import map dialog
 * Allows selection of file path, offset and some weird options.
 */
class ImportMapWindow : public wxDialog
{
public:
	ImportMapWindow(wxWindow* parent, Editor& editor);
	virtual ~ImportMapWindow();

	void OnClickBrowse(wxCommandEvent&);
	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);
protected:
	Editor& editor;

	wxTextCtrl* file_text_field;

	wxString x_offset, y_offset;
	wxString file_to_import;

	wxChoice* house_options;
	wxChoice* spawn_options;

	DECLARE_EVENT_TABLE();
};

/**
 * The export minimap dialog, select output path and what floors to export.
 */
class ExportMiniMapWindow : public wxDialog
{
public:
	ExportMiniMapWindow(wxWindow* parent, Editor& editor);
	virtual ~ExportMiniMapWindow();

	void OnClickBrowse(wxCommandEvent&);
	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);
	void OnExportTypeChange(wxCommandEvent&);
protected:
	Editor& editor;

	wxTextCtrl* file_text_field;
	wxChoice* floor_options;
	wxSpinCtrl* floor_number;

	DECLARE_EVENT_TABLE();
};

/**
 * Text control that will forward up/down pgup / pgdown keys to parent window
 */
class KeyForwardingTextCtrl : public wxTextCtrl 
{
public:
	KeyForwardingTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value = wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr)
		: wxTextCtrl(parent, id, value, pos, size, style, validator, name) {}
	~KeyForwardingTextCtrl() {}

	void OnKeyDown(wxKeyEvent&);

	DECLARE_EVENT_TABLE()
};

/**
 * The list inside a find dialog
 * Presents a list of brushes
 */
class FindDialogListBox : public wxVListBox 
{
public:
	FindDialogListBox(wxWindow* parent, wxWindowID id);
	~FindDialogListBox();
	
	void Clear();
	void SetNoMatches();
	void AddBrush(Brush*);
	Brush* GetSelectedBrush();

	void OnDrawItem(wxDC& dc, const wxRect& rect, size_t index) const;
	wxCoord OnMeasureItem(size_t index) const;
protected:
	bool cleared;
	bool no_matches;
	std::vector<Brush*> brushlist;
};

/**
 * A generic find dialog
 * ShowModal will return 0 or the item id for item dialogs
 * 0 or 1 for brush dialogs
 */
class FindDialog : public wxDialog 
{
public:
	FindDialog(wxWindow* parent, wxString title);
	virtual ~FindDialog();

	void OnKeyDown(wxKeyEvent&);
	void OnTextChange(wxCommandEvent&);
	void OnTextIdle(wxTimerEvent&);
	void OnClickList(wxCommandEvent&);
	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);

	void RefreshContents();
	virtual const Brush* getResult() const {return result_brush;}
	virtual int getResultID() const {return result_id;}
protected:
	virtual void RefreshContentsInternal() = 0;
	virtual void OnClickListInternal(wxCommandEvent&) = 0;
	virtual void OnClickOKInternal() = 0;

	FindDialogListBox* item_list;
	KeyForwardingTextCtrl* search_field;
	wxTimer idle_input_timer;
	const Brush* result_brush;
	int result_id;

	DECLARE_EVENT_TABLE()
};

/**
 * Find a brush dialog
 * Find out what brush was returned through GetResult
 */
class FindBrushDialog : public FindDialog 
{
public:
	FindBrushDialog(wxWindow* parent, wxString title = wxT("Jump to Brush"));
	virtual ~FindBrushDialog();

	virtual void RefreshContentsInternal();
	virtual void OnClickListInternal(wxCommandEvent&);
	virtual void OnClickOKInternal();
};

/**
 * Select an item dialog
 * Find out what item was selected through return value of ShowModal() or getResultID
 */
class FindItemDialog : public FindDialog 
{
public:
	FindItemDialog(wxWindow* parent, wxString title = wxT("Jump to Item"));
	virtual ~FindItemDialog();

	void setCondition(bool condition(const ItemType&));

	virtual void RefreshContentsInternal();
	virtual void OnClickListInternal(wxCommandEvent&);
	virtual void OnClickOKInternal();

protected:
	bool (*extra_condition)(const ItemType&);
};

/**
 * Select two items
 * Will return the two selected items through GetResultFindID() and GetResultWithID()
 */
class ReplaceItemDialog : public wxDialog
{
public:
	ReplaceItemDialog(wxWindow* parent, wxString title = wxT("Replace Item"));
	virtual ~ReplaceItemDialog();
	
	void OnKeyDown(wxKeyEvent&);
	void OnTextChange(wxCommandEvent&);
	void OnTextIdle(wxTimerEvent&);
	void OnClickList(wxCommandEvent&);
	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);

	uint16_t GetResultFindID() const;
	uint16_t GetResultWithID() const;

protected:
	void RefreshContents(FindDialogListBox *whichList);

	KeyForwardingTextCtrl* find_item_field;
	FindDialogListBox *find_item_list;
	KeyForwardingTextCtrl* with_item_field;
	FindDialogListBox *with_item_list;

	wxTimer find_idle_input_timer;
	wxTimer with_idle_input_timer;
	
	Brush* result_find_brush;
	Brush* result_with_brush;
	
	DECLARE_EVENT_TABLE();
};

/**
 * Jump to position dialog
 * Allows entry of 3 coordinates and goes there instantly
 */
class GotoPositionDialog : public wxDialog 
{
public:
	GotoPositionDialog(wxWindow* parent, Editor& editor);
	~GotoPositionDialog() {}

	void OnTypeText(wxKeyEvent&);
	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);

protected:
	Editor& editor;

	DECLARE_EVENT_TABLE();
};

/**
 * Base for the item properties dialogs
 * There are two versions, one for otbmv4 maps and one for the old maps.
 * They are declared in old_properties_window / properties_window
 */
class ObjectPropertiesWindowBase : public wxDialog
{
public:
	ObjectPropertiesWindowBase(
		wxWindow* parent, wxString title,
		const Map* map, const Tile* tile, Item* item,
		wxPoint position = wxDefaultPosition);
	ObjectPropertiesWindowBase(
		wxWindow* parent, wxString title,
		const Map* map, const Tile* tile, Spawn* spawn,
		wxPoint position = wxDefaultPosition);
	ObjectPropertiesWindowBase(
		wxWindow* parent, wxString title,
		const Map* map, const Tile* tile, Creature* creature,
		wxPoint position = wxDefaultPosition);

	Item* getItemBeingEdited();
	Creature* getCreatureBeingEdited();
	Spawn* getSpawnBeingEdited();

protected:
	const Map* edit_map;
	const Tile* edit_tile;
	Item* edit_item;
	Creature* edit_creature;
	Spawn* edit_spawn;
};

/**
 * The edit towns dialog, ugly as sin.
 */
class EditTownsDialog : public wxDialog 
{
public:
	EditTownsDialog(wxWindow* parent, Editor& editor);
	virtual ~EditTownsDialog();

	void OnListBoxChange(wxCommandEvent&);

	void OnClickSelectTemplePosition(wxCommandEvent&);
	void OnClickAdd(wxCommandEvent&);
	void OnClickRemove(wxCommandEvent&);
	void OnClickOK(wxCommandEvent&);
	void OnClickCancel(wxCommandEvent&);
protected:
	
	void BuildListBox(bool doselect);
	void UpdateSelection(int new_selection);

	Editor& editor;

	std::vector<Town*> town_list;
	uint32_t max_town_id;

	wxListBox* town_listbox;
	wxString town_name, town_id;
	wxString temple_x, temple_y, temple_z;

	wxTextCtrl* name_field;
	wxTextCtrl* id_field;

	wxTextCtrl* x_templepos_field;
	wxTextCtrl* y_templepos_field;
	wxTextCtrl* z_templepos_field;
	wxButton* remove_button;
	wxButton* select_position_button;

	DECLARE_EVENT_TABLE();
};

#endif
