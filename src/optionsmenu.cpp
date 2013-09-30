#include "optionsmenu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGroupBox>
#include <QTreeWidget>


OptionsMenu::OptionsMenu(QWidget * parent): QWidget(parent), m_mainbox(new QGroupBox(this)), m_menulist(new QTreeWidget(this)) {



    QWidget * mainwidget = new QWidget(this);
    QHBoxLayout * hlayout = new QHBoxLayout(mainwidget);
    mainwidget->setLayout(hlayout);

    hlayout->addWidget(m_menulist);
    m_menulist->setHeaderLabels(QStringList("Menu Item"));
    hlayout->addWidget(m_mainbox);
    m_mainbox->setLayout(new QVBoxLayout(this));

    QVBoxLayout * toplayout = new QVBoxLayout(this);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, this);
    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::released,
            this, &OptionsMenu::apply);
    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::released,
            this, &OptionsMenu::ok);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::released,
            this, &OptionsMenu::cancel);
    connect(m_menulist, &QTreeWidget::itemDoubleClicked, this, &OptionsMenu::changeMenu);
    toplayout->addWidget(mainwidget);
    toplayout->addWidget(buttonBox);
}

#include <QDebug>
void OptionsMenu::addMenu(const QString& path, OptionsSubMenu* widget) {
    if(m_widgets.constFind(path) == m_widgets.constEnd()) {
        qDebug() << m_widgets.keys();
        m_widgets.insert(path,widget);
        makeOptionsList();
    }
}

OptionsSubMenu* OptionsMenu::currentMenu() {
    return m_widgets[m_current_path];
}
void OptionsMenu::ok() {
    apply();
    cancel();
}
void OptionsMenu::changeMenu(QTreeWidgetItem* item, int column) {
    setMenu(item->text(column));
}
void OptionsMenu::setMenu(const QString& path) {
    qDebug() << "New path: " << path;
    auto&& layout = m_mainbox->layout();
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        layout->removeItem(child);
        child->widget()->hide();
        delete child;
    }
    m_current_path = path;
    layout->addWidget(m_widgets[path]);
    m_widgets[path]->show();



}
void OptionsMenu::apply() {
    if(currentMenu())
        currentMenu()->save();
}
void OptionsMenu::cancel() {
    if(currentMenu())
        currentMenu()->reset();
    this->hide();
}

void OptionsMenu::makeOptionsList() {

    m_menulist->clear();
    for(auto&& key: m_widgets.keys()) {
        m_menulist->addTopLevelItem(new QTreeWidgetItem((QTreeWidget*)0,key.split("/")));
    }
}


OptionsSubMenu::OptionsSubMenu(QWidget * parent): QWidget(parent) {
}

