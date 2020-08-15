#include "vekInitObject.h"

void vekTip(QString tipInfo){
    QMessageBox::warning(nullptr,"TIP",tipInfo);
}
void vekError(QString tipInfo){
    QMessageBox::warning(nullptr,"ERROR",tipInfo);
}
bool vekMesg(QString tipText){
    QMessageBox msgBox;
    msgBox.setInformativeText(tipText);
    msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    int msgRet=msgBox.exec();
    if(msgRet==QMessageBox::Ok){
        return true;
    }
    return false;
}

BaseDockData GetDockerData(QString dockName){
    BaseDockData tempDockData={};
    if(!g_vekLocalData.dockerVec.empty()){
        for(auto a:g_vekLocalData.dockerVec){
            if(a.first==dockName){
                tempDockData=a.second;
                break;
            }
        }
    }
    return tempDockData;
}
BaseAppData GetDockerData(BaseDockData dockData,QString appCID){
    BaseAppData tempAppData={};
    for(auto a:dockData.dData){
        if(a.first==appCID){
            tempAppData=a.second;
            break;
        }
    }
    return tempAppData;
}
QString getFileStr(QString filePath){
    QFile file(filePath);
    if(!file.exists())
    {
        return nullptr;
    }
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QString temp = file.readAll();
    file.close();
    return temp;
}
void saveStrToFile(QString strFile,QString filePath){
    QFile file(filePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream out(&file);
    out << strFile;
    file.close();
}
//综合注册表操作
QStringList DockRegeditStr(QString rObj,QString rPath,QString rKey,QString rTValue,QString rValue){
    QStringList argsList;
    //处理注册表路径空格
    if(rPath.contains(" ",Qt::CaseSensitive)){
        rPath="\""+rPath+"\"";
    }
    //处理注册表键空格
    if(rKey.contains(" ",Qt::CaseSensitive)){
        rKey="\""+rKey+"\"";
    }
    //处理注册表值空格
    if(rValue.contains(" ",Qt::CaseSensitive)){
        rValue="\""+rValue+"\"";
    }
    if(rObj=="add"){
        argsList.append("reg");
        argsList.append(rObj);
        argsList.append(rPath);
        argsList.append("/v");
        argsList.append(rKey);
        argsList.append("/t");
        argsList.append(rTValue);
        argsList.append("/d");
        argsList.append(rValue);
        argsList.append("/f");
    }
    if(rObj=="delete"){
        argsList.append("reg");
        argsList.append(rObj);
        argsList.append(rPath);
        argsList.append("/v");
        argsList.append(rKey);
        argsList.append("/f");
    }
    return argsList;
}
//加载Default Style
void vek_Style(QWidget *parent,int styleType)
{
    QString strStyle;
    parent->setWindowIcon(QIcon(":/res/img/vek.ico"));
    switch(styleType){
    case 0:
        strStyle="Default.qss";
        break;
    case 1:
        strStyle="Dark.qss";
    case 2:
        break;
        strStyle="Light.qss";
    }
    QFile file(":/res/css/"+strStyle);
    file.open(QFile::ReadOnly|QFile::Text);
    if (file.isOpen())
    {
        QString styleSheet = QString::fromUtf8(file.readAll());
        parent->setStyleSheet(styleSheet);
        file.close();
    }
}
void qwidgetGeometry(QWidget* qwidget){
    QDesktopWidget *deskdop = QApplication::desktop();
    qwidget->move((deskdop->width() - qwidget->width())/2, (deskdop->height() - qwidget->height())/2);
}
