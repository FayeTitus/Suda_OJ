/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

/********************************************************************
**
** ͨ��ʹ�������̲߳�����ִ����������������������--������ģ�ͣ�����
** Daemon �������ߣ�Starter �������ߡ�������һ�������ߣ���������ߣ�ʹ��
** ���Կ���������г����е�ִ�����񣬻��ǻ�������Ķ��������Զ����Ĳ���
** �����������ȡ��Ӧ�Ļ�����ʻ����Ա���һ����Ŀ���ظ������С�
**
** ���³�����ʱʵ��һ��һ������������ģ�͡�
**
*********************************************************************/

#include <QtCore>
#include <QtSql>
#include "daemon.h"
#include "executer.h"
#include "compiler.h"

const int BufferSize = 50;
int buffer[BufferSize];  
bool isJudgeActivated = false;

QSemaphore freeBuffer(BufferSize); 
QSemaphore usedBuffer;

// ���м�س���ͨ�������� Compiler ��ʵ�����б��룬������ Executer ��
// ʵ����س������У����ԡ�����Ӧ�ĸ������ݱ���Ϣ����Ҫ�Ǳ� status, 
// �� rankList�� 

void judge(int id)
{
	QSqlQuery query;
	QString tempString;

	// ��ȡʹ�õı�����ԡ�
	query.prepare("select compiler from status  where runID = :id");
	query.bindValue(":id", id);
	query.exec();
	query.first();
	QString languageName = query.value(0).toString();
	QString compilerName;

	// ������ѡ�ı�����������Ӧ�ı���ѡ��
	QStringList opt;
	if(languageName == "C++")
	{
		compilerName = "g++";
		opt << "-w" << "-o" << "../judge/oexe/temp.exe";
		tempString = "../judge/src/";
		tempString.append(QString::number(id));
		tempString.append(".cpp");
		opt << tempString;
	}
	else if(languageName == "C")
	{
		compilerName = "gcc";
		opt << "-w" << "-o" << "../judge/oexe/temp.exe";
		tempString = "../judge/src/";
		tempString.append(QString::number(id));
		tempString.append(".c");
		opt << tempString;	
	}
	else if(languageName == "Java")
	{
	}
	else	// Pascal
	{
	}

	Compiler compiler(compilerName);
	// set compiler environment & options
	compiler.setCompilerOption(opt);

	compiler.execute();
	tempString = compiler.getStatus();
	if( tempString == "Compile Error" )
	{
		query.prepare("update status set judgeStatus = 'Compile Error', "
			"remarks = :rem where runID = :rid");
		query.bindValue(":rem", compiler.getErrorInfo());
		query.bindValue(":rid", id);
		query.exec();
		// return this function earlier
		return;
	}

	// compile OK, continue
	// first, select the problem ID.
	Executer executer;
	query.prepare("select problemID from status where runID = :rid");
	query.bindValue(":rid", id);
	query.exec();
	query.first();
	QString pidString = query.value(0).toString();	
	// ���������ļ�(stdin)
	tempString = "../judge/cases/in/";
	tempString.append(pidString);
	executer.setInputFile(tempString);
	// ��������ļ�(stdout)
	tempString = "../judge/cases/output/";
	tempString.append(QString::number(id));
	executer.setOutputFile(tempString);
	// ����ִ�г����ļ�
	tempString = "../judge/oexe/";
	tempString.append("temp.exe");
	executer.setExePath(tempString);
	// ����ʱ�䣬�ڴ����Ʋ���
	executer.setTimeLimit();
	executer.setMemoryLimit();
	// ���ñ�׼���Դ��ļ�
	tempString = "../judge/cases/out/";
	tempString.append(pidString);
	executer.setTestCasePath(tempString);

	executer.execute();
	qDebug() << executer.getStatus();

	const QTime startTime(11, 30, 0);   //��ʱ����������һ��
	query.prepare("update status set judgeStatus = :js where runID = :rid");
	query.bindValue(":js", executer.getStatus());
	query.bindValue(":rid", id);
	query.exec();

	if( executer.getStatus() == "Accepted" )
	{
		qDebug() << "Accepted, updating the database.\n";
		query.prepare("update problemLib set accepted = accepted + 1, ratio = accepted / submited "
				"where pid = :pid");
		query.bindValue(":pid", pidString.toInt());
		query.exec();
		
		QString team;
		QTime submitTime;
		query.prepare("select author,submitTime from status where runID = :rid");
		query.bindValue(":rid", id);
		query.exec();
		while(query.next())
		{
			team = query.value(0).toString();
			submitTime = query.value(1).toTime();
		}
			
		QString passTime, submitTimes;
		passTime = "passTime_" + pidString;
		submitTimes = "submitTimes_" + pidString;
		
		QString queryString;
		queryString.append("select solvedNumber, contestTime, ");
		queryString.append(passTime);
		queryString.append(", ");
		queryString.append(submitTimes);
		queryString.append(" from rankList where team = '");
		queryString.append(team);
		queryString.append("'");
		qDebug() << queryString;
		query.exec(queryString);
		
		QTime pTime, cTime; //pass time, contest time
		int sNum(0);  //submit times
		int solved(0); //solved number
		int temp;
		while(query.next())
		{
			solved = query.value(0).toInt();
			cTime = query.value(1).toTime();
			pTime = query.value(2).toTime();
			sNum = query.value(3).toInt();
		}
		
		solved += 1;
		sNum = 1 + sNum;
		temp = startTime.secsTo(QTime(0, 0, 0));
		submitTime = submitTime.addSecs(temp);	
		temp = (-sNum) * 1200;
		pTime = submitTime.addSecs(temp);
		
		//update contestTime  !!ע��,�������˲�Ҫ�ظ��ύ,��Ȼ������.
		temp = pTime.secsTo(QTime(0, 0, 0));
		cTime = cTime.addSecs(-temp);
		
		qDebug() << "cTime: " << cTime;
		qDebug() << "solved: " << solved;
		qDebug() << "sNum: " << sNum;
		
		QString updateString;
		updateString = QString("update rankList set solvedNumber = %1, contestTime = \'").arg(solved);
		updateString.append(cTime.toString("HH:mm:ss"));
		updateString.append("\', ");
		updateString.append(passTime);
		updateString.append(" = \'");
		updateString.append(pTime.toString("HH:mm:ss"));
		updateString.append("\', ");
		updateString.append(submitTimes);
		updateString.append(" = ");
		QString str;
		str.setNum(sNum);
		updateString.append(str);
		updateString.append(" where team = \'");
		updateString.append(team);
		updateString.append("\'");
		qDebug() << "updateString" << updateString;
		query.exec(updateString);
	}
}

void Daemon::run()
{
	int i = 0;
	while(isJudgeActivated)
	{		
		// ѡȡ����δ�����е���Ŀ����������뻷�λ�������
		QSqlQuery query("select runID from status  where judgeStatus = 'Waiting'");
		int index = 0;
		while(query.next())
		{
			index = query.value(0).toInt();
			freeBuffer.acquire();
			buffer[i % BufferSize] = index;
			QSqlQuery q;
			q.prepare("update status set judgeStatus = 'Judging' where runID = :rid");
			q.bindValue(":rid", index);
			q.exec();
			++i;
			usedBuffer.release();
		}
		msleep(500);
	}
}

void Starter::run()
{
	int i = 0;
	while(isJudgeActivated)
	{
		// ѡȡһ����Ŀ�������С�
		int id;
		usedBuffer.acquire();
		id = buffer[i % BufferSize];
		++i;
		qDebug() << "runID" << id;
		freeBuffer.release();
		judge(id);
	}
}

void WatchDog::run()
{
	QProcess watchDogProcess;
	while(isJudgeActivated)
	{
		watchDogProcess.execute("taskkill /f /im dwwin.exe");
		msleep(1000);
	}
}

