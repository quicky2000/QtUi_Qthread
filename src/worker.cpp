/*
    This file is part of QtUi_Qthread
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "worker.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

//------------------------------------------------------------------------------
Worker::Worker(void):
  m_run(true)
{
}

//------------------------------------------------------------------------------
void Worker::doWork(void)
{
  std::cout << "Worker::start run" << std::endl ;
  for(unsigned int l_index = 0 ;
      l_index < 10 && m_run ;
      ++l_index)
    {
      std::cout << "Loop " << l_index << std::endl ;      
      std::stringstream l_stream;
      l_stream << "loop " << l_index;
      emit display_status_message(QString(l_stream.str().c_str()));
#ifndef _WIN32
      sleep(1);
#else
      Sleep(1000*1);
#endif

    }
  std::cout << "Worker::End my run" << std::endl ;
  emit end_of_execution();
}


//------------------------------------------------------------------------------
void Worker::stop_work(void)
{
  std::cout << "Workerstop received" << std::endl ;
  m_run = false;
}
//EOF

