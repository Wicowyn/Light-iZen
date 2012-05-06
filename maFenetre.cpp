#include "prototype.h"
#include "NetworkAccessManager.h"

MaFenetre::MaFenetre(){
    if (!QDir(qApp->applicationDirPath() + "/cache/").exists()) QDir(qApp->applicationDirPath()).mkdir("cache"); //est nécessaire pour récupérer des icons
    // Si le dossier de cache n'existe pas, je le crée
    // Je crée un emplacement pour enregistrer les données de cache
    QWebSettings::setIconDatabasePath(qApp->applicationDirPath() + "/cache/");

    widgetTemp=new QWidget();
    layoutTemp=new QVBoxLayout();
    widgetTemp->setLayout(layoutTemp);

    dev.setVisible(true);
    option=new Setting(this);
    m_onglet=new QTabWidget(this);
    m_onglet->setTabsClosable(true);
    m_onglet->setMovable(true);

    m_menuFichier=menuBar()->addMenu("&Fichier");
    m_nouvelOnglet=new QAction("Nouvel onglet...", this);
    QList<QKeySequence> list;
    list<<QKeySequence(Qt::Key_Up, Qt::Key_Up, Qt::Key_Down, Qt::Key_Down);
    list<<QKeySequence(Qt::Key_Left, Qt::Key_Right, Qt::Key_Left, Qt::Key_Right);
    list<<QKeySequence("a+b");
    m_nouvelOnglet->setShortcuts(list);
    m_option=new QAction("Otions", this);
    m_quitter=new QAction("Quitter", this);
    m_menuFichier->addAction(m_nouvelOnglet);
    m_menuFichier->addAction(m_option);
    m_menuFichier->addAction(m_quitter);
    m_nouvelOnglet->setIcon(QIcon("nouveau.png"));

    m_menuNavigation=menuBar()->addMenu("&Navigation");
    m_precedent=new QAction("Page précédente", this);
    m_suivant=new QAction("Page suivante", this);
    m_stop=new QAction("Stop", this);
    m_actualiser=new QAction("Actualiser", this);
    m_home=new QAction("Page de démarage", this);
    m_favoris=new QAction("Favoris", this);
    m_addFavoris=new QAction("Add", this);
    m_menuNavigation->addAction(m_precedent);
    m_menuNavigation->addAction(m_suivant);
    m_menuNavigation->addAction(m_stop);
    m_menuNavigation->addAction(m_actualiser);
    m_menuNavigation->addAction(m_home);
    m_menuNavigation->addAction(m_favoris);
    m_menuNavigation->addAction(m_addFavoris);
    m_precedent->setIcon(QIcon("precedent.png"));
    m_suivant->setIcon(QIcon("suivant.png"));
    m_stop->setIcon(QIcon("stop.png"));
    m_actualiser->setIcon(QIcon("actualiser.png"));
    m_home->setIcon(QIcon("home.png"));
    m_favoris->setIcon(QIcon("favoris.png"));
    m_addFavoris->setIcon(QIcon("addFavoris.png"));

    m_menuAide=menuBar()->addMenu("&?");
    m_apropos=new QAction("&Apropos...", this);
    m_aproposQt=new QAction("Apropos de &Qt...", this);
    m_menuAide->addAction(m_apropos);
    m_menuAide->addAction(m_aproposQt);

    m_toolBar=addToolBar("Navigation");
    m_adresse=new QLineEdit;
    m_go=new QAction("Go", this);
    m_go->setIcon(QIcon("go.png"));
    m_toolBar->addAction(m_precedent);
    m_toolBar->addAction(m_suivant);
    m_toolBar->addAction(m_stop);
    m_toolBar->addAction(m_nouvelOnglet);
    m_toolBar->addAction(m_home);
    m_toolBar->addWidget(m_adresse);
    m_toolBar->addAction(m_go);
    m_toolBar->addAction(m_actualiser);
    m_toolBar->addAction(m_favoris);
    m_toolBar->addAction(m_addFavoris);

    m_statusBar=statusBar();
    m_loading=new QProgressBar;
    m_statusBar->addWidget(m_loading);
    m_loading->setMaximumHeight(15);

    favoris=new Favoris(this);
    QGridLayout *layout=new QGridLayout;
    layout->setRowMinimumHeight(0, 20);
    layout->setColumnStretch(0, 50);
    layout->addWidget(favoris, 1, 1);
    m_onglet->setLayout(layout);

    setCentralWidget(m_onglet);
    setWindowTitle("Light iZen");
    setWindowIcon(QIcon("vague.png"));
    cookies=new CookieJar(this);

    if(option->getGroupPD()==2){
        QStringList list=option->getListPage();
        for(int i=0; i<list.count(); i++){
            nouvelPage(QUrl(list.value(i)));
        }
    }
    else nouvelPage(option->getPageHome());

    connect(m_go, SIGNAL(triggered()), this, SLOT(go()));
    connect(m_home, SIGNAL(triggered()), this, SLOT(home()));
    connect(m_adresse, SIGNAL(returnPressed()), this, SLOT(go()));
    connect(m_actualiser, SIGNAL(triggered()), this, SLOT(actualiser()));
    connect(m_stop, SIGNAL(triggered()), this, SLOT(stop()));
    connect(m_onglet, SIGNAL(currentChanged(int)), this, SLOT(changementPage()));
    connect(m_nouvelOnglet, SIGNAL(triggered()), this, SLOT(nouveau()));
    connect(m_onglet, SIGNAL(tabCloseRequested(int)), this, SLOT(suppr(int)));
    connect(m_precedent, SIGNAL(triggered()), this, SLOT(precedent()));
    connect(m_suivant, SIGNAL(triggered()), this, SLOT(suivant()));
    connect(m_apropos, SIGNAL(triggered()), this, SLOT(aPropos()));
    connect(m_aproposQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(m_quitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_option, SIGNAL(triggered()), this, SLOT(affOption()));
    connect(m_favoris, SIGNAL(triggered()), this, SLOT(affFavoris()));
    connect(favoris, SIGNAL(openPage(QUrl)), this, SLOT(goToUrl(QUrl)));
    connect(favoris, SIGNAL(openNewPage(QUrl)), this, SLOT(nouvelPage(QUrl)));
    connect(m_addFavoris, SIGNAL(triggered()), this, SLOT(addFavoris()));
}

QWebView* MaFenetre::pageActuelle(){
    return m_onglet->currentWidget()->findChild<QWebView *>();
}

void MaFenetre::test(QNetworkRequest* lol){
    QMessageBox::information(this, "kikoolol", lol->url().toString());
}

void MaFenetre::nouvelPage(QUrl adresse=QUrl()){
    QWidget *kikoo=new QWidget;
    QWebView *lol=new QWebView(kikoo);
    QVBoxLayout *layout=new QVBoxLayout(kikoo);
    //lol->page()->setNetworkAccessManager(new NetworkAccessManager(lol->page()));
    layout->addWidget(lol);
    kikoo->setLayout(layout);
    lol->page()->setForwardUnsupportedContent(true);
    //lol->page()->networkAccessManager()->setCookieJar(cookies);

    if(adresse.isEmpty()) adresse=QUrl(option->getPageHome());
    lol->page()->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    lol->page()->settings()->setAttribute(QWebSettings::JavaEnabled, true);
    lol->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    lol->load(adresse);
    m_onglet->addTab(kikoo, "Welcome");

    //connect(lol->page(), SIGNAL(downloadRequested(QNetworkRequest)), this, SLOT(test(QNetworkRequest)));
    connect(lol, SIGNAL(titleChanged(QString)), this, SLOT(titre(QString)));
    connect(lol, SIGNAL(urlChanged(QUrl)), this, SLOT(url(QUrl)));
    connect(lol, SIGNAL(loadProgress(int)), this, SLOT(load(int)));
    connect(lol, SIGNAL(loadStarted()), this, SLOT(loadStart()));
    connect(lol, SIGNAL(loadFinished(bool)), this, SLOT(loadFinish()));
    connect(lol->page(), SIGNAL(unsupportedContent(QNetworkReply*)), this, SLOT(download(QNetworkReply*)));
}

void MaFenetre::nouveau(){
    if(option->getGroupPO()) nouvelPage(QUrl(option->getPageHome()));
    else nouvelPage(QUrl("about:blank"));
}

MaFenetre::~MaFenetre(){
    if(option->getGroupPD()==2){
    QStringList list;
    for(int i=0; i<m_onglet->count(); i++) list.append(m_onglet->widget(i)->findChild<QWebView *>()->url().toString());
    option->setListPage(list);}
}

void MaFenetre::suppr(int index){
    QWebView *web=m_onglet->widget(index)->findChild<QWebView *>();
    QWidget *widget=m_onglet->widget(index);
    QNetworkCookieJar *temp=new QNetworkCookieJar(widget);
    web->page()->networkAccessManager()->setCookieJar(temp);
    //delete web;

    m_onglet->removeTab(index);
    delete widget;
    //layoutTemp->addWidget(web);
    //widget->setToolTip("hcklhck");

}

void MaFenetre::precedent(){
    pageActuelle()->back();
}

void MaFenetre::suivant(){
    pageActuelle()->forward();
}

void MaFenetre::home(){
    if(option->getGroupPD()==0) pageActuelle()->load(QUrl("about:blank"));
    else{
        QString adresse=option->getPageHome();
        if(!adresse.startsWith("http://www.")){
        if(adresse.startsWith("www.")) adresse="http://"+adresse;
        else adresse="http://www."+adresse;
        }
        pageActuelle()->load(QUrl(adresse));
    }
}


void MaFenetre::titre(QString title){
    int i=0;
    while(m_onglet->widget(i)->findChild<QWebView *>()!=qobject_cast<QWebView *>(sender())) i++;
        //permet de retrouver de quel onglet le signal a étais émit
    if(title.size()>15) {
        title.truncate(15);
        title+="...";}
    m_onglet->setTabText(i, title);
}

void MaFenetre::url(QUrl url){
    QString adresse=url.toString();
    m_adresse->setText(adresse);
}

void MaFenetre::go(){
        QString adresse=m_adresse->text();
        if(!adresse.startsWith("http://www.")){
           if(adresse.startsWith("www.")) adresse="http://"+adresse;
          else adresse="http://www."+adresse;
        }
        pageActuelle()->load(QUrl(adresse));
}

void MaFenetre::goToUrl(QUrl url){
    pageActuelle()->load(url);
}

void MaFenetre::actualiser(){
    pageActuelle()->reload();
}

void MaFenetre::stop(){
    pageActuelle()->stop();
}

void MaFenetre::changementPage(){
    url(pageActuelle()->url());
}

void MaFenetre::messageStatus(QString text){
    m_statusBar->showMessage(text, 2000);
}

void MaFenetre::loadStart(){
    m_loading->setVisible(true);
}

void MaFenetre::load(int val){
    m_loading->setValue(val);
}

void MaFenetre::loadFinish(){
    m_loading->setVisible(false);
    m_statusBar->showMessage("Prêt", 2000);
    dev.actuPage(pageActuelle()->page()->mainFrame()->toHtml());
}

void MaFenetre::aPropos(){
    QMessageBox::information(this, "A propos ...", "Navig'Yapi est un projet réalisé pour parfaire mes connaissances en C++/Qt, je remercie le <a href=\"http://www.siteduzero.com\">Site du Zéro</a> pour l'aide qu'il m'as apporter.<br />Pour d'éventuel question -> yapiti@hotmail.fr<br />11/03/2011");
}

void MaFenetre::affOption(){
    option->ouv();
}

void MaFenetre::affFavoris(){
    favoris->setVisible(!favoris->isVisible());
}

void MaFenetre::addFavoris(){
    favoris->addFavoris(pageActuelle()->url(), pageActuelle()->title(), pageActuelle()->icon());
}

void MaFenetre::download(QNetworkReply *rep){
    QMessageBox::information(this, "kikoo", "kikoo");
    /*if(!rep->byteAvailable()) return;
    QNetworkRequest requete(rep->url());
    QNetworkAcessManager *manager=new QNetworkAcessManager;
    QNetworkReply *repBis=new QNetworkReply(manager->get(requete));*/
}

