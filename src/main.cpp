/***************************************************************************
**											
**                                ��ţ ��Ʒ    
**                          http://sonew.512j.com/                          
**                      All Rights Reserved  2007.6.1                        
**                                   V1.0                                 
** ������:                                                                  
**         ���´�����ṩ���������ѧϰʹ�ã�����ҵ��;	                    
****************************************************************************/

#include <QtGui>
#include "window.h"
#include "connectsql.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    	if (!createConnection())  //connect to the database.
        	return 1;
	Window window;
	window.show();
	return app.exec();
}


