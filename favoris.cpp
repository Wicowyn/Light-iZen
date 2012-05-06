#include "prototype.h"

Favoris::Favoris(QWidget *parent=0) : QTabWidget(parent), pr(parent){
    if(!QFile::exists("settingFavoris.ini")){
        setting=new QSettings("settingFavoris.ini", QSettings::IniFormat, parent);
        initialisation();}
    else setting=new QSettings("settingFavoris.ini", QSettings::IniFormat, parent);

    QStringList favorisAdresse=setting->value("favorisAdresse").toStringList();
    QStringList favorisName=setting->value("favorisName").toStringList();
    QFile file("listIcon.ico", pr);
    QDataStream in(&file);
    file.open(QIODevice::ReadOnly);

    QScrollArea *area=new QScrollArea;
    addTab(area, "Favoris");
    favoris=new QWidget;
    layoutFavoris=new QVBoxLayout;
    layoutFavoris->setSpacing(0);
    favoris->setLayout(layoutFavoris);
    area->setWidget(favoris);
    area->setBackgroundRole(QPalette::Light);
    favoris->setFixedWidth(225);

    for(int i=0; i<favorisName.count(); i++){
        QIcon icon;
        in>>icon;
        addFavoris(QUrl(favorisAdresse[i]), favorisName[i], icon);
        connect(listLien[i], SIGNAL(suppr()), this, SLOT(supprFavoris()));
        connect(listLien[i], SIGNAL(openPage(QUrl)), this, SLOT(openPageS(QUrl)));
        connect(listLien[i], SIGNAL(openNewPage(QUrl)), this, SLOT(openNewPageS(QUrl)));
    }

    setVisible(false);
}

Favoris::~Favoris(){
    QStringList favorisAdresse, favorisName;
    for(int i=0; i<listLien.count(); i++){
        favorisAdresse.append(listLien[i]->getUrl().toString());
        favorisName.append(listLien[i]->getName());
    }
    setting->setValue("favorisAdresse", favorisAdresse);
    setting->setValue("favorisName", favorisName);
    QFile file("listIcon.ico", pr);
    QDataStream out(&file);
    file.open(QIODevice::WriteOnly);
    for(int i=0; i<listLien.count(); i++) out<<listLien[i]->getIcon();
}

void Favoris::initialisation(){
    setting->setValue("FavorisName", QStringList("Google"));
    setting->setValue("FavorisAdresse", QStringList("http://www.google.fr"));
}

void Favoris::addFavoris(QUrl url, QString name, QIcon icone){
    listLien.push_back(new LienFavoris(url, name, icone));
    layoutFavoris->addWidget(listLien[listLien.count()-1], Qt::AlignLeft);
    favoris->adjustSize();
}

void Favoris::supprFavoris(){
    LienFavoris *lien=qobject_cast<LienFavoris*>(sender());
    listLien.remove(listLien.indexOf(lien));
    delete lien;
}

void Favoris::openPageS(QUrl url){
    emit openPage(url);
}

void Favoris::openNewPageS(QUrl url){
    emit openNewPage(url);
}

void Favoris::leaveEvent(QEvent *event){
    setVisible(false);
    QTabWidget::leaveEvent(event);
}
