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

#ifdef DEBUG 
#include <syslog.h>
template<typename... Arguments>
static inline void SysLog(const FXchar* const message, Arguments... argv)
{
    FXString fmt("#Func:%s #Line:%d #Message:");
    fmt += message;
    syslog(LOG_ERR, fmt.text(), argv...);  
}
#else // DEBUG
template<typename... Arguments> __attribute__((unused))
static inline void SysLog(const FXchar* const message, Arguments... argv) 
{ }
#endif // DEBUG

#undef LOG 
#undef BEGIN 
#undef END
#define LOG (SysLog(
#define BEGIN ,__func__, __LINE__,
#define END ));

