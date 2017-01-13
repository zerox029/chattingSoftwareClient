#ifndef HEADER_FENCLIENT
#define HEADER_FENCLIENT

#include <QtWidgets>
#include <QtNetwork>
#include "ui_clientWindow.h"


class ClientWindow : public QWidget, private Ui::ClientWindow
{
    Q_OBJECT

    public:
        ClientWindow();

    private slots:
        void on_connexionButton_clicked();
        void on_sendButton_clicked();
        void on_message_returnPressed();
        void receivedData();
        void connected();
        void disconnected();
        void socketError(QAbstractSocket::SocketError error);

    private:
        QTcpSocket *socket; // Représente le serveur
        quint16 messageSize;
};

#endif
