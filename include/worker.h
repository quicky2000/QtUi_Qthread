#ifndef _WORKER_H_
#define _WORKER_H_

#include <QObject>

class Worker : public QObject
{
  Q_OBJECT
    public:
  Worker(void);
  void stop_work(void);
    signals:
  void display_status_message(const QString & p_text);
  void end_of_execution();
  public slots:
  void doWork(void);
 private:
  bool m_run;
};

#endif //  _WORKER_H_
//EOF
