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

//Locale
#include <libintl.h>
#include <clocale>

// STL
#include <cassert>
#include <map>

// FOX
#include <fx.h>
#include <FXAutoPtr.h>
#include <signal.h>

// MediaInfo 
// cmake solo reporta -I/usr/include 
#include <MediaInfo/MediaInfo.h>
#include <ZenLib/Ztring.h>

#include "syslog.hpp"
#include "run_xdg_open.hpp"
#include "registry.hpp"

using namespace MediaInfoLib;

using MediaInfoPtr = FXAutoPtr<MediaInfo>;

static inline const FXchar* const _(const FXchar* const STRING) { return gettext(STRING); }

namespace program  
{
	// - Algunos mensajes de texto se generan dentro de program
	// y es necesario para modal dialog.
	extern FXMainWindow* gWindow;
	
	enum class MediaType : FXuint { GENERAL=0, VIDEO, AUDIO, IMAGE, TEXT, MENU, AUDIO_CHANNEL=10, TEXT_CHANNEL=20 };

	struct PackageFile
	{
		FXString fileName;
		std::map<program::MediaType, String> vProperty;
	};

	// Por cada archivo.
	extern FXPtrListOf<PackageFile> packFiles;
	
    void FreePackFiles();

	MediaInfoPtr makeMediaInfo();
	
	void initializeMediaInfo(const MediaInfoPtr& mediaInfo);
	
	const String getMediaInform(const FXchar* const fileName, const MediaInfoPtr& mediaInfo);
	
	bool openMediaFile(const FXchar* const fileName, const MediaInfoPtr& mediaInfo);
	
	const FXString getVersionApp();

	const FXString readContributorsFile();

	void saveInformFile(const FXchar* const fileName, const FXString& inform);

} //namespace program

