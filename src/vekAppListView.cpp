﻿#include "vekAppListView.h"


vekAppListView::vekAppListView( QWidget *parent):QListView(parent)
{
    parent=nullptr;
    m_hitIndex = -1;
    m_pModel = new vekAppData;
    this->setModel(m_pModel);
    m_pListMap = NULL;}

vekAppListView::~vekAppListView()
{
}
void vekAppListView::contextMenuEvent( QContextMenuEvent * event )
{
    int hitIndex = this->indexAt(event->pos()).row();
    if (hitIndex > -1)
    {
        //增加容器转移功能
        QMenu *pSubMenu = NULL;
        pMenu = new QMenu(this);
        std::map<QString,vekAppListView*>::iterator it = m_pListMap->begin();
        for (it;it != m_pListMap->end();++it)
        {
            if (!pSubMenu)
            {
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
        case object_start:
            _objType=object_start;
            break;
        case object_forcekill:
            _objType=object_forcekill;
            break;
        case object_debugstart:
            objectExtendApp();
            return;
            break;
        case object_setgame:
            setItemSlot();
            return;
            break;
        case object_deletegame:
            setUpDelData(m_pModel->getItem(index),object_delApp);
            return;
            break;
        case object_exportJson:
            ExportJson();
            return;
            break;
        }
        std::vector<QStringList> _codeAgrs;
        if(_objType==object_start){
            taskList.push_back(m_pModel->getItem(index)->mainPrcoName);
            auto pObjectVek=this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget();
            connect(_objectExtend, SIGNAL(objexitTray(bool)), pObjectVek, SLOT(exitTray(bool)));
            emit _startTray();
            objectAppAddMT* oAMT=new objectAppAddMT(m_pModel->getItem(index),nullptr);
            oAMT->sObjectInstall();
            delete oAMT;
            oAMT=nullptr;
        }
        connect(this, SIGNAL(toObjectArgs(BaseAppData,std::vector<QStringList>,objectType,objectWineBoot,objectWineServer)), _objectExtend, SLOT(setDockOptionObjectData(BaseAppData,std::vector<QStringList>,objectType,objectWineBoot,objectWineServer)));
        emit(toObjectArgs(*m_pModel->getItem(index),_codeAgrs,_objType,objectWineBoot::object_wineboot_default,objectWineServer::object_wineserver_default));
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
        _vExportJson->ExportJson(bGameData);
    }
}
void vekAppListView::objectExtendApp(){
    if(_vExtendDebug==nullptr){
        _vExtendDebug=new vekExtendDebug();
        _vExtendDebug->setAttribute(Qt::WA_DeleteOnClose,true);
        _vExtendDebug->setGeometry(this->geometry());
        _vExtendDebug->ConnectDebugObject();
        _vExtendDebug->_data=*m_pModel->getItem(this->currentIndex().row());
        connect(_vExtendDebug,&vekExtendDebug::_unVekDebug,this,&vekAppListView::unDebugApp);
        _vExtendDebug->show();
    }
}
//设置选项
void vekAppListView::setItemSlot(){
    int index = this->currentIndex().row();
    if (index > -1)
    {
        BaseAppData* bGameData=m_pModel->getItem(index);
        if(_vek_App_Add==nullptr){
            //绑定传参槽
            _vek_App_Add=new vekAppAddMT();
            connect(this, SIGNAL(toObjectArgs_ptr(BaseAppData*,objectTypeView)), _vek_App_Add, SLOT(vekAppAddConnectObject(BaseAppData*,objectTypeView)));
            _vek_App_Add->setAttribute(Qt::WA_DeleteOnClose,true);
            _vek_App_Add->setGeometry(this->geometry());
            _vek_App_Add->setWindowTitle("VekAppSet");
            emit(toObjectArgs_ptr(bGameData,object_setApp));
            _vek_App_Add->show();
            connect(_vek_App_Add,&vekAppAddMT::_unDiyAppAdd,this,&vekAppListView::unAppAdd);
            connect(_vek_App_Add,SIGNAL(_upData(BaseAppData*,objectTypeView)),this,SLOT(setUpDelData(BaseAppData*,objectTypeView)));
        }
    }
}
void vekAppListView::setUpDelData(BaseAppData* data,objectTypeView objTypeView){
    int index = this->currentIndex().row();
    if(index>-1){
        QString deleteCID=m_pModel->getItem(index)->appCID;
        QString dockPathStr=m_pModel->getItem(index)->dockPath+"/";
        QString dockNameStr=m_pModel->getItem(index)->dockName;    
        if(objTypeView==object_delApp){
            if(m_pModel->rowCount()<=1){
                if(vekMesg("提示:这是"+dockNameStr+"最后一个程序,若是执意删除则Vek将删除"+dockNameStr+"容器")){
                  deleteDockerTab(dockPathStr,dockNameStr);
                }else{
                    return;
                }
            }
            m_pModel->deleteItem(index);
            objectJson _objectJson;
            _objectJson.deleteAppNodeData(deleteCID);
        }
        if(objTypeView==object_setApp){
            m_pModel->deleteItem(index);
            QString currentTabText =mBox->tabText(mBox->currentIndex());
            if(currentTabText==data->dockName){
               m_pModel->addItem(data);
            }else{
                if(m_pModel->rowCount()<=0){
                    deleteDockerTab(dockPathStr,dockNameStr);
                }
                auto pObjectVek=this->parentWidget()->parentWidget()->parentWidget();
                connect(this,SIGNAL(setUpDelDataSignal(BaseAppData*)),pObjectVek,SLOT(addAppObject(BaseAppData*)));
                emit setUpDelDataSignal(data);
            }
        }
    }
}
void vekAppListView::deleteDockerTab(QString dockPathStr,QString dockNameStr){
        mBox->removeTab(mBox->currentIndex());
        if(dockPathStr!=NULL&dockNameStr!=NULL){
            QDir dockPath(dockPathStr+dockNameStr);
            if(dockPath.exists()){
                dockPath.removeRecursively();
            }
        }
        g_vekLocalData.dockerVec.erase(dockNameStr);
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
        if(index-1<=-1){
            vekTip("移动失败!移动后容器程序列表不能为空");
            return;
        }
        //根据点击的菜单，找到相应的列表，然后才能把图标转移过去
        vekAppListView *pList = m_ActionMap.find(pSender)->second;
        if (pList)
        {          
            BaseAppData *pItem = m_pModel->getItem(index);
            pList->setViewMode(QListView::IconMode);
            pList->setFlow(QListView::LeftToRight);
            pList->addItem(pItem);
            m_pModel->deleteItem(index);
            pItem->dockName=m_ActionMap.find(pSender)->first->text();
            objectJson _objectJson;
            _objectJson.deleteAppNodeData(pItem->appCID);
            _objectJson.updateAppNodeData(pItem->dockName,*pItem);
        }
    }
    //操作完了要把这个临时的映射清空
    m_ActionMap.clear();
}