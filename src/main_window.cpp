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
#include "main_window.h"
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QCloseEvent>
#include <QMenuBar>
#include <QDialog>
#include <QThread>
#include <iostream>
#include <cassert>
#include "worker.h"

using namespace std;

//------------------------------------------------------------------------------
main_window::main_window(void):
  m_exit_action(NULL),
  m_launch_thread_action(NULL),
  m_stop_thread_action(NULL),
  m_file_menu(NULL),
  m_status_label(NULL),
  m_tab_widget(NULL),
  m_worker(NULL)
{
  setWindowTitle(tr("SODA"));
  create_actions();
  create_menus();
  create_status_bar();

  m_tab_widget = new QTabWidget();
  connect(m_tab_widget,SIGNAL(currentChanged(int)),this, SLOT(treat_tab_changed_event(int)));



  setCentralWidget(m_tab_widget);

  setMinimumSize(160, 160);
  resize(1024, 768);
}

//---------------------------------------------------
// Methods inherited from fichier_client_UI_if class
//---------------------------------------------------
// Comunication with user
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void main_window::display_status_message(const QString & p_text)
{
  m_status_label->setText(p_text);
}

//------------------------------------------------------------------------------
void main_window::end_of_thread()
{
  display_status_message("Thread terminated");
  m_stop_thread_action->setEnabled(false);
  //  delete m_worker;
  m_worker = NULL;
  m_launch_thread_action->setEnabled(true);
}

//------------------------------------------------------------------------------
bool main_window::ask_yes_no_qestion(const std::string & p_title, const std::string & p_question)
{
  int l_result = QMessageBox::question(this,
				       p_title.c_str(),
				       p_question.c_str(),
				       QMessageBox::Yes | QMessageBox::Default,
				       QMessageBox::No);
  return l_result == QMessageBox::Yes;
}


//------------------------------------------------------------------------------
void main_window::treat_tab_changed_event(int index)
{
  std::cout << "QtEvent::tab changed" << std::endl ;
  assert(m_tab_widget);
}

//------------------------------------------------------------------------------
void main_window::exit(void)
{
  cout << "QtEvent::Exit" << endl ;
  close();
}

//------------------------------------------------------------------------------
void main_window::treat_stop_thread_event(void)
{
  cout << "QtEvent::Stop_Thread" << endl ;
  m_launch_thread_action->setEnabled(true);
  m_worker->stop_work();
  m_stop_thread_action->setEnabled(false);
}

//------------------------------------------------------------------------------
void main_window::treat_launch_thread_event(void)
{
  cout << "QtEvent::Launch_Thread" << endl ;
  m_launch_thread_action->setEnabled(false);

  m_worker = new Worker;
  connect(m_worker,SIGNAL(display_status_message(const QString & )),this,SLOT(display_status_message(const QString &)));
  QThread *workerThread = new QThread(this);

  connect(workerThread, SIGNAL(started()), m_worker, SLOT(doWork()));
  connect(m_worker,SIGNAL(end_of_execution()),workerThread,SLOT(quit()));

  connect(workerThread, SIGNAL(finished()), this, SLOT(end_of_thread()));
  connect(workerThread, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
  connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));

  //  connect(m_worker, SIGNAL(end_of_execution()), this, SLOT(end_of_thread()));
  m_worker->moveToThread(workerThread);

  // Starts an event loop, and emits workerThread->started()
  workerThread->start();

  m_stop_thread_action->setEnabled(true);
  std::cout << "End of Launch Thread event" << std::endl ;
}

//------------------------------------------------------------------------------
void main_window::closeEvent(QCloseEvent *event)
{
  string l_question("Are you sure want to quit ?");
  if(m_worker)
    {
      l_question += "\nSome thread are still running !\n";
    }
  int l_result = QMessageBox::question(this, tr("Quit"),
				       tr(l_question.c_str()),
				       QMessageBox::Yes | QMessageBox::Default,
				       QMessageBox::No);
  if (l_result == QMessageBox::Yes)
    {
      event->accept();
    }
  else
    {
      event->ignore();
    }
}

//------------------------------------------------------------------------------
void main_window::create_actions(void)
{
  m_launch_thread_action = new QAction(tr("&Launch Thread"),this);
  m_launch_thread_action->setShortcut(tr("Ctrl+L"));
  m_launch_thread_action->setStatusTip(tr("Launch Thread")); 
  connect(m_launch_thread_action,SIGNAL(triggered()),this,SLOT(treat_launch_thread_event()));

  m_stop_thread_action = new QAction(tr("&Stop Thread"),this);
  m_stop_thread_action->setShortcut(tr("Ctrl+L"));
  m_stop_thread_action->setStatusTip(tr("Stop Thread")); 
  connect(m_stop_thread_action,SIGNAL(triggered()),this,SLOT(treat_stop_thread_event()));

  m_exit_action = new QAction(tr("&Quit"),this);
  m_exit_action->setShortcut(tr("Ctrl+Q"));
  m_exit_action->setStatusTip(tr("Quit the application")); 
  connect(m_exit_action,SIGNAL(triggered()),this,SLOT(exit()));
}

//------------------------------------------------------------------------------
void main_window::create_menus(void)
{
  m_file_menu = menuBar()->addMenu(tr("&File"));
  m_file_menu->addAction(m_launch_thread_action);
  m_file_menu->addAction(m_stop_thread_action);
  m_file_menu->addAction(m_exit_action);

  // Manage action activation
  m_exit_action->setEnabled(true);
  m_launch_thread_action->setEnabled(true);
  m_stop_thread_action->setEnabled(false);
}

//------------------------------------------------------------------------------
void main_window::create_status_bar(void)
{
  m_status_label = new QLabel("Status bar");
  statusBar()->addWidget(m_status_label);
}

//EOF
