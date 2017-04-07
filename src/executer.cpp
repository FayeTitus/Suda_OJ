/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

#include "executer.h"
#include <QtCore>

void Executer::execute()
{
	execProcess = new QProcess;
	execProcess->setStandardInputFile(inputFile);
	execProcess->setStandardOutputFile(outputFilePath);
	execProcess->start(exePath);
	qDebug() << "Time limit: " << timeLimit << "ms";
	if(!execProcess->waitForStarted(timeLimit))
		qDebug() << "failed to start the program " << exePath;
	if(!execProcess->waitForFinished(timeLimit + 500))
	{
		qDebug() << "timeout!";
		execProcess->kill();
	}
	checkRuningStatus();
}

void Executer::checkRuningStatus()
{
	QProcess::ProcessError error;
	error = execProcess->error();
	switch(error)
	{
	case QProcess::FailedToStart:
		//Restricted Function
		judgeStatus = "Restricted Function";
		break;
	case QProcess::Crashed:
		//Runtime Error
		//��windows�У�����������ʱ�쳣���ᵯ��dwin.exe�ĵ��Դ��ڣ�
		//��ʱ������δ������������ʱ�Ľ����رգ�judgeStatus����Ϊ
		//Time Limit Exceeded. 
		judgeStatus = "Runtime Error";
		break;
	case QProcess::Timedout:
		//Time Limit Exceeded
		judgeStatus = "Time Limit Exceeded";
		break;
	case QProcess::UnknownError:
		//ok, go ahead
		qDebug() << "test";
		test();
		break;
	default:
		qDebug() << "default of switch";
	}
}

void Executer::test()
{
	QFile testCaseFile(testCasePath);
	QFile outputFile(outputFilePath);
	while(!testCaseFile.open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << "can't open file " << testCasePath;
	while(!outputFile.open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << "can't open file " << outputFilePath;

	QVector<QString> testCaseVector;
	QVector<QString> outputVector;
	QString word;

	QTextStream t(&testCaseFile);
	while(!t.atEnd())
	{
        	t>>word;
		testCaseVector.push_back(word);
    	}
	QTextStream o(&outputFile);
	while(!o.atEnd())
	{
        	o>>word;
		outputVector.push_back(word);
    	}

	if(testCaseVector.size() != outputVector.size())
	{
		//Wrong Answer
		judgeStatus = "Wrong Answer";
		return;
	}
	else
	{
		int size = testCaseVector.size();
		int i = 0;
		for(i = 0; i < size; ++i)
			if(testCaseVector[i] != outputVector[i])
				break;
		if(i<size)
		{
			//Wrong Answer
			judgeStatus = "Wrong Answer";
			return;
		}
	}
	
	if(!checkPresentation())
		return;
	//Accepted
	judgeStatus = "Accepted";
}

bool Executer::checkPresentation()
{
	QFile testCaseFile(testCasePath);
	QFile outputFile(outputFilePath);
	while(!testCaseFile.open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << "can't open file " << testCasePath;
	while(!outputFile.open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << "can't open file " << outputFilePath;

	QTextStream ts(&testCaseFile);
	QTextStream os(&outputFile);

	QString line;
	QString ats, aos;
	while(!ts.atEnd())
	{
		line = ts.readLine();
		ats += line + "\n";
	}
	while(!os.atEnd())
	{
		line = os.readLine();
		aos += line + "\n";
	}

	qDebug() << "testCase " << ats;
	qDebug() << "output " << aos;

	if( ats != aos )
	{
		//Presentation Error
		judgeStatus = "Presentation Error";
		return false;
	}
	else
		return true;
}

