#include "clientwindow.h"

ClientWindow::ClientWindow()
{
    setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(receivedData()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    sendButton->setEnabled(false);
    username->setEnabled(false);
    message->setEnabled(false);
    label_3->setEnabled(false);
    label_4->setEnabled(false);
    messageSize = 0;
}

void ClientWindow::on_connexionButton_clicked()
{
    messageList->append(tr("<em>Tentative de connexion en cours</em>"));
    connexionButton->setEnabled(false);

    socket->abort();
    socket->connectToHost(serverIP->text(), serverPort->value());
}

void ClientWindow::on_sendButton_clicked()
{
    QByteArray package;
    QDataStream out(&package, QIODevice::WriteOnly);

    QString messageToSend = tr("<strong>") + username->text() +tr("</strong> : ") + message->text();

    out << (quint16) 0;
    out << messageToSend;
    out.device()->seek(0);
    out << (quint16) (package.size() - sizeof(quint16));

    socket->write(package);

    message->clear();
    message->setFocus();
}

void ClientWindow::on_message_returnPressed()
{
    on_sendButton_clicked();
}

void ClientWindow::receivedData()
{
    QDataStream in(socket);

    if (messageSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> messageSize;
    }

    if (socket->bytesAvailable() < messageSize)
        return;

    QString receivedMessage;
    in >> receivedMessage;

    messageList->append(receivedMessage);

    messageSize = 0;
}

void ClientWindow::connected()
{
    messageList->append(tr("<em>Connexion réussie !</em>"));
    sendButton->setEnabled(true);
    username->setEnabled(true);
    message->setEnabled(true);
    label_3->setEnabled(true);
    label_4->setEnabled(true);
}

void ClientWindow::disconnected()
{
    messageList->append(tr("<em>Déconnecté du serveur</em>"));
    sendButton->setEnabled(false);
    username->setEnabled(false);
    message->setEnabled(false);
    label_3->setEnabled(false);
    label_4->setEnabled(false);
}

void ClientWindow::socketError(QAbstractSocket::SocketError error)
{
    switch(error)
    {
    case QAbstractSocket::HostNotFoundError:
        messageList->append(tr("<em>ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.</em>"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        messageList->append(tr("<em>ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.</em>"));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        messageList->append(tr("<em>ERREUR : le serveur a coupé la connexion.</em>"));
        break;
    default:
        messageList->append(tr("<em>ERREUR : ") + socket->errorString() + tr("</em>"));
    }
    connexionButton->setEnabled(true);
}
