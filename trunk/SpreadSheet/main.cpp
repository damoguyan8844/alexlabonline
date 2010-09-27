#include <QApplication>
#include <QLibraryInfo>
#include <QDir>

#include "mainwindow.h"

namespace
{
// 	void deleteDirectory(QFileInfo fileList){
// 
// 		if(fileList.isDir()){
// 			int childCount =0;
// 			QString dir = fileList.filePath();
// 			QDir thisDir(dir);
// 			childCount = thisDir.entryInfoList().count();
// 			QFileInfoList newFileList = thisDir.entryInfoList();
// 			if(childCount>2){
// 				for(int i=0;i<childCount;i++){
// 					if(newFileList.at(i).fileName().operator ==(".")|newFileList.at(i).fileName().operator ==("..")){
// 						continue;
// 					}
// 					deleteDirectory(newFileList.at(i));
// 				}
// 			}
// 			fileList.absoluteDir().rmpath(fileList.fileName());
// 		}else if(fileList.isFile()){
// 			fileList.absoluteDir().remove(fileList.fileName());
// 		}
// 
// 	}

}
int main(int argc, char *argv[])
{
	// 	QStringList dirList;
	// 	dirList<<QLatin1String("D:/Qt/4.7.0");
	// 
	// 	QLatin1String tmpPath("/tmp");
	// 
	// 	while(!dirList.isEmpty())
	// 	{
	// 		QDir dir(dirList.takeFirst());
	// 
	// 		deleteDirectory(dir.absolutePath()+tmpPath);
	// 
	// 		QStringList files = dir.entryList(QStringList() << "*", QDir::Dirs, QDir::Name);
	// 		files.removeOne(".");
	// 		files.removeOne("..");
	// 
	// 		foreach( const QString & subDir , files )
	// 		{
	// 			dirList.push_back(dir.absolutePath() +QObject::tr("/")+subDir);
	// 		}
	// 	}

	QApplication app(argc, argv);
	MainWindow mainWin;
	mainWin.show();
	return app.exec();
}
