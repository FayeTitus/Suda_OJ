/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

#ifndef DAEMON_H
#define DAEMON_H

#include <QtCore>

extern bool isJudgeActivated;

class Daemon : public QThread
{
public:
	void run();
private:

};

class Starter : public QThread
{
public:
	void run();
};

class WatchDog : public QThread
{
public:
	void run();
};

#endif
		
