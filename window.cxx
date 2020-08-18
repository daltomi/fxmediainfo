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


#include "window.hpp"


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

FXDEFMAP( MainWindow ) MainWindowMap[] = {
	FXMAPFUNC(SEL_COMMAND,      MainWindow::ID_QUIT,        MainWindow::OnCmdQuit),
	FXMAPFUNC(SEL_SIGNAL,       MainWindow::ID_QUIT,        MainWindow::OnCmdQuit),
	FXMAPFUNC(SEL_CLOSE,        MainWindow::ID_TITLE,       MainWindow::OnCmdQuit),
	FXMAPFUNC(SEL_COMMAND,      MainWindow::ID_RECENT_FILES,MainWindow::OnCmdRecentFile),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_OPEN,		MainWindow::OnCmdOpen),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_SAVE,		MainWindow::OnCmdSave),
	FXMAPFUNC(SEL_UPDATE,		MainWindow::ID_SAVE,		MainWindow::OnCmdSave),
	FXMAPFUNC(SEL_SELECTED,		MainWindow::ID_TREE_OFILES,	MainWindow::OnCmdTreeOpenFiles),
	FXMAPFUNCS(SEL_COMMAND,		MainWindow::ID_ABOUT, MainWindow::ID_HELP,	MainWindow::OnCmdHelp), 
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_FONT,		MainWindow::OnCmdFont),
	FXMAPFUNC(SEL_DND_DROP,		MainWindow::ID_TREE_OFILES,	MainWindow::OnCmdDropFiles),
	FXMAPFUNC(SEL_DND_MOTION,	MainWindow::ID_TREE_OFILES,	MainWindow::OnCmdDropMotion),
	FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,	MainWindow::ID_TEXTINFO, MainWindow::OnCmdTextInfoRigthMouse),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_CLEAR_TREE_OFILES, MainWindow::OnCmdClearTreeOpenFiles),
	FXMAPFUNC(SEL_UPDATE,		MainWindow::ID_CLEAR_TREE_OFILES,	MainWindow::OnCmdClearTreeOpenFiles),
};

FXIMPLEMENT( MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap) )

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 



constexpr FXHiliteStyle MainWindow::STYLES[MainWindow::MAX_STYLES];


MainWindow::MainWindow(FXApp* app) : 

	FXMainWindow(app, "FXMediaInfo", nullptr, nullptr, DECOR_ALL, 0, 0, 700, 600),
	_numOpenFiles(0),
	_recentFiles(app),
	_fontTextInfo(nullptr)
{ 
	
	FXMenuPane* menuPane = nullptr;
	FXHorizontalFrame* hFrame = nullptr;
	FXVerticalFrame* vFrame = nullptr;
	FXMenuCommand* menuCmd = nullptr;
	FXMenuTitle* menuTitle = nullptr;
	FXLabel* label = nullptr;
	FXFont* font = nullptr;
	FXButton* button = nullptr;
	FXSplitter* splitter = nullptr;
	FXStatusBar* statusBar = nullptr;
	
	const FXColor COLOR_WHITE = FXColors::Gray99; 
	const FXColor COLOR_BLUE = FXColors::DodgerBlue3; 
	const FXColor COLOR_BLUE_LIGHT = FXColors::DodgerBlue2; 
	const FXColor COLOR_SELECT = FXColors::DodgerBlue1;
	
	setTarget(this);
	setSelector(ID_TITLE);

	_recentFiles.setTarget(this);
	_recentFiles.setSelector(ID_RECENT_FILES);

	// FXAutoPtr's
	_iconMediaInfo = new FXPNGIcon(getApp(), mediainfo_png);
	_iconMediaInfo->blend(FXRGB(255,255,255));
	_iconTitle = new FXPNGIcon(getApp(), title_png);
	_iconTitle->blend(FXRGB(255,255,255));
	_iconOpen = new FXPNGIcon(getApp(), open_png);
	_iconSave = new FXPNGIcon(getApp(), save_png);
	_iconAudio = new FXPNGIcon(getApp(), audio_png);
	_iconVideo = new FXPNGIcon(getApp(), video_png);
	_iconImage = new FXPNGIcon(getApp(), image_png);
	_iconPackage = new FXPNGIcon(getApp(), package_png);
	_iconText = new FXPNGIcon(getApp(), text_png);
	_iconClose = new FXPNGIcon(getApp(), close_png);
	_iconDrop = new FXPNGIcon(getApp(), drop_png);
	_iconBookmark = new FXPNGIcon(getApp(), bookmark_png);
	_iconFont = new FXPNGIcon(getApp(), font_png);
	_iconClear = new FXPNGIcon(getApp(), clear_png);
	
	
	// Tooltips
	new FXToolTip(getApp());
	
	// Status Bar.
	// Por defecto para setNormalText es Ready.
	// El uso de setNormalText aqui es para luego traducirlo con gettext, por ej. Listo.
	statusBar = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | STATUSBAR_WITH_DRAGCORNER);
	statusBar->getStatusLine()->setNormalText(_("Ready."));
	
	_labelStatusOpenFiles = new FXLabel(statusBar, "0", _iconOpen, LAYOUT_RIGHT | LAYOUT_FILL_Y | FRAME_SUNKEN | TEXT_AFTER_ICON); 

	// Menu Bar
	FXMenuBar* menuBar = new FXMenuBar(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_DOCK_SAME);
	setBackColor(menuBar, COLOR_WHITE);
	_objectsToDelete.push(menuBar);

	
	// Menu Pane "File"
	{
	   menuPane = new FXMenuPane(this);
	   _objectsToDelete.push(menuPane);
	   
	   menuTitle = new FXMenuTitle(menuBar, _("&File"), nullptr, menuPane);
	   setBackColor(menuTitle, COLOR_WHITE);

	   menuCmd = new FXMenuCommand(menuPane, _("&Open...\tCtrl-O\tOpen a new media file."), _iconOpen, this, ID_OPEN);
	   setBackColor(menuCmd, COLOR_WHITE);

	   menuCmd = new FXMenuCommand(menuPane, _("&Save...\tCtrl-S\tSave information list."), _iconSave, this, ID_SAVE);
	   setBackColor(menuCmd, COLOR_WHITE);

	   new FXMenuSeparator(menuPane, &_recentFiles, FXRecentFiles::ID_ANYFILES);

	   menuCmd = new FXMenuCommand(menuPane, FXString::null, nullptr, &_recentFiles, FXRecentFiles::ID_FILE_1);
	   setBackColor(menuCmd, COLOR_WHITE);

	   menuCmd = new FXMenuCommand(menuPane, FXString::null, nullptr, &_recentFiles, FXRecentFiles::ID_FILE_2);
	   setBackColor(menuCmd, COLOR_WHITE);

	   menuCmd = new FXMenuCommand(menuPane, FXString::null, nullptr, &_recentFiles, FXRecentFiles::ID_FILE_3);
	   setBackColor(menuCmd, COLOR_WHITE);
	   
	   menuCmd  = new FXMenuCommand(menuPane, _("&Clear recent files\t\tClean list of recently opened files."), _iconClear, &_recentFiles, FXRecentFiles::ID_CLEAR);
	   setBackColor(menuCmd, COLOR_WHITE);
		
	   new FXMenuSeparator(menuPane, &_recentFiles, FXRecentFiles::ID_ANYFILES);
	   
	   menuCmd = new FXMenuCommand(menuPane, _("&Quit\tCtrl-Q\tExit program."), _iconClose, this, ID_QUIT);
	   setBackColor(menuCmd, COLOR_WHITE);

	}

	// Menu Options
	{
		menuPane = new FXMenuPane(this);
		_objectsToDelete.push(menuPane);
		
		menuTitle = new FXMenuTitle(menuBar, _("O&ptions"), nullptr, menuPane, LAYOUT_LEFT);
		setBackColor(menuTitle, COLOR_WHITE);
		
		menuCmd = new FXMenuCommand(menuPane, _("&Change Font...\t\tChange font type."), _iconFont, this, ID_FONT);
		setBackColor(menuCmd, COLOR_WHITE);
		
		new FXSeparator(menuPane);
		
		menuCmd = new FXMenuCommand(menuPane, _("C&lear open files\t\tClear list of open files."), _iconClear, this, ID_CLEAR_TREE_OFILES);
		setBackColor(menuCmd, COLOR_WHITE);		
	}

	// Menu Pane "Help"
	{
		menuPane = new FXMenuPane(this);
		_objectsToDelete.push(menuPane);

		menuTitle = new FXMenuTitle(menuBar, _("&Help"), nullptr, menuPane, LAYOUT_RIGHT);
		setBackColor(menuTitle, COLOR_WHITE);

		menuCmd = new FXMenuCommand(menuPane, _("Help...\tF1\tShow html help"), nullptr, this, ID_HELP);
		menuCmd->setTextColor(FXColors::Blue);
		setBackColor(menuCmd, COLOR_WHITE);
		
		menuCmd = new FXMenuCommand(menuPane, _("&About\t\tShow dialog About."), nullptr, this, ID_ABOUT);
		setBackColor(menuCmd, COLOR_WHITE);
	}


	// Title/ Description
	{
		hFrame = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,0,0,0,0,15,5,5,3); 
		setBackColor(hFrame, COLOR_WHITE);
			

		{ // Title
			label = new FXLabel(hFrame,FXString::null, _iconTitle ,ICON_ABOVE_TEXT|LAYOUT_CENTER_X|JUSTIFY_LEFT |LAYOUT_FILL_X,0,0,0,0,0,0,0,0);
			label->setBackColor(FXRGB(255,255,255));
		}

		{ // Description
			font = makeFont(8, FXFont::Normal);
			label = new FXLabel(hFrame, _("Powered by MediaInfo\nhttp://mediaarea.net/MediaInfo"), _iconMediaInfo, LAYOUT_RIGHT | LAYOUT_CENTER_Y | TEXT_BEFORE_ICON);
						
			label->setFont(font);
			setBackColor(label, COLOR_WHITE);
		}
	}

	new FXSeparator(this, SEPARATOR_GROOVE | LAYOUT_FILL_X);
  
	vFrame = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	setBackColor(vFrame, COLOR_BLUE);

	// Toolbar frame -> Main frame
	{
		hFrame = new FXHorizontalFrame(vFrame, LAYOUT_FILL_X | LAYOUT_SIDE_TOP | PACK_UNIFORM_HEIGHT | PACK_UNIFORM_WIDTH);
		setBackColor(hFrame, COLOR_BLUE);
		
		button = new FXButton(hFrame, _("&Open...\tOpen a new media file.\tOpen a new media file."), _iconOpen, this, ID_OPEN, FX::BUTTON_TOOLBAR | FX::FRAME_RAISED | TEXT_AFTER_ICON);
		button->setTextColor(COLOR_WHITE);
		setBackColor(button, COLOR_BLUE);
		
		button = new FXButton(hFrame, _("&Save...\tSave information list.\tSave information list."), _iconSave, this, ID_SAVE, FX::BUTTON_TOOLBAR | FX::FRAME_RAISED | TEXT_AFTER_ICON);
		button->setTextColor(COLOR_WHITE);
		setBackColor(button, COLOR_BLUE);
	} 
		
	splitter = new FXSplitter(vFrame, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y); //| SPLITTER_TRACKING);// | SPLITTER_REVERSED);
	setBackColor(splitter, COLOR_BLUE_LIGHT);
	
	// List files frame -> Splitter
	{
		vFrame = new FXVerticalFrame(splitter, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 200);
		setBackColor(vFrame, COLOR_BLUE);
		
		label = new FXLabel(vFrame, _("List of open files:"), _iconBookmark, TEXT_AFTER_ICON | LAYOUT_TOP);
		label->setTextColor(COLOR_WHITE);
		setBackColor(label, COLOR_BLUE);
		
		_treeOpenFiles = new FXTreeList(vFrame, this, ID_TREE_OFILES, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP
						| LAYOUT_LEFT | TREELIST_SHOWS_BOXES | TREELIST_SHOWS_LINES | TREELIST_SINGLESELECT);
		
		// Drag&Drop support
		_treeOpenFiles->dropEnable();			
		 
		_treeOpenFiles->setTextColor(COLOR_WHITE);
		_treeOpenFiles->setSelBackColor(COLOR_SELECT);
		setBackColor(_treeOpenFiles, COLOR_BLUE);
		
		_labelDragAndDrop = new FXLabel(vFrame, _("Drag your files here..."), _iconDrop, TEXT_AFTER_ICON | LAYOUT_EXPLICIT | LAYOUT_FILL_X, 1, 150, 200, 30);
		_labelDragAndDrop->setTextColor(FXColors::Gray);
		_labelDragAndDrop->dropEnable();
		setBackColor(_labelDragAndDrop, COLOR_BLUE);
	}
				
	// Information frame -> Splitter
	{
		vFrame = new FXVerticalFrame(splitter, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FIX_WIDTH);
		setBackColor(vFrame, COLOR_BLUE);
		
		font = makeFont(12, FXFont::Bold);
		
		_labelInfo = new FXLabel(vFrame, _("Information:"));
		_labelInfo->setFont( font );
		_labelInfo->setTextColor(COLOR_WHITE);
		setBackColor(_labelInfo, COLOR_BLUE);
		
		_labelFile = new FXLabel(vFrame, _("File:"));
		_labelFile->setTextColor(COLOR_WHITE);
		setBackColor(_labelFile, COLOR_BLUE);	
		
		 // - Fuente inicial para textinfo
		 // - Intenta leer desde el registro.
		 const FXchar* regFontName;
		 regFontName = getApp()->reg().readStringEntry("Fonts", "TextInfo", "Courier,80,normal,normal");
		 
		_fontTextInfo = makeFont(regFontName);
		
		_textInfo = new FXText(vFrame, this, ID_TEXTINFO, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_LEFT);
		_textInfo->setEditable(false);
		_textInfo->setFont(_fontTextInfo);
		_textInfo->setHiliteStyles(const_cast<FXHiliteStyle*>(STYLES));
		_textInfo->setStyled(true);
	}
}



// UNUSED 
MainWindow::MainWindow() 
{ }



MainWindow::~MainWindow()
{
	for (auto i = 0; i < _objectsToDelete.no(); ++i)
	{
		delete _objectsToDelete[i];
	}

	delete _fontTextInfo;

    program::FreePackFiles();
}



// Override
void MainWindow::create()
{
	FXMainWindow::create();
   
   	_iconMediaInfo->create();
	_iconOpen->create();
	_iconSave->create();
	_iconAudio->create();
	_iconVideo->create();
	_iconImage->create();
	_iconPackage->create();
	_iconText->create();  
	_iconClose->create();
	_iconDrop->create();
	_iconBookmark->create();
	_iconFont->create();
	_iconClear->create();
   
	const FXString version{ program::getVersionApp() };

	setTitle(version);
	
	if (not urilistType)
	{ 
		urilistType=getApp()->registerDragType(urilistTypeName);
	}
	
	show(PLACEMENT_SCREEN);
}



// - Server-side free resources.
// - Existe por el soporte a Drag & Drop
void MainWindow::detach()
{
	FXMainWindow::detach();
	urilistType = 0;
}


// ID_QUIT | ID_TITLE Events
long MainWindow::OnCmdQuit(FXObject* obj, FXSelector sel, void* data)
{
	getApp()->exit(0);
	return 1;
}



// ID_RECENT_FILES Events
long MainWindow::OnCmdRecentFile(FXObject* obj, FXSelector sel, void* data)
{
	getApp()->beginWaitCursor();
	
	MediaInfoPtr mediaInfo = program::makeMediaInfo();

	assert(data != nullptr);
	
	openFile((const FXchar*)data, mediaInfo);
	
	getApp()->endWaitCursor();
	
	return 1;
}


// ID_SAVE Events
long MainWindow::OnCmdOpen(FXObject* obj, FXSelector sel, void* data)
{
	static FXString lastDirectory = FXSystem::getHomeDirectory();

	FXFileDialog openDlg(this, _("FXMediaInfo - Open multimedia files"));
	
	openDlg.setSelectMode(SELECTFILE_MULTIPLE_ALL);
	openDlg.setDirectory(lastDirectory);
	
	if (not openDlg.execute()) { return 1; }
	
	const FXString* listFiles = openDlg.getFilenames();
	
	if (nullptr != listFiles)
	{
		lastDirectory = openDlg.getDirectory();
		
		MediaInfoPtr mediaInfo = program::makeMediaInfo();
		
		getApp()->beginWaitCursor();

		for (auto i = 0; not listFiles[i].empty(); ++i)
		{
			if (not openFile(listFiles[i].text(), mediaInfo))
			{
				openDirectory(listFiles[i].text(), mediaInfo);
			}
		}
		
		getApp()->endWaitCursor();
		
		delete [] listFiles;
	}

	return 1;
}



// ID_SAVE Events
long MainWindow::OnCmdSave(FXObject* obj, FXSelector sel, void* data)
{
	static FXString fileNameToSave("FXMediaInfo.txt");
	
	switch (FXSELTYPE(sel))
	{
		case SEL_UPDATE :
			{
				const FXint length = _textInfo->getLength();
				// Menu item and Button
				obj->handle(this, length ? FXSEL(SEL_COMMAND, ID_ENABLE) : FXSEL(SEL_COMMAND, ID_DISABLE ), nullptr);
				// Cuando se deshabilita se ve mal, asi que lo oculto.
				obj->handle(this, length ? FXSEL(SEL_COMMAND, ID_SHOW) : FXSEL(SEL_COMMAND, ID_HIDE), nullptr);
				break;
			}
			
		case SEL_COMMAND :
			{
				FXFileDialog saveDlg(this, _("FXMediaInfo - Save information") );
				saveDlg.setFilename(fileNameToSave);
				
				if (not saveDlg.execute()) { break; }
				
				if (FXStat::exists(saveDlg.getFilename()))
				{
					if (MBOX_CLICKED_NO == FXMessageBox::question(this,MBOX_YES_NO,
						 _("FXMediaInfo - Save information"), _("Do you want to overwrite the file?")))
					 {
						 break; // No sobrescribir archivo
					 }
				}
		
				fileNameToSave = saveDlg.getFilename();
				getApp()->beginWaitCursor();
				program::saveInformFile(fileNameToSave.text(), _textInfo->getText());
				getApp()->endWaitCursor();
				
				break;
			}
	}
			
	return 1;
}



// - Crea una fuente basada en el tipo de fuente normal.
// - weight: normal, bold, light,...
FXFont* MainWindow::makeFont(const FXival size, const FXival weight)
{
	FXFontDesc fnt_desc = getApp()->getNormalFont()->getFontDesc();
	fnt_desc.size = (fnt_desc.size * size)/10;
	fnt_desc.weight = weight;
	return new FXFont(getApp(), fnt_desc);
}


FXFont* MainWindow::makeFont(const FXString& fontName)
{
	FXFont *font  = new FXFont(getApp(), fontName);
	font->create();
	return font;
}


FXFont* MainWindow::makeFont(const FXFontDesc& fontDesc)
{
	FXFont* font = new FXFont(getApp(), fontDesc);
	font->create();
	return font;
}



// - Actualiza StatusBar con el 
// número de archivos abiertos.
// - numOpenFile static.
void MainWindow::updateStatusOpenFiles()
{
	FXString str;
	
	str.format("%i", _numOpenFiles);
	_labelStatusOpenFiles->setText(str);	
}



// - Actualiza la lista de archivos abiertos.
void MainWindow::updateTreeOpenFiles()
{
	++_numOpenFiles;
	
	_labelDragAndDrop->hide();
	
	updateStatusOpenFiles();
		
	program::PackageFile * pk = program::packFiles.tail();
	
	assert(nullptr != pk);	
	
	FXTreeItem* top = nullptr;
	FXuint iAudioChannel = 1;
	FXuint iTextChannel = 1;
	
	
	// - TODO: esto no seria necesario si en este momento
	// se lanzara el evento SEL_SELECTED
	_labelFile->setText(_("File: ") + pk->fileName);
	_labelInfo->setText(_("General information"));
	_textInfo->setText("");

	for (const auto & it  : pk->vProperty)
	{
			switch (it.first)
			{
				case program::MediaType::GENERAL : // siempre primero
					top = _treeOpenFiles->appendItem(0, pk->fileName , _iconPackage, _iconPackage, (FXptr*)pk);
					break;
					
				case program::MediaType::VIDEO :
					_treeOpenFiles->appendItem(top, _("Video"), _iconVideo, _iconVideo, (FXptr*)pk);
					break;

				case program::MediaType::AUDIO :
					_treeOpenFiles->appendItem(top, _("Audio"), _iconAudio, _iconAudio, (FXptr*)pk);
					break;
					
				case program::MediaType::IMAGE :
					_treeOpenFiles->appendItem(top, _("Image"), _iconImage, _iconImage, (FXptr*)pk);
					break;
					
				case program::MediaType::TEXT :
					_treeOpenFiles->appendItem(top, _("Text"), _iconText, _iconText, (FXptr*)pk);
					break;
					
				case program::MediaType::MENU :
					_treeOpenFiles->appendItem(top, _("Menu"), _iconText, _iconText, (FXptr*)pk);
					break;			
					
				default :
				{
					FXString strChannel;
					
					if (it.first > program::MediaType::AUDIO_CHANNEL and it.first < program::MediaType::TEXT_CHANNEL)
					{
						strChannel.format(_("Audio #%i"), iAudioChannel++);
						_treeOpenFiles->appendItem(top, strChannel, _iconAudio, _iconAudio, (FXptr*)pk);
					}
					else if (it.first > program::MediaType::TEXT_CHANNEL)
					{
						strChannel.format(_("Text #%i"), iTextChannel++);
						_treeOpenFiles->appendItem(top, strChannel, _iconText, _iconText, (FXptr*)pk);						
					}
				}
			}// end switch

			// Acualiza General Informacion
			setTextInfo(it.second);	

	}// end for
	
	_treeOpenFiles->expandTree(top);
	_treeOpenFiles->selectItem(top);
}



// ID_CLEAR_TREE_OFILES event
long MainWindow::OnCmdClearTreeOpenFiles(FXObject* obj, FXSelector sel, void* data)
{
	switch (FXSELTYPE(sel))
	{
		case SEL_UPDATE :
		{
			obj->handle(this, (_treeOpenFiles->getNumItems() ? FXSEL(SEL_COMMAND, ID_ENABLE) : FXSEL(SEL_COMMAND, ID_DISABLE) ), nullptr); 
			break;
		}
		
		case SEL_COMMAND :
		{
			resetDefaults();
		}
	}
	return 1;
}


void MainWindow::resetDefaults()
{
	_numOpenFiles = 0; // static
			
	_treeOpenFiles->clearItems();
	_textInfo->setText("");
	_labelFile->setText(_("File:"));
	_labelInfo->setText(_("Information:"));
	_labelDragAndDrop->show();
	
	updateStatusOpenFiles();

    program::FreePackFiles();
}


// ID_TREE_OFILES Events
// - Procesa únicamente el item selecionado.
long MainWindow::OnCmdTreeOpenFiles(FXObject* obj, FXSelector sel, void* data)
{
	if (FXSELTYPE(sel) == SEL_SELECTED)
	{
		const FXTreeItem* item = _treeOpenFiles->getCurrentItem();
		program::PackageFile* pk = (program::PackageFile*)item->getData();
		
		assert(nullptr != pk);
		
		_labelFile->setText(_("File: ") + pk->fileName);
		_textInfo->setText(""); // Limpiar 

		if (not _treeOpenFiles->isItemExpanded(item) and item->getNumChildren() == 0)
		{
			const FXString label = item->getText();
			
			_labelInfo->setText(label + _(" information"));
			
			if (label == _("Video"))
			{
				setTextInfo(pk->vProperty[program::MediaType::VIDEO]);
			}
			else if (label == _("Audio"))
			{
				setTextInfo(pk->vProperty[program::MediaType::AUDIO]);	
			}
			else if (label == _("Image"))
			{
				setTextInfo(pk->vProperty[program::MediaType::IMAGE]);	
			}
			else if (label == _("Text"))
			{
				setTextInfo(pk->vProperty[program::MediaType::TEXT]);	
			}
			else if (label.contains( _("Audio #")))
			{
				const FXuint iChannel = FXString(label.after('#')).toUInt();
				const FXuint selectChannel = static_cast<FXuint>(program::MediaType::AUDIO_CHANNEL) + iChannel;
				const program::MediaType type = static_cast<program::MediaType>(selectChannel);

				setTextInfo(pk->vProperty[type]);	
			}
			else if (label.contains( _("Text #")))
			{
				const FXuint iChannel = FXString(label.after('#')).toUInt();
				const FXuint selectChannel = static_cast<FXuint>(program::MediaType::TEXT_CHANNEL) + iChannel;
				const program::MediaType type = static_cast<program::MediaType>(selectChannel);

				setTextInfo(pk->vProperty[type]);				
			}
			else if (label == _("Menu"))
			{
				setTextInfo(pk->vProperty[program::MediaType::MENU]);
			}
		}
		else
		{
			_labelInfo->setText(_("General information"));
			
			for (const auto & it : pk->vProperty)
			{	
				setTextInfo(it.second);	
			}
		}
	}
	return 1;
}



// - Muestra dialogo Acerca de..
// - Abre el arch. html con un web browser.
long MainWindow::OnCmdHelp(FXObject* obj, FXSelector sel, void* data)
{
	switch (FXSELID(sel))
	{
		case ID_ABOUT :
		{
			HelpDlg help(this);
			help.execute(PLACEMENT_OWNER);
			break;
		}
		case ID_HELP :
		{
			program::RunXdgOpen runWebBrowser("/usr/share/doc/fxmediainfo/help.html");
			runWebBrowser.run();
		}
	}
	
	return 1;
}


long MainWindow::OnCmdFont(FXObject* obj, FXSelector sel, void* data)
{
	FXFontDialog fontDlg(this, _("FXMediaInfo - Select one font") );
	
	// Ubica en el dialogo la fuente actual de textInfo
	fontDlg.setFontDesc( _textInfo->getFont()->getFontDesc() );
	
	if (fontDlg.execute())
	{
		if (nullptr != _fontTextInfo)
		{
			delete _fontTextInfo;
		}
		
		// Guarda nombre de fuente selecionada en el registro.
		getApp()->reg().writeStringEntry("Fonts", "TextInfo", fontDlg.getFont().text() );
		
		_fontTextInfo = makeFont( fontDlg.getFont() );
		_textInfo->setFont(_fontTextInfo);
		_textInfo->recalc();
	} 
	return 1;
}



// - Sola para _textInfo
// - Asigna un estilo al titulo de la propiedad
const String::size_type  MainWindow::setTextInfoTitle(const String& strProperty, const FXuint style)
{
	const String::size_type iCut = strProperty.find(__T("\n") );
	_textInfo->appendStyledText(strProperty.substr(0, iCut).c_str(), style);
	return iCut;
}



// - Sola para _textInfo
// - Asigna un estilo al titulo de la propiedad y 
// agrega las demás líneas.
void MainWindow::setTextInfo(const String& strInfo)
{
	const String::size_type iCut = setTextInfoTitle(strInfo, 1);
	_textInfo->appendText(&strInfo.c_str()[iCut]);	
}


// ID_TREE_OFILES Events
long MainWindow::OnCmdDropMotion(FXObject* obj, FXSelector sel, void* data)
{
	if (offeredDNDType(FROM_DRAGNDROP, urilistType))
	{
		acceptDrop(DRAG_ACCEPT);
		return 1;
	}
	
  return 0;
}


// ID_TREE_OFILES Events
long MainWindow::OnCmdDropFiles(FXObject* obj, FXSelector sel, void* data)
{
	FXString strData;

	if (DRAG_COPY == inquireDNDAction() and getDNDData(FROM_DRAGNDROP, urilistType, strData))
	{
		FXint n = 0;
		FXString fileName;
		
		MediaInfoPtr mediaInfo = program::makeMediaInfo();
		
		getApp()->beginWaitCursor();
		
		for (FXString url = strData.section('\n', n); url.length() > 1; url = strData.section('\n', ++n))
		{
			fileName = FXURL::fileFromURL(url);
			
			if (not openFile(fileName.text(), mediaInfo))
			{
				openDirectory(fileName.text(), mediaInfo);
			}
		} // end for
		
		getApp()->endWaitCursor();
		
		return 1;
	}
	
	return 0;
}


void MainWindow::openDirectory(const FXchar* nameDir, const MediaInfoPtr & mediaInfo)
{
	FXString* fileList = nullptr;
				
	const FXint nfiles = FXDir::listFiles(fileList, nameDir, "*", FXDir::NoParent | FXDir::NoDirs);
	
	for (auto i = 0; i < nfiles; ++i) 
	{ 
		FXString path(nameDir);
		path += /*FX::*/PATHSEPSTRING + fileList[i];
		
		if (true == program::openMediaFile(path.text(), mediaInfo))
		{
			_recentFiles.appendFile(path);
			updateTreeOpenFiles();	
		}				
	}
	
	delete [] fileList; 		
}



bool MainWindow::openFile(const FXchar* nameFile, const MediaInfoPtr & mediaInfo)
{
	if (not FXStat::isDirectory(nameFile))
	{
		if (true == program::openMediaFile(nameFile, mediaInfo))
		{
			_recentFiles.appendFile(nameFile);
			updateTreeOpenFiles();		
			return true;
		}
	}
	
	return false;	
}




// ID_TEXTINFO Events
// - Muestra un menu contextual para copiar texto seleccionado.
long MainWindow::OnCmdTextInfoRigthMouse(FXObject* obj, FXSelector sel, void* data)
{
	const FXEvent* event = static_cast<FXEvent*>(data);
	
	if (not event->moved)
	{
		FXMenuPane menuPopup(this, POPUP_SHRINKWRAP);
		new FXMenuCommand(&menuPopup, _("Copy"), nullptr, _textInfo, FXText::ID_COPY_SEL);
			
		menuPopup.forceRefresh();
		menuPopup.create();
		menuPopup.popup(nullptr, event->root_x, event->root_y);
		
		getApp()->runModalWhileShown(&menuPopup);
	}
	
	return 1;
}
