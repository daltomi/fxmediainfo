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
#include "help.hpp"


class MainWindow : public FXMainWindow
{
	FXDECLARE(MainWindow)

	public:

		MainWindow(FXApp*);
		MainWindow();
		virtual ~MainWindow();

		long OnCmdQuit(FXObject* obj, FXSelector sel, void* data);
		long OnCmdRecentFile(FXObject* obj, FXSelector sel, void* data);
		long OnCmdOpen(FXObject* obj, FXSelector sel, void* data);
		long OnCmdSave(FXObject* obj, FXSelector sel, void* data);
		long OnCmdTreeOpenFiles(FXObject* obj, FXSelector sel, void* data);
		long OnCmdHelp(FXObject* obj, FXSelector sel, void* data);   
		long OnCmdFont(FXObject* obj, FXSelector sel, void* data);
		long OnCmdDropFiles(FXObject* obj, FXSelector sel, void* data);
		long OnCmdDropMotion(FXObject* obj, FXSelector sel, void* data);
		long OnCmdTextInfoRigthMouse(FXObject* obj, FXSelector sel, void* data);
		long OnCmdClearTreeOpenFiles(FXObject* obj, FXSelector sel, void* data);

		void openDirectory(const FXchar* nameDir, const MediaInfoPtr & mediaInfo);
		bool openFile(const FXchar* nameFile, const MediaInfoPtr & mediaInfo);
		
				
		enum {
			ID_OPEN = FXMainWindow::ID_LAST,
			ID_SAVE,
			ID_TITLE,
			ID_RECENT_FILES,
			ID_QUIT,
			ID_ABOUT,
			ID_HELP,
			ID_TREE_OFILES,
			ID_FONT,
			ID_TEXTINFO,
			ID_CLEAR_TREE_OFILES
		};

	protected:
	
		virtual void create();
		virtual void detach();  //server-side free resources


	private:
		
		FXuint _numOpenFiles;
   	
		FXRecentFiles	_recentFiles;
		FXTreeList*		_treeOpenFiles;
		FXText*		_textInfo;
		FXLabel*	_labelInfo;
		FXLabel*	_labelFile;
		FXLabel*	_labelDragAndDrop;

		
		// - Etiqueta número de archivos abiertos para
		// statusbar.
		FXLabel* _labelStatusOpenFiles;
		
		// - Almacena algunos objetos creados con new que se
		// deben eliminar explicitamente.
		// - Luego se eliminaran desde el destructor.
		// - FXAutoPtr<> no funciono como esperaba.
		// - Ej: FXMenuBar, FXMenuPane deben eliminarse explicitamente.
		FXObjectList _objectsToDelete;
		
		
		// - Los iconos deben eliminarse explicitamente.
		FXAutoPtr<FXIcon> _iconMediaInfo;
		FXAutoPtr<FXIcon> _iconTitle;
		FXAutoPtr<FXIcon> _iconOpen;
		FXAutoPtr<FXIcon> _iconSave;
		FXAutoPtr<FXIcon> _iconPackage;
		FXAutoPtr<FXIcon> _iconAudio;
		FXAutoPtr<FXIcon> _iconImage;
		FXAutoPtr<FXIcon> _iconVideo;
		FXAutoPtr<FXIcon> _iconText;
		FXAutoPtr<FXIcon> _iconClose;
		FXAutoPtr<FXIcon> _iconDrop;
		FXAutoPtr<FXIcon> _iconBookmark;
		FXAutoPtr<FXIcon> _iconFont;
		FXAutoPtr<FXIcon> _iconClear;
 
		FXFont* _fontTextInfo;
		
		static constexpr FXuint MAX_STYLES = 4;
		
		static constexpr FXHiliteStyle STYLES[MAX_STYLES] = { 
				{ // TextInfo titles
					FXColors::Red,       	 // ForeColor
					FXRGB( 255, 255, 255),   // BackColor
					FXRGB( 255, 0, 0 ),      // Selected ForeColor
					FXRGB( 255, 255, 255 ),  // Selected BackColor
					FXRGB( 255, 0, 0 ),      // Hilite ForeColor
					FXRGB( 255, 255, 255 ),  // Hilite BackColor
					FXRGB( 255, 255, 255 ),  // Active BackColor
					FXText::STYLE_UNDERLINE  // Hilite Text Style
				},
				{ // FX color title
					FXColors::Red2,       	 // ForeColor
					FXRGB( 255, 255, 255),   // BackColor
					FXColors::Red2,      	 // Selected ForeColor
					FXRGB( 255, 255, 255 ),  // Selected BackColor
					FXRGB( 255, 0, 0 ),      // Hilite ForeColor
					FXRGB( 255, 255, 255 ),  // Hilite BackColor
					FXRGB( 255, 255, 255 ),  // Active BackColor
					FXText::STYLE_BOLD  	 // Hilite Text Style
				},
				{ // Media color title
					FXColors::Green4,		 // ForeColor
					FXRGB( 255, 255, 255),   // BackColor
					FXColors::Green4,	     // Selected ForeColor
					FXRGB( 255, 255, 255 ),  // Selected BackColor
					FXRGB( 255, 0, 0 ),      // Hilite ForeColor
					FXRGB( 255, 255, 255 ),  // Hilite BackColor
					FXRGB( 255, 255, 255 ),  // Active BackColor
					FXText::STYLE_BOLD  	 // Hilite Text Style	
				},
				{ // Info color title
					FXColors::Blue,       	 // ForeColor
					FXRGB( 255, 255, 255),   // BackColor
					FXColors::Blue,		     // Selected ForeColor
					FXRGB( 255, 255, 255 ),  // Selected BackColor
					FXRGB( 255, 0, 0 ),      // Hilite ForeColor
					FXRGB( 255, 255, 255 ),  // Hilite BackColor
					FXRGB( 255, 255, 255 ),  // Active BackColor
					FXText::STYLE_BOLD  	 // Hilite Text Style
				}
 			};

	private:

		inline void setBackColor(FXWindow* obj, FXival color) 
		{ 
			assert(nullptr != obj);
			obj->setBackColor(color); 
		}
		
		FXFont* makeFont(const FXival size, const FXival weight);
		FXFont* makeFont(const FXString& fontName);
		FXFont* makeFont(const FXFontDesc& fontDesc);
		
		const String::size_type setTextInfoTitle(const String& strProperty, const FXuint style);
		void setTextInfo(const String& strInfo);
		void updateStatusOpenFiles();
		void updateTreeOpenFiles();
		void resetDefaults();
 };

