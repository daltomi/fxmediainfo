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


#include "program.hpp"

FXMainWindow* program::gWindow = nullptr;

FXPtrListOf<program::PackageFile> program::packFiles;


MediaInfoPtr program::makeMediaInfo()
{
	MediaInfoPtr mediaInfo(new MediaInfo);
	
	initializeMediaInfo(mediaInfo);

	assert(mediaInfo != nullptr);
	
	return mediaInfo;
}
	
void program::initializeMediaInfo(const MediaInfoPtr& mediaInfo)
{
	mediaInfo->Option(__T("CharSet"), __T(""));
	mediaInfo->Option(__T("LineSeparator"), __T("\n"));
	mediaInfo->Option(__T("Inform"),String());
	mediaInfo->Option(__T("Internet"), __T("No"));
	mediaInfo->Close();
}



// fileName : direccion aboluta.
const String program::getMediaInform(const FXchar* const fileName, const MediaInfoPtr& mediaInfo)
{
	const FXint MAX_PATH_LEN = 4024;

	FXwchar path[MAX_PATH_LEN] = {0};
	
	FX::utf2wcs(path, fileName, strlen(fileName));
	
	if (0 == mediaInfo->Open(String(path)))
		return String();
	
	return mediaInfo->Inform(size_t(-1));
}



// - Rellena packFiles con la info del archivo.
// - Retorna false en caso de error.
// - fileName : direccion aboluta.
bool program::openMediaFile(const FXchar* const fileName, const MediaInfoPtr& mediaInfo)
{
	const String strInform = getMediaInform(fileName, mediaInfo);
	
	if (0 == strInform.length())
	{
		mediaInfo->Close();
		
		FXString msg;
		msg.format(_("File not found.\n%s"), fileName);
		
		FXMessageBox::information(program::gWindow, MBOX_OK, _("FXMediaInfo - Information"), msg.text());
		
		return false;
	}
	
	PackageFile* newPackFile = new PackageFile;
	 
	newPackFile->fileName = FXPath::name(fileName);
	
	std::wstringstream streamLine(strInform);
	String strLine;
	String strProperty;
	MediaType mediaType = MediaType::GENERAL;

	// - Si el archivo de video tiene
	// varios canales de audio, mediaInfo
	// lo informa asi: Audio #1, Audio #2, etc.
	FXuint iAudioChannel = 1;
	FXString strAudioChannel;
	strAudioChannel.format("Audio #%i", iAudioChannel);

	//- Si el archivo de video tiene
	//varios canales de texto, mediaInfo
	//lo informa asi: Text #1, Text #2, etc.
	FXuint iTextChannel = 1;
	FXString strTextChannel;
	strTextChannel.format("Text #%i", iTextChannel);

	while (getline(streamLine, strLine))
	{
		strProperty += strLine.c_str();
		strProperty += __T("\n");

		if (strLine == __T("General"))
		{
			mediaType = MediaType::GENERAL;
		}
		else if (strLine == __T("Video"))
		{
			mediaType = MediaType::VIDEO;
		}
		else if (strLine == __T("Audio"))
		{
			mediaType = MediaType::AUDIO;
		}
		else if (strAudioChannel == strLine.c_str())
		{
			const FXuint newChannel = static_cast<FXuint>( MediaType::AUDIO_CHANNEL ) + iAudioChannel++;
			mediaType = static_cast<MediaType>(newChannel);
			strAudioChannel.format("Audio #%i", iAudioChannel);
		}
		else if (strTextChannel == strLine.c_str())
		{
			const FXuint newChannel = static_cast<FXuint>( MediaType::TEXT_CHANNEL ) + iTextChannel++;
			mediaType = static_cast<MediaType>(newChannel);
			strTextChannel.format("Text #%i", iTextChannel);
		}
		else if (strLine == __T("Image"))
		{
			mediaType = MediaType::IMAGE;
		}
		else if (strLine == __T("Text"))
		{
			mediaType = MediaType::TEXT;
		}
		else if (strLine == __T("Menu"))
		{
			mediaType = MediaType::MENU;
		}
		else
		{
			// Consume las lineas del cuerpo de la propiedad.
			while (getline(streamLine, strLine))
			{
			   strProperty += strLine.c_str();
			   strProperty += __T("\n");

				// ¿ Termina el segmento de la propiedad?
				if (strLine.size() == 0)
				{
					// Los numeros de lineas se guarda finalmente al paquete.
					newPackFile->vProperty[mediaType] = strProperty;
					strProperty.clear();
					break;
				}
			}
		}
	}//end while
	
	// El paquete esta completo.
	packFiles.push(newPackFile);
			
	mediaInfo->Close();
	
	return true;
}



// - Obtiene la version a partir del archivo VERSION.
// - El archivo VERSION se crea con el comando de 
// ArchLinux makepkg.
const FXString program::getVersionApp()
{
	const FXchar* const verFileName = "/usr/share/fxmediainfo/VERSION";
	FXFile file;
	
	// Init and reserve
	FXString verTitle("FXMediaInfo v.xx.yy");
	
	if (true == file.open(verFileName))
	{
		const FXuint SIZE_VERSION = 7; // 'v.xx.yy'
		const FXuint SIZE_TITLE = 12; // 'FXMediaInfo '

		file.readBlock((verTitle.text() + SIZE_TITLE), SIZE_VERSION);
	}
	else 
	{
		LOG "Error open %s file" BEGIN verFileName END
	}

	verTitle.trimEnd(); // clean whitespace

	return verTitle;
}



// - Guarda el informe en un archivo.
void program::saveInformFile(const FXchar* const fileName, const FXString& inform)
{
	FXFile fsave;

	if (true == fsave.open(fileName, FXIO::Writing))
	{
		FXString head;
		head.format("* Generate by FXMediaInfo\n* Date: %s\n\n\n", FXSystem::localTime(FXThread::time()).text());

		fsave.writeBlock(head.text(), head.length());
		fsave.writeBlock(inform.text(), inform.length());
	}
	else
	{
		FXMessageBox::error(program::gWindow, MBOX_OK, _("FXMediaInfo - Save information"), 
			_("Error save file %s.\nPossibly not have permissions."), fileName);
	}

}



// - Lee el archivo contributors.txt 
// - Retorna los datos del archivo.
const FXString program::readContributorsFile()
{
    const FXchar* const fileName = "/usr/share/fxmediainfo/contributors.txt";
    FXString msg("<contributors.txt here>");
    FXFile fileHandle; 
    FXival fileSize = 0;
    
	// No óptimo pero fácil para leer.
	const FXbool isValidOpen = (true == fileHandle.open(fileName, FXIO::Reading));
	const FXbool isValidSize = ((0 != (fileSize = fileHandle.size())) and (fileSize <= 14000)); // 14K = ~300 líneas 

    if (isValidOpen and isValidSize)
    {
    	FXchar data[fileSize + 1] = {'\0'};
    	fileHandle.readBlock(data, fileSize);
    	msg = data; // limpiar y copiar
    }
	else 
	{
		LOG "Error read %s file" BEGIN fileName END
	}

	return msg;
}

void program::FreePackFiles()
{
	for (auto i = 0; i < packFiles.no(); ++i)
	{
		delete packFiles[i];
	}
	packFiles.clear();
}
