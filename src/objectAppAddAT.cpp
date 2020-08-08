#include "objectAppAddAT.h"

objectAppAddAT::objectAppAddAT(ObjectAddDataAT objAddDataAT)
{
   _objAddDataAT=objAddDataAT;
}

objectAppAddAT::~objectAppAddAT(){
    delete _baseAutoSetJson;
    _baseAutoSetJson=nullptr;
}
QString objectAppAddAT::jsonPathTypeToStr(QString jsonPathFile){
    try {
        QString jsonDataStr=nullptr;
        if(jsonPathFile.contains("http",Qt::CaseSensitive)||jsonPathFile.contains("https",Qt::CaseSensitive)){
            vekGetCurl* _vekgetcurl=new vekGetCurl();
            jsonDataStr=QString::fromStdString(_vekgetcurl->vekGetData(jsonPathFile.toStdString()));
            delete _vekgetcurl;
            _vekgetcurl=nullptr;
        }else{
            QFile file(jsonPathFile);
            if(!file.exists())
            {
                return nullptr;
            }
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            jsonDataStr = file.readAll();
            file.close();
        }
        if(jsonDataStr=="error"||jsonDataStr==nullptr){
            return nullptr;
        }
        return jsonDataStr;
    } catch (nullopt_t) {
        return nullptr;
    }
}
bool objectAppAddAT::jsonUnserialize(QString jsonPathFile){
    QString jsonData=jsonPathTypeToStr(jsonPathFile);
        if(jsonData==nullptr){
            return false;
        }
        objectJson _objectJson;
        if(_objectJson.unDataSerializeScriptData(_baseAutoSetJson,jsonData)==nullptr){
            return false;
        }
    return true;
}
void objectAppAddAT::dataToBase(){
    if(!_baseAutoSetJson->Option.empty()){
        for(auto a:_baseAutoSetJson->Option){
            if(a.first=="appName"){
                baseAppData->appName=a.second;
            }
            if(a.first=="defaultFont"){
                QVariant defaultFontValue=a.second;
                baseAppData->defaultFonts=(defaultFontValue).toBool();
            }
            if(a.first=="sharedMemory"){
                QVariant sharedMemoryValue=a.second;
                baseAppData->taskMemorySharing=(sharedMemoryValue).toBool();
            }
            if(a.first=="writeCopy"){
                QVariant writeCopyValue=a.second;
                baseAppData->taskMemoryOptimization=(writeCopyValue).toBool();
            }
            if(a.first=="rtServer"){
                QVariant rtServerValue=a.second;
                baseAppData->taskRealTimePriority=(rtServerValue).toBool();
            }
            if(a.first=="dockSystemVersion"){
                baseAppData->dockSystemVersion=a.second;
            }
            if(a.first=="monoState"){
                QVariant monoState=a.second;
                baseAppData->monoState=(monoState).toBool();;
            }
            if(a.first=="geckoState"){
                QVariant geckoState=a.second;
                baseAppData->geckoState=(geckoState).toBool();;
            }
            if(a.first=="mainPrcoName"){
                baseAppData->mainPrcoName=a.second;
            }
            baseAppData->wineVersion=_objAddDataAT.pWineVersion;
        }
    }
    if(!_baseAutoSetJson->Dxvk.empty()){
        for(auto a:_baseAutoSetJson->Dxvk){
            if(a.first=="dxvkVersion"){
                baseAppData->dxvkVerson=a.second;
            }
            if(a.first=="dxvkState"){
                QVariant dxvkStateValue=a.second;
                baseAppData->dxvkState=(dxvkStateValue).toBool();
            }
            if(a.first=="dxvkHUD"){
                QVariant dxvkHUDValue=a.second;
                baseAppData->dxvkHUD=(dxvkHUDValue).toBool();
            }
        }
    }
    if(!_baseAutoSetJson->Regs.empty()){
        baseAppData->dockRegs=_baseAutoSetJson->Regs;
    }
    if(!_baseAutoSetJson->Libs.empty()){
        baseAppData->dockLibs=_baseAutoSetJson->Libs;
    }
    if(!_baseAutoSetJson->Env.empty()){
        baseAppData->dockEnv=_baseAutoSetJson->Env;
    }
    if(_baseAutoSetJson->Args!=nullptr){
        baseAppData->appOtherAgrs=_baseAutoSetJson->Args;
    }
    if(!_baseAutoSetJson->AttachProc.empty()){
        baseAppData->attachProc=_baseAutoSetJson->AttachProc;
    }
    baseAppData->dockPath=_objAddDataAT.pDckPath;
    baseAppData->dockName=_objAddDataAT.pDockName;
    baseAppData->appExe=_objAddDataAT.pAppExePath;
    QFileInfo fi = QFileInfo(_objAddDataAT.pAppExePath);
    baseAppData->workPath=fi.path();
    objectJson* _objectJson=new objectJson();
    baseAppData->appCID=_objectJson->GetRandomCID();
    delete _objectJson;
    _objectJson=nullptr;
    baseAppData->appIco=":/res/img/vek.ico";
    for(auto &x : g_vekLocalData.wineVec){
        if(x.first==_objAddDataAT.pWineVersion){
            baseAppData->wineVersion=x.second.WineInstallName;
            baseAppData->winePath=x.second.wineInstallPath;
            break;
        }
    }
}
void objectAppAddAT::objectAutoObj(){
    if(objDiyAddApp!=nullptr){
        delete objDiyAddApp;
        objDiyAddApp=nullptr;
    }
    emit Tips("配置容器中请稍候!");
    objDiyAddApp=new objectAppAddMT(baseAppData,nullptr);
    objDiyAddApp->InitDockObj(false);
    objDiyAddApp->optionRegs();
    objDiyAddApp->DockLibsInstall();
    objDiyAddApp->SaveDataToJson(baseAppData->dockName,*baseAppData);
    delete objDiyAddApp;
    objDiyAddApp=nullptr;
}
void objectAppAddAT::run(){
    _baseAutoSetJson=new BaseAutoSetJson();
    baseAppData=_objAddDataAT.pBaseAppData;
    if(!jsonUnserialize(_objAddDataAT.pJsonPath)){
        emit Error("配置容器出错!",true);
        return;
    }else{
        dataToBase();
        objectAutoObj();
        emit Done();
    }
}