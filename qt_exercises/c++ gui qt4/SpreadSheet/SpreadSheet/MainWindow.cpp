#include "MainWindow.h"

#include "spreadsheet.h"
#include <Windows.h>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	m_Spreadsheet = new Spreadsheet;
	setCentralWidget(m_Spreadsheet);

	createActions();
	createMenus();
	createContextMenu();
	createToolbars();
	createStatusbar();

	readSettings();
	setWindowIcon(QIcon(":/res/images/tool.png"));
	setCurrentFile("");

}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (okToContinue())
	{
		writeSettings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void MainWindow::createActions()
{
	// 1. create file actions
	newAction = new QAction(tr("&New"), this);
	newAction->setIcon(QIcon(":/res/images/new.png"));
	newAction->setShortcut(QKeySequence::New);
	newAction->setStatusTip(tr("Create a spreadsheet file"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	openAction = new QAction(tr("&Open"), this);
	openAction->setIcon(QIcon(":/res/images/open.png"));
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(tr("Open a spreadsheet file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setIcon(QIcon(":/res/images/save.png"));
	saveAction->setStatusTip(tr("Save a spreadsheet file"));
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save));

	saveAsAction = new QAction(tr("Save &As..."), this);
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	for (int i = 0; i < MaxRecentFiles; i++)
	{
		recentFilesActions[i] = new QAction(this);
		recentFilesActions[i]->setVisible(false);
		connect(recentFilesActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	// 2. init edit actions
	cutAction = new QAction(tr("Cu&t"), this);
	cutAction->setShortcut(QKeySequence::Cut);
	cutAction->setStatusTip(tr("Cut the content"));
	cutAction->setIcon(QIcon(":/res/images/cut.png"));
	connect(cutAction, SIGNAL(triggered()), m_Spreadsheet, SLOT(cut()));

	copyAction = new QAction(tr("&Copy"), this);
	copyAction->setShortcut(QKeySequence::Copy);
	copyAction->setIcon(QIcon(":/res/images/copy.png"));
	connect(copyAction, SIGNAL(triggered()), m_Spreadsheet, SLOT(copy()));

	pasteAction = new QAction(tr("&Paste"), this);
	pasteAction->setShortcut(QKeySequence::Paste);
	pasteAction->setIcon(QIcon(":/res/images/paste.png"));
	connect(pasteAction, SIGNAL(triggered()), m_Spreadsheet, SLOT(paste()));

	deleteAction = new QAction(tr("Delete"), this);
	deleteAction->setShortcut(QKeySequence::Delete);
	connect(deleteAction, SIGNAL(triggered()), m_Spreadsheet, SLOT(del()));

	selectRowAction = new QAction(tr("&Row"), this);
	connect(selectRowAction, SIGNAL(triggered()), m_Spreadsheet, SLOT(selectCurrentRow()));

	selectColumnAciton = new QAction(tr("&Column"), this);
	connect(selectColumnAciton, SIGNAL(triggered()), m_Spreadsheet, SLOT(selectCurrentColumn()));

	selectAllAction = new QAction(tr("&All"), this);
	selectAllAction->setShortcut(QKeySequence::SelectAll);
	connect(selectAllAction, SIGNAL(triggered()), m_Spreadsheet, SLOT(selectAll()));

	findAction = new QAction(tr("&Find"), this);
	findAction->setShortcut(QKeySequence::Find);
	findAction->setIcon(QIcon(tr(":/res/images/find.png")));
	connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

	goToCellAction = new QAction(tr("&Go to Cell"), this);
	goToCellAction->setShortcut(tr("Ctrl+G"));
	goToCellAction->setIcon(QIcon(":/res/images/goto.png"));
	connect(goToCellAction, SIGNAL(triggered()), this, SLOT(goToCell()));

	// 3. init tools actions
	recalculateAction = new QAction(tr("&Recalculate"), this);
	recalculateAction->setShortcut(tr("F9"));
	connect(recalculateAction, SIGNAL(triggered()), m_Spreadsheet, SLOT(recalculate()));

	sortAction = new QAction(tr("&Sort"), this);
	connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));

	// 4. init options actions
	showGridAction = new QAction(tr("&Show Grid"), this);
	showGridAction->setCheckable(true);
	showGridAction->setChecked(m_Spreadsheet->showGrid());
	connect(showGridAction, SIGNAL(toggled(bool)), m_Spreadsheet, SLOT(setShowGrid(bool)));

	autoRecalcualteAction = new QAction(tr("&Auto-Recalculate"), this);
	autoRecalcualteAction->setCheckable(true);
	autoRecalcualteAction->setChecked(m_Spreadsheet->autoRecalculate());
	connect(autoRecalcualteAction, SIGNAL(toggled(bool)), m_Spreadsheet, SLOT(setAutoRecalculate()));

	// 5. init help actions
	aboutAction = new QAction(tr("&About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("About &Qt"), this);
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	// 1. init file menu
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	separatorAction = fileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; i++)
	{
		fileMenu->addAction(recentFilesActions[i]);
	}
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	// 2. init edit menu
	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAction);
	editMenu->addAction(copyAction);
	editMenu->addAction(pasteAction);
	editMenu->addAction(deleteAction);
	// 2.1 init select menu
	selectSubMenu = new QMenu(tr("&Select"), this);
	selectSubMenu->addAction(selectRowAction);
	selectSubMenu->addAction(selectColumnAciton);
	selectSubMenu->addAction(selectAllAction);
	editMenu->addMenu(selectSubMenu);
	editMenu->addSeparator();
	editMenu->addAction(findAction);
	editMenu->addAction(goToCellAction);

	// 3. init tool menu
	toolMenu = menuBar()->addMenu(tr("&Tools"));
	toolMenu->addAction(recalculateAction);
	toolMenu->addAction(sortAction);

	// 4. init options menu
	optionMenu = menuBar()->addMenu(tr("&Options"));
	optionMenu->addAction(showGridAction);
	optionMenu->addAction(autoRecalcualteAction);

	// 5. init help menu
	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

void MainWindow::createContextMenu()
{
	m_Spreadsheet->addAction(cutAction);
	m_Spreadsheet->addAction(copyAction);
	m_Spreadsheet->addAction(pasteAction);
	m_Spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolbars()
{
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	editToolBar = addToolBar(tr("&Edit"));
	editToolBar->addAction(cutAction);
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);
	editToolBar->addSeparator();
	editToolBar->addAction(findAction);
	editToolBar->addAction(goToCellAction);
}

void MainWindow::createStatusbar()
{
	locationLabel = new QLabel(tr("W999"));
	locationLabel->setAlignment(Qt::AlignCenter);
	locationLabel->setMinimumSize(QSize(100, 20));

	formulaLabel = new QLabel("sadfasdfasfasdfas");
	formulaLabel->setIndent(3);

	statusBar()->addWidget(locationLabel);
	statusBar()->addWidget(formulaLabel, 1);
	
	connect(m_Spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
	connect(m_Spreadsheet, SIGNAL(modified()), this, SLOT(spreadsheetModified()));

	updateStatusBar();
}

void MainWindow::readSettings()
{
	QSettings settings("Software Inc.", "Spradsheet");

	restoreGeometry(settings.value("geometry").toByteArray());

	recentFiles = settings.value("recentFiles").toStringList();
	updateRecentFileActions();

	bool showGrid = settings.value("showGrid", true).toBool();
	showGridAction->setChecked(showGrid);

	bool autoRecalc = settings.value("autoRecalc", true).toBool();
	autoRecalcualteAction->setChecked(autoRecalc);

}

void MainWindow::writeSettings()
{
	QSettings settings("Software Inc.", "Spradsheet");
	
	settings.setValue("geometry", saveGeometry());
	settings.setValue("recentFiles", recentFiles);
	settings.setValue("showGrid", showGridAction->isChecked());
	settings.setValue("autoRecalc", autoRecalcualteAction->isChecked());
}

bool MainWindow::okToContinue()
{
	if (isWindowModified())
	{
		int select = QMessageBox::warning(this, tr("Spreasheet"), 
			tr("The doucuement has ben modified.\ndo you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel);
		if (select == QMessageBox::Yes)
		{
			return save();
		}
		else if (select == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

bool MainWindow::loadFile(const QString& fileName)
{
	if (!m_Spreadsheet->readFile(fileName))
	{
		statusBar()->showMessage(tr("Loading canceled"), 2000);
		return false;
	}
	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
	return true;
}

bool MainWindow::saveFile(const QString& fileName)
{
	if (!m_Spreadsheet->writeFile(fileName))
	{
		statusBar()->showMessage(tr("Saving canceled"), 2000);
		return false;
	}
	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void MainWindow::setCurrentFile(const QString& fileName)
{
	curFile = fileName;
	setWindowModified(false);
	
	QString showName = tr("Untitled");
	if (!curFile.isEmpty())
	{
		showName = strippedName(curFile);
		recentFiles.removeAll(curFile);
		recentFiles.prepend(curFile);
		updateRecentFileActions();
	}
	setWindowTitle(tr("%1[*] - %2").arg(showName).arg(tr("Spreadsheet")));
}

void MainWindow::updateRecentFileActions()
{
	QMutableStringListIterator i(recentFiles);
	while (i.hasNext()){
		if (!QFile::exists(i.next())){
			i.remove();
		}
	}

	for (int i = 0; i < MaxRecentFiles; i++){
		if (i < recentFiles.count()) {
			QString text = tr("&%1  %2").arg(i + 1).arg(strippedName( recentFiles[i]));
			recentFilesActions[i]->setVisible(true);
			recentFilesActions[i]->setText(text);
			recentFilesActions[i]->setData(recentFiles[i]);
		}
		else {
			recentFilesActions[i]->setVisible(false);
		}
	}

	separatorAction->setVisible(!recentFiles.isEmpty());
}

QString MainWindow::strippedName(const QString& fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void MainWindow::newFile()
{
	if (okToContinue())
	{
		m_Spreadsheet->clear();
		setCurrentFile("");
	}

}

void MainWindow::open()
{
	if (okToContinue())
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spradsheet"), ".",
			tr("Spreadsheet files (*.sp)"));
		if (!fileName.isEmpty())
		{
			loadFile(fileName);
		}
	}
}

bool MainWindow::save()
{
	if (curFile.isEmpty())
	{
		return saveAs();
	}
	else
	{
		return saveFile(curFile);
	}
	return false;
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spreadsheet"),
		".", tr("Spreadsheet files (*.sp)"));
	if (fileName.isEmpty())
	{
		return false;
	}
	return saveFile(fileName);
}

void MainWindow::find()
{
}

void MainWindow::goToCell()
{
}

void MainWindow::sort()
{
}

void MainWindow::about()
{

}

void MainWindow::openRecentFile()
{
	if (okToContinue()){
		QAction* action = qobject_cast<QAction*>(sender());
		if (action){
			loadFile(action->data().toString());
		}
	}
}

void MainWindow::updateStatusBar()
{
	locationLabel->setText(m_Spreadsheet->currentLocation());
	formulaLabel->setText(m_Spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
	setWindowModified(true);
	updateStatusBar();
}
