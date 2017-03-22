#include "imagestream.h"
#include <QApplication>

int main(int argc,char *argv[])
{
   QApplication a(argc,argv);
   ImageStream *is=new ImageStream();
   is->show();
   return a.exec();
}
