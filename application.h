#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QVariant>
#include "QCoreApplication"
#include "OICServer.h"
#include <arpa/inet.h>
#include <net/if.h>
#include "QSerialPort"
#include "QSerialPortInfo"



class Application : public QCoreApplication
{
    Q_OBJECT
public:
    explicit Application(int& argc, char *argv[]);
    ~Application();
    OICServer* getServer(){return server;}
    void setSocketFd(int s) { m_socketFd = s;}
    bool isRunning();
signals:

public slots:
    static void* runDiscovery(void* param);
    static void* run(void* param);

    void notifyObservers(QString name, QVariant value);

private:
    bool m_running;
    String convertAddress(sockaddr_in a);
    void send_packet(sockaddr_in destination, COAPPacket* packet);
    void send_packet(COAPPacket* packet);

    void setOutput(quint8 out, quint16 val);




    int m_socketFd;
    QSerialPort* m_serial;
    OICServer* server;
    pthread_t m_thread;
    pthread_t m_discoveryThread;
    quint16 m_front;
};

#endif // APPLICATION_H
