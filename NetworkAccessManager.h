#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <qDebug>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit NetworkAccessManager(QObject *parent = 0);
    QNetworkReply* post(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply* post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply* get(const QNetworkRequest &request);

signals:

public slots:

};

#endif // NETWORKACCESSMANAGER_H
