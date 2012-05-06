#include <QtGui>
#include <QtWebkit>
#include <QtNetwork>
#include <iostream>
#include <vector>


//RC_FILE += ressource.rc
class Label : public QLabel{
    Q_OBJECT
public:
    Label(QWidget *parent=0);
    virtual void mousePressEvent(QMouseEvent*);

signals:
    void clicked();
};

class LienFavoris : public QWidget{
    Q_OBJECT
public:
    LienFavoris(QUrl, QString nameA=QString(), QIcon iconA=QIcon(), QWidget *parent=0);
    QString getName();
    QUrl getUrl();
    QIcon getIcon();

signals:
    void openPage(QUrl);
    void openNewPage(QUrl);
    void suppr();

protected:
    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent*);
    virtual void mouseDoubleClickEvent(QMouseEvent*);

private slots:
    void loadFinish(bool);
    void saveName();
    void rename();
    void ouvrir();
    void ouvrirNO();
    void supprimer();
    void openMenu(QPoint);

private:
    void initCache();
    void initLayout();
    void initMenu();
    Label *icone, *lien;
    QLineEdit *lienEdit;
    QPushButton *button;
    QMenu *menu;
    QUrl url;
    QString name;

};


class CookieJar : public QNetworkCookieJar{
public:
    CookieJar(QWidget *parent=0);
    ~CookieJar();
};


class ToolDev: public QWidget{
public:
    ToolDev();
    void actuPage(QString);

private:
    QTextEdit *text;
};


class Favoris: public QTabWidget{
    Q_OBJECT

public:
    Favoris(QWidget*);
    ~Favoris();
    void setFavoris(QList<QUrl>);
    void setFavoris(QList<QUrl>, QStringList);
    void initialisation();
    virtual void leaveEvent(QEvent*);

public slots:
    void addFavoris(QUrl, QString name=QString(), QIcon icone=QIcon());

signals:
    void openPage(QUrl);
    void openNewPage(QUrl);

private slots:
    void openPageS(QUrl);
    void openNewPageS(QUrl);
    void supprFavoris();

private:
    QWidget *favoris, *historique, *pr;
    QVBoxLayout *layoutFavoris;
    QVector<LienFavoris*> listLien;
    QSettings *setting;
};


class Setting: public QDialog{
    Q_OBJECT

public:
    Setting(QWidget*);
    QString getPageHome();
    int getGroupPD();
    int getGroupPO();
    void ouv();
    QStringList getListPage();
    void setListPage(QStringList);
    void initialisation();

public slots:
    void save();

private slots:
    void quitter();

private:
    QRadioButton *pageV, *pageA, *pageD, *pageVO, *pageDO;
    QLineEdit *m_pageHome;
    QPushButton *m_ok, *m_annuler;
    QSettings *setting;

};


class MaFenetre: public QMainWindow{
    Q_OBJECT

public:
    MaFenetre();
    ~MaFenetre();
    QWebView* pageActuelle();

public slots:
    void precedent();
    void suivant();
    void stop();
    void actualiser();
    void home();
    void go();
    void goToUrl(QUrl);
    void suppr(int);
    void aPropos();
    void nouveau();
    void affFavoris();
    void addFavoris();
    void download(QNetworkReply*);
    void nouvelPage(QUrl);

private slots:
    void loadStart();
    void load(int);
    void loadFinish();
    void titre(QString);
    void url(QUrl);
    void changementPage();
    void messageStatus(QString);
    void affOption();
    void test(QNetworkRequest*);

private:
    QTabWidget *m_onglet;
    QWidget *widgetTemp;
    QVBoxLayout *layoutTemp;
    Setting *option;
    Favoris *favoris;
    QMenu *m_menuFichier, *m_menuNavigation, *m_menuAide;
    QAction *m_nouvelOnglet, *m_option, *m_quitter;
    QAction *m_precedent, *m_suivant, *m_stop, *m_actualiser, *m_home, *m_favoris, *m_addFavoris;
    QAction *m_go, *m_apropos, *m_aproposQt;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QLineEdit *m_adresse;
    QProgressBar *m_loading;
    QStringList *listFavorisAdresse;
    CookieJar *cookies;
    ToolDev dev;
};
