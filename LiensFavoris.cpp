#include "prototype.h"

Label::Label(QWidget *parent) : QLabel(parent){

}

void Label::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton) emit clicked();
    QLabel::mousePressEvent(event);
}

LienFavoris::LienFavoris(QUrl urlA, QString nameA, QIcon iconA, QWidget *parent) : QWidget(parent){
    initCache();
    url=urlA;
    name=nameA;

    lien=new Label();
    icone=new Label();
    lienEdit=new QLineEdit();
    button=new QPushButton(QIcon("fleche_droite.png"), "");
    button->setFixedSize(25, 15);
    lienEdit->setFixedHeight(15);
    setFixedHeight(15);
    if(iconA.isNull()){
        if(name.isNull()){
            QWebView *page=new QWebView(parent);
            page->load(url);
            connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinish(bool)));
        }
        else{
            icone->setPixmap(QWebSettings::iconForUrl(url).pixmap(QSize(lien->height(), lien->height())));
            lien->setText(name);
        }
    }
    else icone->setPixmap(iconA.pixmap(QSize(15, 15)));

    lien->setOpenExternalLinks(true);
    setToolTip(url.toString());
    initLayout();
    initMenu();
    adjustSize();

    connect(lienEdit, SIGNAL(returnPressed()), this, SLOT(saveName()));
    connect(lien, SIGNAL(clicked()), this, SLOT(ouvrir()));
    connect(icone, SIGNAL(clicked()), this, SLOT(ouvrir()));
    connect(button, SIGNAL(clicked()), this, SLOT(ouvrirNO()));

}

void LienFavoris::initCache(){
    if (!QDir(qApp->applicationDirPath() + "/cache/").exists()) QDir(qApp->applicationDirPath()).mkdir("cache"); //est nécessaire pour récupérer des icons
    // Si le dossier de cache n'existe pas, je le crée
    // Je crée un emplacement pour enregistrer les données de cache
    QWebSettings::setIconDatabasePath(qApp->applicationDirPath() + "/cache/");
}

void LienFavoris::initLayout(){
    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(icone);
    layout->addWidget(lien);
    layout->addWidget(lienEdit);
    layout->addStretch(50);
    layout->addWidget(button);
    lienEdit->hide();
    button->hide();
    layout->setSpacing(5);
    layout->setContentsMargins(0, 0, 0, 0); //bordure
    setLayout(layout);
}

void LienFavoris::initMenu(){
    menu=new QMenu(this);
    QAction *ouvrir=menu->addAction("Ouvrir");
    QAction *ouvrirNO=menu->addAction("Ouvrir dans un nouvel onglet");
    QAction *rename=menu->addAction("Renommer");
    QAction *suppr=menu->addAction("Supprimer");

    connect(ouvrir, SIGNAL(triggered()), this, SLOT(ouvrir()));
    connect(ouvrirNO, SIGNAL(triggered()), this, SLOT(ouvrirNO()));
    connect(rename, SIGNAL(triggered()), this, SLOT(rename()));
    connect(suppr, SIGNAL(triggered()), this, SLOT(supprimer()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(openMenu(QPoint)));
}

void LienFavoris::loadFinish(bool lol){
    QWebView *page=qobject_cast<QWebView*>(sender());
    if(name.isNull()) name=page->title();
    lien->setText(name);
    icone->setPixmap(page->icon().pixmap(QSize(15, 15)));
    page->deleteLater();
}

void LienFavoris::enterEvent(QEvent *event){
    button->show();
    QWidget::enterEvent(event);
}

void LienFavoris::leaveEvent(QEvent *event){
    button->hide();
    lienEdit->hide();
    lien->show();
    QWidget::leaveEvent(event);
}

void LienFavoris::mouseDoubleClickEvent(QMouseEvent *event){
    rename();
    QWidget::mouseDoubleClickEvent(event);
}

void LienFavoris::saveName(){
    name=lienEdit->text();
    lien->setText(name);
    lienEdit->hide();
    lien->show();
    button->show();
}

void LienFavoris::rename(){
    button->hide();
    lien->hide();
    lienEdit->show();
    lienEdit->setText(name);
    lienEdit->selectAll();
    lienEdit->setFocus();
}

void LienFavoris::ouvrir(){
    emit openPage(url);
}

void LienFavoris::ouvrirNO(){
    emit openNewPage(url);
}

void LienFavoris::supprimer(){
    emit suppr();
}

void LienFavoris::openMenu(QPoint point){
    menu->popup(mapToGlobal(point));
}

QString LienFavoris::getName(){
    return name;
}

QUrl LienFavoris::getUrl(){
    return url;
}

QIcon LienFavoris::getIcon(){
    return QIcon(*icone->pixmap());
}
