/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

#include "compiler.h"
#include <QtCore>

void Compiler::execute()
{
	QProcess  compilerProcess;
	compilerProcess.start(compiler, compilerOption);
	if(!compilerProcess.waitForStarted())
		qDebug("Failed to start the compiling process.");

	if(!compilerProcess.waitForFinished())
		qDebug("the compiling process maybe crashed.");

	QByteArray result = compilerProcess.readAllStandardError();
	if(result.isEmpty())
	{
		qDebug("compiling OK.");
		judgeStatus = "Compile OK";
	}
	else
	{
		qDebug("Compile Error");
		judgeStatus = "Compile Error";
		ErrorInfo = result;
	}
}
