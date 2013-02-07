#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>

class QAction;
class QMenu;
class QLabel;
class QTabWidget;

class Worker;

class main_window : public QMainWindow
{
  Q_OBJECT

    public:
  main_window(void);

  //---------------------------------------------------
  // Methods inherited from fichier_client_UI_if class
  //---------------------------------------------------
  // Comunication with user
  bool ask_yes_no_qestion(const std::string & p_title, const std::string & p_question);

  private slots:
  void display_status_message(const QString & p_text);
  void end_of_thread();
 //
  void treat_tab_changed_event(int index);
  void treat_launch_thread_event();
  void treat_stop_thread_event();
  void exit();
  

 private:
  void closeEvent(QCloseEvent *event);
  void create_actions(void);
  void create_menus(void);
  void create_status_bar(void);

  QAction *m_exit_action;
  QAction *m_launch_thread_action;
  QAction *m_stop_thread_action;
  QMenu * m_file_menu;
  QLabel * m_status_label;
  QTabWidget *m_tab_widget;

  Worker * m_worker;
};

#endif
