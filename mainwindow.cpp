#include "mainwindow.h"

MainWindow::MainWindow(QString currentFile, QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(500, 400);
    this->setUnifiedTitleAndToolBarOnMac(true);
    //this->grabGesture(Qt::PanGesture);

    this->currentFile = currentFile;

    autosaveTimer = new QTimer(this);
    connect(autosaveTimer, &QTimer::timeout, this, &MainWindow::save);
    autosaveTimer->start(settings.getAutosaveInterval() * 60000);

    connect(QApplication::instance(), &QApplication::aboutToQuit, this, [=]() {
                                                                            if(textEdit->toPlainText().size() > 0) {
                                                                                this->save();
                                                                            }
                                                                        });

    settings.addSupportedLanguage("Deutsch", "de_DE");

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

    // the central widget that displays either the textEdit or the textRender
    stackedCentralWidget = new QStackedWidget(this);
    textEdit = new TextEditor(stackedCentralWidget, &statistics, spellchecker);
    stackedCentralWidget->addWidget(textEdit);
    textRender = new TextRenderer(stackedCentralWidget);
    stackedCentralWidget->addWidget(textRender);
    stackedCentralWidget->setCurrentWidget(textEdit);
    this->setCentralWidget(stackedCentralWidget);

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
            this, [=]() {findDock->changeVisibility(!findDock->isVisible());});
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
            this, [=]() {settingsDock->changeVisibility(!settingsDock->isVisible());});
    toolbar->addAction(optionsAction);

    this->addToolBar(toolbar);

    // read settings
    settings.setFont(textEdit->font());
    settings.setFontsize(settings.getFont().pointSize());

    loadSettings();

    // SettingsDock
    settingsDock = new SettingsDock(this, &settings);
    this->addDockWidget(Qt::RightDockWidgetArea, settingsDock);
    connect(settingsDock, &SettingsDock::visibilityChanged, this, [=](bool visible) {if(!visible) {textEdit->grabKeyboard();}});
    connect(settingsDock, &SettingsDock::languageChangeRequested, this, &MainWindow::selectLanguage);
    connect(settingsDock, &SettingsDock::lightThemeRequested, this, &MainWindow::setLightTheme);
    connect(settingsDock, &SettingsDock::darkThemeRequested, this, &MainWindow::setDarkTheme);
    connect(settingsDock, &SettingsDock::renderTextRequested,
            this, [=] (const bool render) {
                       if(render) {
                           textRender->setMarkdown(textEdit->toPlainText());
                           stackedCentralWidget->setCurrentWidget(textRender);
                       } else {
                           stackedCentralWidget->setCurrentWidget(textEdit);
                   }});
    connect(settingsDock, &SettingsDock::setEnableFixedTextwidthRequested,
            this, [=] (const bool limitTextwidth) {
                       settings.setLimitTextwidth(limitTextwidth);
                       textEdit->limitTextWidth(settings.getLimitTextwidth());
                       textRender->limitTextWidth(settings.getLimitTextwidth());
                   });
    connect(settingsDock, &SettingsDock::textwidthChangeRequested,
            this, [=] (const int textwidth) {
                       settings.setTextwidth(textEdit->setTextWidth(textwidth));
                       textRender->setTextWidth(settings.getTextwidth());
                   });
    connect(settingsDock, &SettingsDock::fontChangeRequested,
            this, [=] (const QFont font) {
                       settings.setFont(font);
                       textEdit->setFont(settings.getFont());
                       textRender->setFont(settings.getFont());
                   });
    connect(settingsDock, &SettingsDock::fontSizeChangeRequested,
            this, [=] (const int fontsize) {
                       settings.setFontsize(fontsize);
                       textEdit->setFontSize(settings.getFontsize());
                       textRender->setFontSize(settings.getFontsize());
                   });
    connect(settingsDock, &SettingsDock::useSpellCheckerRequested,
            this, [=] (const bool useSpellChecker) {settings.setUseSpellChecker(useSpellChecker); textEdit->setUseSpellChecker(settings.getUseSpellChecker());});
    connect(settingsDock, &SettingsDock::showWordcountRequested,
            this, [=] (const bool showWordcount) {settings.setShowWordcount(showWordcount); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::showPagecountRequested,
            this, [=] (const bool showPagecount) {settings.setShowPagecount(showPagecount); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::showReadtimeRequested,
            this, [=] (const bool showReadtime) {settings.setShowReadtime(showReadtime); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::showDifficultyRequested,
            this, [=] (const bool showDifficulty) {settings.setShowDifficulty(showDifficulty); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::useCharactersPerPage,
            this, [=] (const bool pagecountFromCharacters) {settings.setPagecountFromCharacters(pagecountFromCharacters); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::wordsPerPageChangeRequested,
            this, [=] (const int wordsPerPage) {settings.setWordsPerPage(wordsPerPage); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::charactersPerPageChangeRequested,
            this, [=] (const int charactersPerPage) {settings.setCharactersPerPage(charactersPerPage); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::wordsPerMinuteChangeRequested,
            this, [=] (const int wordsPerMinute) {settings.setWordsPerMinute(wordsPerMinute); statisticsLabel->statisticsChanged(false);});
    connect(settingsDock, &SettingsDock::setEnableAutosaveRequested,
            this, [=] (const bool useAutosave) {settings.setUseAutosave(useAutosave); useAutosave ? autosaveTimer->start() : autosaveTimer->stop();});
    connect(settingsDock, &SettingsDock::autosaveIntervalChangeRequested,
            this, [=] (const int autosaveInterval) {settings.setAutosaveInterval(autosaveInterval); autosaveTimer->start(settings.getAutosaveInterval() * 60000);});
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

    this->textEdit->setUseSpellChecker(settings.getUseSpellChecker());

    if(settings.getUseLightTheme()) {
        this->setLightTheme();
    } else {
        this->setDarkTheme();
    }

    settings.setTextwidth(textEdit->setTextWidth(settings.getTextwidth()));
    textRender->setTextWidth(settings.getTextwidth());

    textEdit->limitTextWidth(settings.getLimitTextwidth());
    textRender->limitTextWidth(settings.getLimitTextwidth());

    textEdit->setFont(settings.getFont());
    textRender->setFont(settings.getFont());

    textEdit->setFontSize(settings.getFontsize());
    textRender->setFontSize(settings.getFontsize());

    if(settings.getUseAutosave()) {
        autosaveTimer->start(settings.getAutosaveInterval() * 60000);
    } else {
        autosaveTimer->stop();
    }

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
    stackedCentralWidget->setCurrentWidget(textEdit);

    file.close();

    this->currentFile = filename;
}


void MainWindow::open() {
    currentFile = QFileDialog::getOpenFileName(this, tr("Open"), "", "Markdown files (*.md *.mkd *.MD *.MKD)");

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

