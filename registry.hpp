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

#include <cassert>

#include <fx.h>


namespace program {

	static FXApp* gApp = nullptr;

	static const FXchar* SEC_SERVICE		= "Service";
	static const FXchar* KEY_SERVICE		= "Name";

	static const FXchar* SPRUNGE_SERVICE	= "sprunge.us";
	static const FXchar* IXIO_SERVICE		= "ix.io";

	static const FXchar* SEC_ACTION			= "Action";
	static const FXchar* KEY_ACTION			= "Select";

	static const FXuint COPY_ACTION			= 1;
	static const FXuint WEB_ACTION			= 2;

	const FXchar* readService(); 
	const FXbool writeService(const FXchar* value);

	const FXuint readAcction();
	const FXbool writeAction(const FXuint value);
} 


