#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <QWidget>
#include <QMap>

class OptionsSubMenu;
class QGroupBox;
class QTreeWidget;
class QTreeWidgetItem;
class OptionsMenu: public QWidget {
    Q_OBJECT
    public:
        OptionsMenu(QWidget * parent = 0);
        void addMenu(const QString& path, OptionsSubMenu* widget);
    private:
        QMap<QString, OptionsSubMenu*> m_widgets;
        QString m_current_path;
        OptionsSubMenu* currentMenu();
        QGroupBox* m_mainbox;
        QTreeWidget * m_menulist;
        

    public slots:
        void changeMenu(QTreeWidgetItem* item, int column);
        void setMenu(const QString& path);
        void makeOptionsList();
        void apply();
        void ok();
        void cancel();


};


class OptionsSubMenu: public QWidget {
    Q_OBJECT
    public:
    OptionsSubMenu(QWidget * parent = 0);
    public slots:
        virtual void save() = 0;
        virtual void reset() = 0;

    
};
#endif
