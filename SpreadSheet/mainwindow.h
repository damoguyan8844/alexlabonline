#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;
class QDir;
class QActionGroup;
class QTranslator;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	QDir directoryOf(const QString &subdir);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void find();
	void goToCell();
	void sort();
	void about();
	void openRecentFile();
	void updateStatusBar();
	void spreadsheetModified();
	void switchLanguage(QAction *action);
private:
	void createActions();
	void createMenus();
	void createContextMenu();
	void createToolBars();
	void createStatusBar();
	void createLanguageMenu();

	void retranslateUi();
	void readSettings();
	void writeSettings();
	bool okToContinue();
	bool loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	QString strippedName(const QString &fullFileName);

	Spreadsheet *spreadsheet;
	FindDialog *findDialog;
	QLabel *locationLabel;
	QLabel *formulaLabel;
	QStringList recentFiles;
	QString curFile;

	enum { MaxRecentFiles = 5 };
	QAction *recentFileActions[MaxRecentFiles];
	QAction *separatorAction;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *selectSubMenu;
	QMenu *toolsMenu;
	QMenu *optionsMenu;
	QMenu *helpMenu;
	QMenu *languageMenu;

	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QAction *newAction;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction;
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *deleteAction;
	QAction *selectRowAction;
	QAction *selectColumnAction;
	QAction *selectAllAction;
	QAction *findAction;
	QAction *goToCellAction;
	QAction *recalculateAction;
	QAction *sortAction;
	QAction *showGridAction;
	QAction *autoRecalcAction;
	QAction *aboutAction;
	QAction *aboutQtAction;

	QActionGroup * languageActionGroup;
	QTranslator  * qtTranslator;
	QTranslator  * appTranslator;
};

#endif
