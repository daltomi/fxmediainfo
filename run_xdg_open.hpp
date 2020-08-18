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

#include "program.hpp"

namespace program
{

	class RunXdgOpen : public FXRunnable
	{
		public:
			
			explicit RunXdgOpen(const FXchar* url = nullptr);

			virtual ~RunXdgOpen(); 

			virtual int run(); //pure

		private:

			const FXchar* _url;
	};

} //namespace program

