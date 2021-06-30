#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>

class Spreadsheet;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolbars();
    void createStatusbar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    void updateRecentFileActions();
    QString strippedName(const QString& fullFileName);

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

private:
    QStringList recentFiles;
    QString curFile;

    Spreadsheet* m_Spreadsheet;

    QMenu* editMenu;
    QMenu* fileMenu;
    QMenu* toolMenu;
    QMenu* optionMenu;
    QMenu* helpMenu;

    QToolBar* fileToolBar;
    QToolBar* editToolBar;

    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    enum { MaxRecentFiles = 5 };
    QAction* separatorAction;
    QAction* recentFilesActions[MaxRecentFiles];
    QAction* exitAction;

    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* selectRowAction;
    QAction* selectColumnAciton;
    QAction* selectAllAction;
    QAction* deleteAction;
    QMenu* selectSubMenu;
    QAction* findAction;
    QAction* goToCellAction;

    QAction* recalculateAction;
    QAction* sortAction;
    
    QAction* showGridAction;
    QAction* autoRecalcualteAction;

    QAction* aboutAction;
    QAction* aboutQtAction;

    
    QLabel* locationLabel;
    QLabel* formulaLabel;

};
