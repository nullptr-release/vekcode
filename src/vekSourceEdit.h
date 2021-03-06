#ifndef VEKSOURCEEDIT_H
#define VEKSOURCEEDIT_H

#include "obj/common.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QTableWidgetItem>
#include "obj/objectJson.h"
#include "obj/objectSource.h"
typedef enum{
    WINESRC,
    GAMESRC
}srcType;
namespace Ui {
class vekSourceEdit;
}
class vekSourceEdit : public QDialog
{
    Q_OBJECT

public:
    static vekSourceEdit& GetInstance(){
        static vekSourceEdit instance;
        return instance;
    }
private:
    vekSourceEdit();
    ~vekSourceEdit();
    vekSourceEdit(const vekSourceEdit& instance);
    const vekSourceEdit& operator =(const vekSourceEdit& instance);
    Ui::vekSourceEdit *ui;
    QStandardItemModel *tableModel=nullptr;
    void loadSrcDataTableView(QTableView* qTableView);
    void setTableView(QTableView*);
    QMenu *pMenu=nullptr;
    void loadTableView();
    void objectAddSrc(QTableView*);
    void objectDeleteSrc(QTableView*);
    void objectUpdateSrc(QTableView*);
    void loadData();
    void saveAllData();
private:

private slots:
    void clicked_rightMenu(const QPoint &pos);  //右键信号槽函数
    void onTaskBoxContextMenuEvent();
    void slotsDone();
signals:
    void _unSourveEdit();
};

#endif // VEKSOURCEEDIT_H
