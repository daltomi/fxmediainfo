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


#include "registry.hpp"


const FXchar* program::readService()
{
	assert(nullptr != gApp);
	return gApp->reg().readStringEntry(SEC_SERVICE, KEY_SERVICE, SPRUNGE_SERVICE);
}


const FXbool program::writeService(const FXchar* value)
{
	assert(nullptr != gApp);
	return gApp->reg().writeStringEntry(SEC_SERVICE, KEY_SERVICE, value); 
}


const FXuint program::readAcction()
{
	assert(nullptr != gApp);
	return gApp->reg().readUIntEntry(SEC_ACTION, KEY_ACTION, WEB_ACTION);
}


const FXbool program::writeAction(const FXuint value)
{
	assert(nullptr != gApp);
	return gApp->reg().writeUIntEntry(SEC_ACTION, KEY_ACTION, value);
}
