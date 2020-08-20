#ifndef VEKSTYLE_H
#define VEKSTYLE_H

#include "common.h"
static std::vector<QString> styleNames= {"Default","Dark","Light"};
QStringList DockRegeditStr(QString rObj,QString rPath,QString rKey,QString rTValue,QString rValue);
void vek_Style(QWidget *parent,int styleType);
void vekTip(QString tipInfo);
void vekError(QString tipInfo);
bool vekMesg(QString tipText);
QString getFileStr(QString filePath);
void saveStrToFile(QString strFile,QString filePath);
void qwidgetGeometry(QWidget*);
BaseDockData GetDockerData(QString dockName);
BaseAppData GetAppData(BaseDockData dockData,QString appCID);
void SaveDockerDataToJson(BaseDockData dockData,QString dockName);
void AddAppDataToJson(BaseDockData dockData,BaseAppData appData);
void SaveAppDataToJson(BaseDockData dockData,BaseAppData appData);
void DeleteWineDataToJson(QString wineName);
#endif // LOADSTYLE_H
