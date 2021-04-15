#ifndef VEKAPPADDAT_H
#define VEKAPPADDAT_H

#include "obj/common.h"
#include "obj/objectJson.h"
#include "obj/objectAppAT.h"
#include "view/jview/vekViewAT.h"
namespace Ui {
class vekAppAddAT;
}

class vekAppAddAT : public QWidget
{
    Q_OBJECT
public:
    explicit vekAppAddAT(QWidget *parent = nullptr);
    ~vekAppAddAT();
    void connectDockObject(SdockerData);
private:
    Ui::vekAppAddAT *ui;
    SappData* autoAppData=nullptr;
    SdockerData* autoDockData=nullptr;
    void SetObject();
    void addAutoApp();
    void controlState(bool pState);
    void defalutValue();
signals:
    void _unAutoDock();
    void autoObjDock(SdockerData*,SappData*);
private slots:
    void TipText(QString TipInfo);
    void ErrorText(QString ErrorInfo,bool cState);
    void ObjDone(SdockerData*,SappData*);

};

#endif // VEKGAMEAUTOADD_H
