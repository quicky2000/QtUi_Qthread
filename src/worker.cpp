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

