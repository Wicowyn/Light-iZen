#include "NetworkAccessManager.h"

NetworkAccessManager::NetworkAccessManager(QObject *parent) : QNetworkAccessManager(parent){
    qDebug()<<"create";
}

QNetworkReply* NetworkAccessManager::post(const QNetworkRequest &request, QIODevice *data){
    qDebug()<<"Post";
    return QNetworkAccessManager::post(request, data);
}

QNetworkReply* NetworkAccessManager::post(const QNetworkRequest &request, const QByteArray &data){
    qDebug()<<"Post";
    return QNetworkAccessManager::post(request, data);
}

QNetworkReply* NetworkAccessManager::get(const QNetworkRequest &request){
    qDebug()<<"Get";
    return QNetworkAccessManager::get(request);
}
