#include "Prototype.h"

Setting::Setting(QWidget *parent=0) : QDialog(parent){
    m_ok=new QPushButton("Ok", this);
    m_annuler=new QPushButton("Annuler", this);

    m_pageHome=new QLineEdit(this);
    QLabel *pageHome=new QLabel("Page Home:", this);

    QGroupBox *groupPD=new QGroupBox("Page de démarage", this);
    pageV=new QRadioButton("Page vierge", groupPD);
    pageA=new QRadioButton("Derniére page", groupPD);
    pageD=new QRadioButton("Page Home", groupPD);
    QVBoxLayout *layoutVPD=new QVBoxLayout;
    layoutVPD->addWidget(pageV);
    layoutVPD->addWidget(pageA);
    layoutVPD->addWidget(pageD);
    groupPD->setLayout(layoutVPD);

    QGroupBox *groupPO=new QGroupBox("NouvelOnglet", this);
    pageVO=new QRadioButton("Page Vierge", groupPO);
    pageDO=new QRadioButton("Page Home", groupPO);
    QVBoxLayout *layoutVPO=new QVBoxLayout;
    layoutVPO->addWidget(pageVO);
    layoutVPO->addWidget(pageDO);
    groupPO->setLayout(layoutVPO);

    QGridLayout *layout=new QGridLayout(this);
    layout->addWidget(m_pageHome, 0, 1);
    layout->addWidget(pageHome, 0, 0);
    layout->addWidget(groupPD, 1, 0, 1, 2);
    layout->addWidget(groupPO, 2, 0, 1, 2);
    layout->addWidget(m_ok, 3, 0);
    layout->addWidget(m_annuler, 3, 1);
    setLayout(layout);

    if(!QFile::exists("setting.ini")){
        setting=new QSettings("setting.ini", QSettings::IniFormat, parent);
        initialisation();}
    else setting=new QSettings("setting.ini", QSettings::IniFormat, parent);

    setVisible(false);
    setWindowTitle("Options...");

    connect(m_ok, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_annuler, SIGNAL(clicked()), this, SLOT(quitter()));
}

void Setting::save(){
    if(pageV->isChecked()){
        setting->setValue("HomePage", "");
        setting->setValue("groupPD", 0);}
    else {
        if(pageD->isChecked()){
            setting->setValue("HomePage", m_pageHome->displayText());
            setting->setValue("groupPD", 1);}
        else setting->setValue("groupPD", 2);
    }
    if(pageVO->isChecked()) setting->setValue("groupPO", 0);
    else setting->setValue("groupPO", 1);
    setVisible(false);
}

void Setting::quitter(){
    setVisible(false);
}

void Setting::ouv(){
    setVisible(!isVisible());
    m_pageHome->setText(setting->value("HomePage").toString());
    switch(setting->value("groupPD").toInt()){
        case 0: pageV->setChecked(true); break;
        case 1: pageD->setChecked(true); break;
        case 2: pageA->setChecked(true); break;
        default:;}
    if(setting->value("groupPO").toInt()) pageDO->setChecked(true);
    else pageVO->setChecked(true);
}

QString Setting::getPageHome(){
    return setting->value("HomePage").toString();
}

void Setting::initialisation(){
    setting->setValue("groupPD", 1);
    setting->setValue("groupPO", 1);
    setting->setValue("HomePage", "http://www.google.fr");
    setting->setValue("savePages", "http://www.google.fr");
}

int Setting::getGroupPD(){
    return setting->value("groupPD").toInt();
}

int Setting::getGroupPO(){
    return setting->value("groupPO").toInt();
}

QStringList Setting::getListPage(){
    return setting->value("savePages").toStringList();
}

void Setting::setListPage(QStringList list){
    setting->setValue("savePages", list);
}
