/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

#ifndef EXECUTER_H
#define EXECUTER_H

#include <QtCore>
class Executer{
public:
	Executer() {};
	void setInputFile(const QString &file) { inputFile = file; }
	void setOutputFile(const QString &file) { outputFilePath = file; }
	void setExePath(const QString &path) { exePath = path; }
	void setTimeLimit(int limit = 1000) { timeLimit = limit; }
	void setMemoryLimit(int limit = 32) { memoryLimit = limit; }
	void setTestCasePath(const QString &path) { testCasePath = path; }
	void execute();	
	void checkRuningStatus();
	void test();
	bool checkPresentation();
	QString getStatus() const { return judgeStatus; }

private:
	int timeLimit;
	int memoryLimit;
	QString exePath;
	QString inputFile;
	QString outputFilePath;
	QString testCasePath;
	QProcess *execProcess;
	QString judgeStatus;
};

#endif
