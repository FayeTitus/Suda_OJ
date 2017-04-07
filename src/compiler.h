/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

#ifndef COMPILER_H
#define COMPILER_H

#include <QtCore>

class Compiler{
public:
	Compiler(const QString &cr = "g++") { compiler = cr; }
	void setCompilerOption(const QStringList &cn) { compilerOption = cn; }
	QString getStatus() const { return judgeStatus; }
	QByteArray getErrorInfo() const { return ErrorInfo; }
	void execute();
private:
	QString compiler;
	QStringList compilerOption;
	QString judgeStatus;
	QByteArray ErrorInfo;
};

#endif
