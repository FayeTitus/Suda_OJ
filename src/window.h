/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include "daemon.h"

class Window : public QWidget
{
	Q_OBJECT
public:
	Window();

protected:
	void closeEvent(QCloseEvent *event);
	bool event(QEvent *event);   

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void waitForQuit();
	void startJudge();
	void stopJudge();
	void showTime();
	void getContestTime(const QDateTime &cdt) { contestTime = cdt; }
	void getContestLength(const QTime &cl) { contestLength = cl; }

private:
	void createActions();
	void createLayouts();
	void remainTime();
	QString toDateTimeString(int secs);

	QSystemTrayIcon *trayIcon;
	QAction *monitorAction;
	QAction *quitAction;
	QMenu *trayIconMenu;
	QPushButton *startButton;
	QPushButton *stopButton;
	QPushButton *quitButton;
	QLabel *currentTimeLabel;
	QLabel *timeRemainsLabel;
	QDateTime contestTime;
	QTime contestLength;

	Daemon *daemon;
	Starter *starter;
	//WatchDog *watchDog;
};

#endif
