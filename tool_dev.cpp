#include "prototype.h"

ToolDev::ToolDev() : QWidget(){
    text=new  QTextEdit(this);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(text);
    setLayout(layout);
}

void ToolDev::actuPage(QString str){
    text->setText(str);
}

