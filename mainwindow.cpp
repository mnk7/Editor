#include "mainwindow.h"

MainWindow::MainWindow(QString currentFile, QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(500, 400);
    this->setUnifiedTitleAndToolBarOnMac(true);
    this->grabGesture(Qt::PanGesture);

    this->currentFile = currentFile;

    autosaveTimer = new QTimer(this);
    connect(autosaveTimer, &QTimer::timeout, this, &MainWindow::save);
    autosaveTimer->start(settings.getAutosaveInterval() * 60000);

    connect(QApplication::instance(), &QApplication::aboutToQuit, this, [=]() {
                                                                            if(textEdit->toPlainText().size() > 0) {
                                                                                this->save();
                                                                            }
                                                                        });

    // install translator
    translator = new QTranslator();

    // SpellChecker
    if (temporaryDictDir.isValid()) {
        QFile::copy(":/dictionaries/dictionaries/en/en_GB.aff", temporaryDictDir.path() + "/en.aff");
        QFile::copy(":/dictionaries/dictionaries/en/en_GB.dic", temporaryDictDir.path() + "/en.dic");
        QFile::copy(":/dictionaries/dictionaries/de/de_DE_frami.aff", temporaryDictDir.path() + "/de.aff");
        QFile::copy(":/dictionaries/dictionaries/de/de_DE_frami.dic", temporaryDictDir.path() + "/de.dic");
    }

    // have to request WRITE_EXTERNAL_STORAGE for Android
    //QString applicationDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    spellchecker = new SpellChecker(getDictionary(settings.getLocale()), "");

    // TextEdit
    textEdit = new TextEditor(this, &statistics, spellchecker);
    this->setCentralWidget(textEdit);

    // FindDock
    findDock = new FindDock(this);
    connect(findDock, &FindDock::findRequested, textEdit, &TextEditor::findRequested);
    connect(findDock, &FindDock::replaceRequested, textEdit, &TextEditor::replaceRequested);
    connect(findDock, &FindDock::replaceAllRequested, textEdit, &TextEditor::replaceAllRequested);
    this->addDockWidget(Qt::TopDockWidgetArea, findDock);
    findDock->setVisible(false);

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
    connect(openAction, &QAction::triggered, this, [=]() {
                                                       if(textEdit->toPlainText().size() > 0) {
                                                           this->save();
                                                       }
                                                       this->open();
                                                   });
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

    statisticsLabel = new StatisticsLabel(&settings, &statistics, this);
    connect(textEdit, &TextEditor::textAnalyzed, statisticsLabel, &StatisticsLabel::statisticsChanged);
    toolbar->addWidget(statisticsLabel);

    optionsAction = new QAction(this);
    connect(optionsAction, &QAction::triggered,
            this, [=]() {settingsDock->setVisible(!settingsDock->isVisible());});
    toolbar->addAction(optionsAction);

    this->addToolBar(toolbar);

    // read settings
    settings.setFont(textEdit->font());
    settings.setFontsize(settings.getFont().pointSize());

    loadSettings();

    // SettingsDock
    settingsDock = new SettingsDock(this, &settings);
    this->addDockWidget(Qt::RightDockWidgetArea, settingsDock);
    connect(settingsDock, &SettingsDock::lightThemeRequested, this, &MainWindow::setLightTheme);
    connect(settingsDock, &SettingsDock::darkThemeRequested, this, &MainWindow::setDarkTheme);
    connect(settingsDock, &SettingsDock::setEnableFixedTextwidthRequested, this, &MainWindow::setLimitTextwidth);
    connect(settingsDock, &SettingsDock::textwidthChangeRequested, this, &MainWindow::setTextWidth);
    connect(settingsDock, &SettingsDock::fontChangeRequested, this, &MainWindow::setFont);
    connect(settingsDock, &SettingsDock::fontSizeChangeRequested, this, &MainWindow::setFontSize);
    connect(settingsDock, &SettingsDock::languageChangeRequested, this, &MainWindow::selectLanguage);
    connect(settingsDock, &SettingsDock::useSpellCheckerRequested, this, &MainWindow::setUseSpellChecker);
    connect(settingsDock, &SettingsDock::showWordcountRequested, this, &MainWindow::setShowWordcount);
    connect(settingsDock, &SettingsDock::showPagecountRequested, this, &MainWindow::setShowPagecount);
    connect(settingsDock, &SettingsDock::showReadtimeRequested, this, &MainWindow::setShowReadtime);
    connect(settingsDock, &SettingsDock::showDifficultyRequested, this, &MainWindow::setShowDifficulty);
    connect(settingsDock, &SettingsDock::useCharactersPerPage, this, &MainWindow::setUseCharactersPerPage);
    connect(settingsDock, &SettingsDock::wordsPerPageChangeRequested, this, &MainWindow::setWordsPerPage);
    connect(settingsDock, &SettingsDock::charactersPerPageChangeRequested, this, &MainWindow::setCharactersPerPage);
    connect(settingsDock, &SettingsDock::wordsPerMinuteChangeRequested, this, &MainWindow::setWordsPerMinute);
    connect(settingsDock, &SettingsDock::setEnableAutosaveRequested, this, &MainWindow::setEnableAutosave);
    connect(settingsDock, &SettingsDock::autosaveIntervalChangeRequested, this, &MainWindow::setAutosaveInterval);
    connect(settingsDock, &SettingsDock::settingsChangeRequested, &settings, &Settings::writeSettings);
    settingsDock->setVisible(false);

    if(currentFile != "") {
        open(currentFile);
    }
}


void MainWindow::retranslate() {
    openAction->setText(tr("Open"));
    saveAction->setText(tr("Save"));
    saveasAction->setText(tr("Save as"));
    findAction->setText(tr("Find"));
    undoAction->setText(tr("Undo"));
    redoAction->setText(tr("Redo"));
    statisticsLabel->statisticsChanged(false);
    optionsAction->setText(tr("Options"));

    findDock->retranslate();
    settingsDock->retranslate();
}


void MainWindow::closeEvent(QCloseEvent *event) {
    settings.writeSettings();

    event->accept();
}


void MainWindow::changeEvent(QEvent *event) {
    if(event->type() == QEvent::LanguageChange) {
        this->retranslate();
    }

    QMainWindow::changeEvent(event);
}


void MainWindow::loadSettings() {
    settings.readSettings();

    this->selectLanguage(settings.getLocale());

    this->setUseSpellChecker(settings.getUseSpellChecker());

    if(settings.getUseLightTheme()) {
        this->setLightTheme();
    } else {
        this->setDarkTheme();
    }

    this->setTextWidth(settings.getTextwidth());

    textEdit->limitTextWidth(settings.getLimitTextwidth());

    textEdit->setFont(settings.getFont());

    textEdit->setFontSize(settings.getFontsize());

    this->setAutosaveInterval(settings.getAutosaveInterval());

    this->setEnableAutosave(settings.getUseAutosave());

    statisticsLabel->statisticsChanged(false);
}


void MainWindow::open(const QString &filename) {
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot open file: ") + file.errorString());
        return;
    }

    this->setWindowTitle(QFileInfo(currentFile).fileName());
    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());

    file.close();

    this->currentFile = filename;
}


void MainWindow::open() {
    currentFile = QFileDialog::getOpenFileName(this, tr("Open"));

    open(currentFile);
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


void MainWindow::saveToDisk(const QString &filename) {
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


void MainWindow::selectLanguage(const QString locale) {
    settings.setLocale(locale);

    QApplication::removeTranslator(translator);
    translator->load(":/translations/Editor_" + settings.getLocale() + ".qm");
    QApplication::installTranslator(translator);

    delete spellchecker;
    spellchecker = new SpellChecker(this->getDictionary(settings.getLocale()), "");
    textEdit->setSpellChecker(spellchecker);
}


QString MainWindow::getDictionary(QString locale) {
    return temporaryDictDir.path() + "/" + locale.left(2);
}


void MainWindow::setDarkTheme() {
    QFile style(":/styles/dark_style.qss");
    style.open(QFile::ReadOnly);
    this->setStyleSheet(style.readAll());

    settings.setUseLightTheme(false);
}


void MainWindow::setLightTheme() {
    QFile style(":/styles/light_style.qss");
    style.open(QFile::ReadOnly);
    this->setStyleSheet(style.readAll());

    settings.setUseAutosave(true);
}


MainWindow::~MainWindow() {
    delete spellchecker;
}

