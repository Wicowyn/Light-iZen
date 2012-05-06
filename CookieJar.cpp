#include "prototype.h"

CookieJar::CookieJar(QWidget *parent) : QNetworkCookieJar(parent){
    QList<QNetworkCookie> list;
    QFile file("cookieJar.txt");
    file.open(QIODevice::ReadOnly);
    QByteArray line;
    line=file.readLine();
    while(!line.isEmpty()){
        list.append(QNetworkCookie::parseCookies(line));
        line=file.readLine();
    }
    file.close();
    setAllCookies(list);
}

CookieJar::~CookieJar(){
    QList<QNetworkCookie> list=allCookies();
    QFile file("cookieJar.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    for(int i=0; i<list.count(); i++) out<<list[i].toRawForm()<<endl;
}
