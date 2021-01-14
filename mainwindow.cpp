#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(500, 400);
    this->setUnifiedTitleAndToolBarOnMac(true);
    this->grabGesture(Qt::PanGesture);

    wordcount = 0;
    wordsPerPage = 250;
    pagecount = 0;
    wordsPerMinute = 150;
    readtime = 0;
    difficulty = 100;

    showWordcount = true;
    showPagecount = false;
    showReadtime = false;
    showDifficulty = false;

    textwidth = 80;

    // install translator
    translator = new QTranslator();
    locale = QLocale::system().name();
    translator->load("Editor_" + locale + ".qm");
    QApplication::installTranslator(translator);

    // TextEdit
    textEdit = new TextEditor(this);
    this->setCentralWidget(textEdit);

    // FindDock
    findDock = new FindDock(this);
    connect(findDock, &FindDock::findRequested, textEdit, &TextEditor::findRequested);
    connect(findDock, &FindDock::replaceRequested, textEdit, &TextEditor::replaceRequested);
    connect(findDock, &FindDock::replaceAllRequested, textEdit, &TextEditor::replaceAllRequested);
    this->addDockWidget(Qt::TopDockWidgetArea, findDock);
    findDock->setVisible(false);

    // SettingsDock
    settingsDock = new SettingsDock(this, locale);
    this->addDockWidget(Qt::RightDockWidgetArea, settingsDock);
    connect(settingsDock, &SettingsDock::lightThemeRequested, this, &MainWindow::setLightTheme);
    connect(settingsDock, &SettingsDock::darkThemeRequested, this, &MainWindow::setDarkTheme);
    connect(settingsDock, &SettingsDock::setEnableFixedTextwidthRequested, textEdit, &TextEditor::limitTextWidth);
    connect(settingsDock, &SettingsDock::textwidthChangeRequested, this, &MainWindow::setTextWidth);
    connect(settingsDock, &SettingsDock::fontChangeRequested, textEdit, &TextEditor::setFont);
    connect(settingsDock, &SettingsDock::fontSizeChangeRequested, textEdit, &TextEditor::setFontSize);
    connect(settingsDock, &SettingsDock::languageChangeRequested, this, &MainWindow::selectLanguage);
    connect(settingsDock, &SettingsDock::showWordcountRequested, this, &MainWindow::setShowWordcount);
    connect(settingsDock, &SettingsDock::showPagecountRequested, this, &MainWindow::setShowPagecount);
    connect(settingsDock, &SettingsDock::showReadtimeRequested, this, &MainWindow::setShowReadtime);
    connect(settingsDock, &SettingsDock::showDifficultyRequested, this, &MainWindow::setShowDifficulty);
    settingsDock->setVisible(false);

    // ToolBar
    toolbar = new QToolBar(this);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    toolbar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextOnly);
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setAllowedAreas(Qt::ToolBarArea::TopToolBarArea);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    openAction = new QAction(this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::open);
    toolbar->addAction(openAction);

    saveAction = new QAction(this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    toolbar->addAction(saveAction);

    saveasAction = new QAction(this);
    saveasAction->setShortcut(QKeySequence::SaveAs);
    connect(saveasAction, &QAction::triggered, this, &MainWindow::saveas);
    toolbar->addAction(saveasAction);

    findAction = new QAction(this);
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction, &QAction::triggered,
            this, [=]() {findDock->setVisible(!findDock->isVisible());});
    toolbar->addAction(findAction);

    undoAction = new QAction(this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, textEdit, &TextEditor::undo);
    toolbar->addAction(undoAction);

    redoAction = new QAction(this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, textEdit, &TextEditor::redo);
    toolbar->addAction(redoAction);

    statisticsLabel = new QLabel();
    statisticsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    statisticsLabel->setAlignment(Qt::AlignRight);
    statisticsLabel->setTextFormat(Qt::TextFormat::MarkdownText);
    statisticsLabel->setContentsMargins(20, 5, 20, 5);
    connect(textEdit, &TextEditor::textAnalyzed, this, &MainWindow::statisticsChanged);
    toolbar->addWidget(statisticsLabel);

    optionsAction = new QAction(this);
    connect(optionsAction, &QAction::triggered,
            this, [=]() {settingsDock->setVisible(!settingsDock->isVisible());});
    toolbar->addAction(optionsAction);

    this->addToolBar(toolbar);

    this->retranslate();
    //this->setDarkTheme();
}


void MainWindow::retranslate() {
    openAction->setText(tr("Open"));
    saveAction->setText(tr("Save"));
    saveasAction->setText(tr("Save as"));
    findAction->setText(tr("Find"));
    undoAction->setText(tr("Undo"));
    redoAction->setText(tr("Redo"));
    statisticsChanged(wordcount);
    optionsAction->setText(tr("Options"));

    findDock->retranslate();
    settingsDock->retranslate();
}


void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();

    if(wordcount > 0) {
        this->save();
    }
    event->accept();
}


void MainWindow::changeEvent(QEvent *event) {
    if(event->type() == QEvent::LanguageChange) {
        this->retranslate();
    }

    QMainWindow::changeEvent(event);
}


void MainWindow::readSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        this->resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        this->move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        this->restoreGeometry(geometry);
    }
}


void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}


void MainWindow::open() {
    currentFile = QFileDialog::getOpenFileName(this, tr("Open"));

    QFile file(currentFile);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot open file: ") + file.errorString());
        return;
    }

    this->setWindowTitle(QFileInfo(currentFile).fileName());
    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());

    file.close();
}


void MainWindow::save() {
    if (currentFile.isEmpty()) {
        currentFile = QFileDialog::getSaveFileName(this, tr("Save"));
    }

    saveToDisk(currentFile);
}


void MainWindow::saveas() {
    currentFile = QFileDialog::getSaveFileName(this, tr("Save"));
    saveToDisk(currentFile);
}


void MainWindow::saveToDisk(QString &filename) {
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot save file: ") + file.errorString());
        return;
    }

    this->setWindowTitle(QFileInfo(filename).fileName());
    QTextStream out(&file);
    QString text = textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
}


void MainWindow::selectLanguage(QString locale) {
    QApplication::removeTranslator(translator);
    translator->load(":translations/Editor_" + locale + ".qm");
    QApplication::installTranslator(translator);

    this->locale = locale;
}


void MainWindow::setDarkTheme() {
    QFile style(":/styles/dark_style.qss");
    style.open(QFile::ReadOnly);
    this->setStyleSheet(style.readAll());
}


void MainWindow::setLightTheme() {
    QFile style(":/styles/light_style.qss");
    style.open(QFile::ReadOnly);
    this->setStyleSheet(style.readAll());
}


void MainWindow::statisticsChanged(const int wordcount) {
    this->wordcount = wordcount;
    pagecount = wordcount / wordsPerPage;
    readtime = wordcount / wordsPerMinute;

    QString showntext = "";

    if(showWordcount) {
        if(wordcount == 1) {
            showntext += tr("1 word");
        } else {
            showntext += QString::number(wordcount) + tr(" words");
        }
    }

    if(showPagecount) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        if(pagecount == 1) {
            showntext += tr("1 page");
        } else {
            showntext += QString::number(pagecount) + tr(" pages");
        }
    }

    if(showReadtime) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        showntext += QString::number(readtime / 60) + tr("h ")
                     + QString::number(readtime % 60) + tr("m");
    }

    if(showDifficulty) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        showntext += tr(" level: ") + QString::number(difficulty);
    }

    statisticsLabel->setText(showntext);
}


MainWindow::~MainWindow()
{
}

