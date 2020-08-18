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


#pragma once

#include "program.hpp"
#include "icons.hpp"


class HelpDlg : public FXDialogBox
{
	FXDECLARE(HelpDlg)

	public:

		HelpDlg(FXWindow* parent);
		
		virtual ~HelpDlg();

		void setTextInfo();
		long OnCmdShowContrib(FXObject* obj, FXSelector sel, void* data);
		
		enum {
			ID_SHOW_CONTRIB = FXDialogBox::ID_LAST,
		};
		
	private:
	
		static const FXchar* const MSG_ABOUT;	
		FXText* _textInfo; //Not auto pointer
		FXAutoPtr<FXVerticalFrame> _vFrame;
		FXAutoPtr<FXIcon> _iconText;
		FXAutoPtr<FXIcon> _iconClose;
			
	protected:

		HelpDlg() { };
		HelpDlg(const HelpDlg&) = delete;
};
			
