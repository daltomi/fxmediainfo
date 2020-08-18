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

int main(int argc, char *argv[] )
{
	setlocale(LC_ALL, "");
	
	bindtextdomain("fxmediainfo", "/usr/share/locale/");

	textdomain( "fxmediainfo");
	
	FXApp app("FXMediaInfo","fxmediainfo");
   
	app.init(argc, argv);

	if (not app.isInitialized())
	{
		LOG "App %s not initialized" BEGIN "FXMediaInfo" END
		return EXIT_FAILURE;
	}

	app.setHiliteColor(FXRGB(250, 250, 250));
   
	MainWindow* window = new MainWindow(&app);

	app.addSignal(SIGINT, window, MainWindow::ID_QUIT);

	app.create();

	program::gApp = &app;
	
	program::gWindow = window;
		
	assert((program::gApp != nullptr) and (program::gWindow != nullptr));

	if (argc > 1) 
	{
		MediaInfoPtr mediaInfo = program::makeMediaInfo();
		app.beginWaitCursor();
		
		for (int i = 1; i < argc; ++i)
		{
			if (not window->openFile(argv[i], mediaInfo))
			{
				window->openDirectory(argv[i], mediaInfo);
			}
		}

		app.endWaitCursor();
	}
		
	return app.run();
}

