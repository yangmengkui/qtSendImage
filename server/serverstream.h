#ifndef IMAGESERVER_H_
#define IMAGESERVER_H_

#include <iostream>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtGui/QImage>
#include <QtCore/QThread>
//#include <QtGui/QWidget>
#include<QWidget>
#include<QTimer>
#include<QCoreApplication>


class ServerStream : public QObject {
   Q_OBJECT
   private:
      QTcpServer server;
      QTcpSocket *socket;   //socket对象
      QTimer *timer;
      void initNetWork();
      void delay(int len);
      bool connectFlag;
      int seconds;
   public:
      ServerStream(QObject *parent=0);
      ~ServerStream();
      QByteArray formHead(QByteArray by);
   public slots:
      void dealConnection();
//      void askConnect();
//      void connectTimeOut();
//      void connectError();
signals:
      void overtime();
};

#endif
