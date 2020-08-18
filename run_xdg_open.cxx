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

#include "run_xdg_open.hpp"

	
program::RunXdgOpen::RunXdgOpen(const FXchar* url) : _url(url) 
{
	assert(url != nullptr);
}

program::RunXdgOpen::~RunXdgOpen() 
{ }

int program::RunXdgOpen::run()
{
	const FXchar* const command = "/usr/bin/xdg-open";
	const FXchar* const argv[] = { command, _url, nullptr };

	FXProcess process;
	if (not process.start(command, argv))
	{
		FXMessageBox::error(gWindow, MBOX_OK, _("FXMediaInfo - Error"), 
			_("Failed to execute the process:\n%s"), command);

		return 0;
	}
	return 1;
}


