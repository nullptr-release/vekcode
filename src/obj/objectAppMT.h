#ifndef OBJECTAPPMT_H
#define OBJECTAPPMT_H

#include "objectJson.h"
#include "objectExtend.h"
#include "pObject.h"
class objectAppMT :public QObject
{
    Q_OBJECT
public:
    objectAppMT(BaseAppData*,BaseDockData*);
    ~objectAppMT();
    QString GetRandomCID();
    bool InitDockObj(bool);
    std::vector<QStringList> argsList;
    void DockLibsInstall();
    void ExecuteObj(objectType,objectWineBoot,objectWineServer);
    void newDock();
    void optionRegs();
    void sObjectInstall();
private:
    BaseAppData* appData=new BaseAppData;
    BaseDockData* dockData=new BaseDockData;
    void InitDockDir(bool foceState,QDir _dockPath,QDir _dockDir);
    QStringList GetDxvkFileList(QString basedxvkDir);
    void DxvkFileInstall();
    void DxvkRegedit(QStringList dxvkFileList);
    void DxvkHUDRegs();
    void DxvkConfigFile();
    void DefaultFontsFileInstall();
    void DefaultFontsRegs();
    void WaitObjectDone(objectExtend*);  
    void DockEditSystemVersion();
    void addJsonAuto(QString);
    void installMonoPlugs();
    void installGeckoPlugs();
};

#endif // VEKGAMEADDOBJECT_H