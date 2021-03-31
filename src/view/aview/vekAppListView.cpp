﻿#include "vekAppListView.h"


vekAppListView::vekAppListView( QWidget *parent):QListView(parent)
{
    parent=nullptr;
    m_hitIndex = -1;
    m_pModel = new vekAppData;
    this->setModel(m_pModel);
    m_pListMap = NULL;
}

vekAppListView::~vekAppListView()
{
}
void vekAppListView::contextMenuEvent( QContextMenuEvent * event )
{
    int hitIndex = this->indexAt(event->pos()).row();
    if (hitIndex > -1)
    {      
        QMenu *pSubMenu = NULL;
        pMenu = new QMenu(this);
        std::map<QString,vekAppListView*>::iterator it;
        for (it=m_pListMap->begin();it != m_pListMap->end();++it)
        {
            if (!pSubMenu)
            {
                //增加容器转移功能
                pSubMenu = new QMenu(tr("转移软件到容器=>") ,pMenu);
                pMenu->addMenu(pSubMenu);
            }
            if (it->second != this)
            {
                QAction *pMoveAct = new QAction( it->first,pMenu);
                //记录菜单与分组的映射，在moveSlot()响应时需要用到。
                m_ActionMap.insert(std::pair<QAction*,vekAppListView*>(pMoveAct,it->second));
                pSubMenu->addAction(pMoveAct);
                connect(pMoveAct,SIGNAL(triggered ()),this,SLOT(moveSlot()));
            }

        }
        for(int i=0;i<=(int)objectTypeStr.size()-1;i++){
            QAction *pObject = new QAction(objectTypeStr[i],pMenu);
            pObject->setObjectName(QString::number(i, 10));
            pMenu->addAction(pObject);
            connect(pObject,SIGNAL(triggered (bool)),this,SLOT(ObjectRun()));
        }
        pMenu->popup(mapToGlobal(event->pos()));
    }
}
void vekAppListView::mouseDoubleClickEvent(QMouseEvent * event){
    UNUSED(event);
    int index = this->currentIndex().row();
    if(index>-1){
        startApp(object_start);
    }
}
void vekAppListView::ObjectRun(){
    int index = this->currentIndex().row();
    if (index > -1)
    {
        QObject *object = QObject::sender();
        QAction *action_obnject = qobject_cast<QAction *>(object);
        int object_int=object_start;
        if(action_obnject!=nullptr){
            object_int= action_obnject->objectName().toInt();
        }
        objectExtend* _objectExtend=new objectExtend();
        objectType _objType=object_default;
        std::vector<QStringList> _codeAgrs;
        switch(object_int)
        {
        case object_winecfg:
            _objType=object_winecfg;
            break;
        case object_regedit:
            _objType=object_regedit;
            break;
        case object_control:
            _objType=object_control;
            break;
        case object_uninstall:
            _objType=object_uninstall;
            break;
        case object_winetricks_gui:
            _objType=object_winetricks_gui;
            break;
        case object_winetricks_cmd_libs:
            _objType=object_winetricks_cmd_libs;
            _codeAgrs=vekWinetricks_cArgs();
            if(_codeAgrs.empty()){
                return;
            }
            break;
        case object_start:
            _objType=object_start;
            break;
        case object_forcekill:
            _objType=object_forcekill;
            break;
        case object_debugstart:
            objectExtendApp();
            return;
        case object_setgame:
            setItemSlot();
            return;
        case object_deletegame:
            setUpDelData(pObject::getDockerData(mBox->tabText(mBox->currentIndex())),m_pModel->getItem(index),object_delApp);
            return;
        case object_exportJson:
            ExportJson();
            return;
        case object_packageDeb:
            pObject::vekTip("功能开发中!");
            return;
        }       
        if(_objType==object_start){
            startApp(object_start);
        }else{
            _objectExtend->setDockOptionObjectData(pObject::getDockerData(mBox->tabText(mBox->currentIndex())),m_pModel->getItem(index)->AppCID,_codeAgrs,_objType,objectWineBoot::object_wineboot_default,objectWineServer::object_wineserver_default);
            _objectExtend->start();
        }
    }
}
std::vector<QStringList> vekAppListView::vekWinetricks_cArgs(){
        QString dlgTitle="winetricks命令框";
        QString txtLabel="直接输入功能,多个库和功能空格隔开·例如:vcrun2012 vcrun2015";
        QString defaultInput;
        QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
        bool ok=false;
        QString arg = QInputDialog::getText(nullptr, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
        std::vector<QStringList> args;
        if (ok && !arg.isEmpty())
        {
            QStringList _args=arg.split(" ");
            args.push_back(_args);
        }
        return args;
}
void vekAppListView::startApp(objectType _objType){
    std::vector<QStringList> _codeAgrs;
    int index = this->currentIndex().row();
    if(index>-1){
        BaseDockData dockData=pObject::getDockerData(mBox->tabText(mBox->currentIndex()));
        BaseAppData* appData=m_pModel->getItem(index);
        if(!QDir(dockData.WinePath).exists()){
            pObject::vekTip("Wine路径丢失!");
            return;
        }
        QString pDock=dockData.DockerPath+"/"+dockData.DockerName;
        if(!QDir(pDock).exists()){
            pObject::vekTip("容器路径丢失!");
            return;
        }
        if(!QFile(appData->AppExe).exists()){
            pObject::vekTip(appData->AppName+"应用执行程序不存在!"+"路径:"+appData->AppExe);
            return;
        }
        objectExtend* _objectExtend=new objectExtend();
        taskList.push_back(appData->MainPrcoName);
        auto pObjectVek=this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget();
        connect(_objectExtend, SIGNAL(objexitTray(bool)), pObjectVek, SLOT(exitTray(bool)));
        emit _startTray();
        objectAppMT* oAMT=new objectAppMT(appData,&dockData);
        oAMT->sObjectInstall();
        delete oAMT;
        oAMT=nullptr;
        _objectExtend->setDockOptionObjectData(pObject::getDockerData(mBox->tabText(mBox->currentIndex())),appData->AppCID,_codeAgrs,_objType,objectWineBoot::object_wineboot_default,objectWineServer::object_wineserver_default);
        _objectExtend->start();
    }

}
void vekAppListView::ExportJson(){
    int index = this->currentIndex().row();
    if(_vExportJson==nullptr){
        BaseAppData bGameData=*m_pModel->getItem(index);
        _vExportJson=new vekExportJson();
        _vExportJson->setAttribute(Qt::WA_DeleteOnClose,true);
        _vExportJson->show();
        connect(_vExportJson,&vekExportJson::_unExportJson,this,&vekAppListView::unExportJson);
        _vExportJson->ExportJson(pObject::getDockerData(mBox->tabText(mBox->currentIndex())),bGameData.AppCID);
    }
}
void vekAppListView::objectExtendApp(){
    if(_vExtendDebug==nullptr){
        _vExtendDebug=new vekExtendDebug();
        _vExtendDebug->setAttribute(Qt::WA_DeleteOnClose,true);
        QString currentTabText =mBox->tabText(mBox->currentIndex());
        QString currentAppCID=m_pModel->getItem(this->currentIndex().row())->AppCID;
        _vExtendDebug->ConnectDebugObject(currentTabText,currentAppCID);
        connect(_vExtendDebug,&vekExtendDebug::_unVekDebug,this,&vekAppListView::unDebugApp);
        _vExtendDebug->show();
    }
}
//设置选项
void vekAppListView::setItemSlot(){
    int index = this->currentIndex().row();
    if (index > -1)
    {
        if(_vek_App_Add==nullptr){
            //绑定传参槽
            _vek_App_Add=new vekAppAddMT();
            _vek_App_Add->setAttribute(Qt::WA_DeleteOnClose,true);
            _vek_App_Add->setWindowTitle("Vek软件设置");
            QString currentAppCID=m_pModel->getItem(this->currentIndex().row())->AppCID;
            _vek_App_Add->vekAppAddConnectObject(&g_vekLocalData.dockerVec.at(mBox->tabText(mBox->currentIndex())),currentAppCID,object_setApp);
            _vek_App_Add->setWindowFlags(Qt::WindowStaysOnTopHint);
            _vek_App_Add->show();
            connect(_vek_App_Add,&vekAppAddMT::_unDiyAppAdd,this,&vekAppListView::unAppAdd);
            connect(_vek_App_Add,SIGNAL(_upData(BaseDockData,BaseAppData*,objectTypeView)),this,SLOT(setUpDelData(BaseDockData,BaseAppData*,objectTypeView)));
        }
    }
}
void vekAppListView::setUpDelData(BaseDockData dockData,BaseAppData* appData,objectTypeView objTypeView){
    int index = this->currentIndex().row();
    if(index>-1){
        QString deleteCID=m_pModel->getItem(index)->AppCID;
        QString dockPathStr=dockData.DockerPath+"/";
        QString dockNameStr=dockData.DockerName;
        objectJson _objectJson;
        bool dState=false;
        if(objTypeView==object_delApp){
            if(m_pModel->rowCount()<=1){
                 dState=pObject::vekMesg("提示:这是"+dockNameStr+"最后一个程序,若是执意删除则Vek将删除"+dockNameStr+"容器");
            }
            m_pModel->deleteItem(index);
            _objectJson.deleteAppNodeData(dockData,deleteCID);
            if(dState){
                deleteDockerTab(dockPathStr,dockNameStr);
                _objectJson.deleteDockerNodeData(dockData.DockerName);
            }
        }
        //修改设置
        if(objTypeView==object_setApp){
            QString currentTabText =mBox->tabText(mBox->currentIndex());
            m_pModel->deleteItem(index);
            auto pObjectVek=this->parentWidget()->parentWidget()->parentWidget();
            //判断设置后的dockName和当前是否相同，该功能是利用设置修改容器名
            if(currentTabText==dockData.DockerName){
                m_pModel->addItem(appData);
                _objectJson.updateAppNodeData(dockData,*appData);
                connect(this,SIGNAL(setUpGroupTabIcoSignal()),pObjectVek,SLOT(upTabIco()));
                emit setUpGroupTabIcoSignal();
            }else{
                //在新的容器中执行
                _objectJson.deleteAppNodeData(pObject::getDockerData(currentTabText),appData->AppCID);
                _objectJson.updateAppNodeData(dockData,*appData);     
                connect(this,SIGNAL(setUpDelDataSignal(BaseDockData*,BaseAppData*)),pObjectVek,SLOT(addAppObject(BaseDockData*,BaseAppData*)));
                emit setUpDelDataSignal(&dockData,appData);
            }  
        }
    }
}
void vekAppListView::deleteDockerTab(QString dockPathStr,QString dockNameStr){
    mBox->removeTab(mBox->currentIndex());
    if(dockPathStr!=nullptr&&dockNameStr!=nullptr){
        QDir dockPath(dockPathStr+dockNameStr);
        if(dockPath.exists()){
            dockPath.removeRecursively();
        }
    }
    g_vekLocalData.dockerVec.erase(dockNameStr);
    objectJson _oJson;
    _oJson.deleteDockerNodeData(dockNameStr);
    m_pListMap->erase(dockNameStr);
}
void vekAppListView::unAppAdd(){
    _vek_App_Add=nullptr;
}
void vekAppListView::unExportJson(){
    _vExportJson=nullptr;
}
void vekAppListView::unDebugApp(){
    _vExtendDebug=nullptr;
}

void vekAppListView::setListMap( std::map<QString,vekAppListView*> *pListMap,QTabWidget* pBox)
{
    m_pListMap = pListMap;
    mBox=pBox;
}

void vekAppListView::addItem(BaseAppData *pItem )
{
    m_pModel->addItem(pItem);
}

void vekAppListView::moveSlot()
{
    QAction *pSender = qobject_cast<QAction*>(sender());
    int index = this->currentIndex().row();
    if (pSender)
    {
        //根据点击的菜单，找到相应的列表，然后才能把图标转移过去
        vekAppListView *pList = m_ActionMap.find(pSender)->second;
        if (pList)
        {
            BaseAppData *pItem = m_pModel->getItem(index);
            pList->setViewMode(QListView::IconMode);
            pList->setFlow(QListView::LeftToRight);
            pList->addItem(pItem);
            m_pModel->deleteItem(index);
            QString dockName=m_ActionMap.find(pSender)->first->text();
            objectJson _objectJson;
            _objectJson.deleteAppNodeData(pObject::getDockerData(mBox->tabText(mBox->currentIndex())),pItem->AppCID);
            _objectJson.addAppNodeData(pObject::getDockerData(dockName),*pItem);
        }
    }
    //操作完了要把这个临时的映射清空
    m_ActionMap.clear();
}
