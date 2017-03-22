#ifndef IMAGESTREAM_H_
#define IMAGESTREAM_H_

#include <QWidget>
#include <QLabel>
#include <iostream>
#include <QtGui/QPalette>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include<QTimer>


class ImageStream : public QWidget{
    //public QWidget  这个是必须要写的
   Q_OBJECT
   private:
      QHostAddress hostAddress;   //主机地址
      QTcpSocket client;   //socket对象
      QLabel *ql_show; //建议还是用new，这样在构造函数时，只分配了指针所用的4个字节的空间，再需要用的时候，在动态分配空间，在不需要的时候，将其销毁，可以节省空间，提高效率
      QByteArray imageData;   //存放接收到的图片数据的QByteArray
      QTimer *timer;
      bool hasReadHead;   //是否接收到了当前所接收图片的第一个数据包。
      long avalibleNum;
      void initNetWork();

      fd_set set;
       int max_fds;
   public:
      ImageStream(/*QWidget *qw=0*/);
      ~ImageStream();
      void changeFace();
   public slots:
      void getSocketImage();  //获取从服务器端传来的图片数据
       void askConnect();
       void connectAgain();

};

#endif
