/*
   Copyright (c) 2015, daltomi <daltomi@disroot.org>

   This file is part of FXMediaInfo.

	FXMediaInfo is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	FXMediaInfo is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FXMediaInfo.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "help.hpp"


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

FXDEFMAP(HelpDlg) HelpDlgMap[] = {
	FXMAPFUNC(SEL_COMMAND,	HelpDlg::ID_SHOW_CONTRIB, HelpDlg::OnCmdShowContrib),
};


FXIMPLEMENT(HelpDlg, FXDialogBox, HelpDlgMap, ARRAYNUMBER(HelpDlgMap) );

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


const FXchar* const HelpDlg::MSG_ABOUT = {
"MediaInfo front-end by FOX Toolkit\n \
Copyright (c) 2015, daltomi <daltomi@disroot.org>\n \
Email: daltomi@opmbx.org\n" 
};



HelpDlg::HelpDlg(FXWindow* parent) :
	FXDialogBox(parent, _("FXMediaInfo - About..."), DECOR_ALL, 0, 0, 278, 130),
	_textInfo(nullptr),
	_vFrame(nullptr)
{
	_iconText = new FXPNGIcon(getApp(), text_png);
	_iconClose = new FXPNGIcon(getApp(), close_png);
	_iconClose->create();
	_iconText->create();

	_vFrame  = new FXVerticalFrame(this, FRAME_NONE | LAYOUT_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	new FXLabel(_vFrame, MSG_ABOUT, nullptr, LAYOUT_TOP | LAYOUT_CENTER_X);

	new FXSeparator(_vFrame, SEPARATOR_GROOVE | LAYOUT_FILL_X | LAYOUT_TOP);
	
	FXHorizontalFrame* hFrame = new FXHorizontalFrame(_vFrame, FRAME_NONE | LAYOUT_BOTTOM | LAYOUT_FILL_X);
	
	new FXButton(hFrame, _("C&ontributors..."), _iconText, this, ID_SHOW_CONTRIB, LAYOUT_BOTTOM | LAYOUT_LEFT | FRAME_RAISED | FRAME_THICK | ICON_BEFORE_TEXT);

	FXButton* button = new FXButton(hFrame, _("&Close"), _iconClose, this, FXDialogBox::ID_ACCEPT, BUTTON_INITIAL | BUTTON_DEFAULT | 
						ICON_BEFORE_TEXT | LAYOUT_RIGHT | LAYOUT_BOTTOM | FRAME_RAISED | FRAME_THICK);
	
	button->setFocus();
}



HelpDlg::~HelpDlg()
{  }



// - Intenta abrir el archivo contributors.txt para mostrar
// su contenido.
void HelpDlg::setTextInfo()
{
	const FXString dataFile{ program::readContributorsFile() };	
	_textInfo->appendText(dataFile);
}


long HelpDlg::OnCmdShowContrib(FXObject* obj, FXSelector sel, void* data)
{
	const FXshort RESIZE = 100;

	((FXButton*)obj)->hide();

	_textInfo = new FXText(_vFrame, nullptr, 0, LAYOUT_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y | TEXT_READONLY);
	_textInfo->create();
	setTextInfo();
	resize(getWidth() + RESIZE , getHeight() + RESIZE);
	return 1;
}

