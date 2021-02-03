#include "mainwindow.h"

MainWindow::MainWindow(QString currentFile, QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(500, 400);
    this->setUnifiedTitleAndToolBarOnMac(true);
    this->grabGesture(Qt::PanGesture);

    this->currentFile = currentFile;

    textwidth = 80;
    limitTextwidth = true;

    autosaveInterval = 3;
    useAutosave = true;

    autosaveTimer = new QTimer(this);
    connect(autosaveTimer, &QTimer::timeout, this, &MainWindow::save);
    autosaveTimer->start(autosaveInterval * 60000);

    connect(QApplication::instance(), &QApplication::aboutToQuit, this, [=]() {
                                                                            if(textEdit->toPlainText().size() > 0) {
                                                                                this->save();
                                                                            }
                                                                        });

    wordsPerPage = 250;
    charactersPerPage = 1500;
    pagecount = 0;
    wordsPerMinute = 150;
    readtime = 0;
    difficulty = 100;

    showWordcount = true;
    showPagecount = false;
    pagecountFromCharacters = false;
    showReadtime = false;
    showDifficulty = false;

    // install translator
    translator = new QTranslator();
    locale = QLocale::system().name();

    // SpellChecker
    useSpellChecker = true;

    if (temporaryDictDir.isValid()) {
        QFile::copy(":/dictionaries/dictionaries/dictionaries/en/index.aff", temporaryDictDir.path() + "/en_index.aff");
        QFile::copy(":/dictionaries/dictionaries/dictionaries/en/index.dic", temporaryDictDir.path() + "/en_index.dic");
        QFile::copy(":/dictionaries/dictionaries/dictionaries/de/index.aff", temporaryDictDir.path() + "/de_index.aff");
        QFile::copy(":/dictionaries/dictionaries/dictionaries/de/index.dic", temporaryDictDir.path() + "/de_index.dic");
    }

    // have to request WRITE_EXTERNAL_STORAGE for Android
    //QString applicationDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    spellchecker = new SpellChecker(getDictionary(locale), "");

    // TextEdit
    textEdit = new TextEditor(this, spellchecker);
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

    // read settings
    font = textEdit->font();
    fontsize = font.pointSize();
    if(fontsize < 0) {
        fontsize = this->fontInfo().pointSize();
    }


    readSettings();

    // SettingsDock
    settingsDock = new SettingsDock(this, locale, useSpellChecker, textwidth, limitTextwidth, useAutosave, autosaveInterval,
                                    font, fontsize, wordsPerPage, charactersPerPage, wordsPerMinute, showWordcount,
                                    showPagecount, pagecountFromCharacters, showReadtime, showDifficulty);
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
    connect(settingsDock, &SettingsDock::settingsChangeRequested, this, &MainWindow::writeSettings);
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
    statisticsChanged(data);
    optionsAction->setText(tr("Options"));

    findDock->retranslate();
    settingsDock->retranslate();
}


void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();

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
    locale = settings.value("locale", locale).toString();
    this->selectLanguage(locale);

    useSpellChecker = settings.value("use_spellchecker", useSpellChecker).toBool();
    this->setUseSpellChecker(useSpellChecker);

    useLightTheme = settings.value("use_light_theme", useLightTheme).toBool();
    if(useLightTheme) {
        this->setLightTheme();
    } else {
        this->setDarkTheme();
    }

    textwidth = settings.value("textwidth", textwidth).toInt();
    this->setTextWidth(textwidth);

    limitTextwidth = settings.value("limit_textwidth", limitTextwidth).toBool();
    textEdit->limitTextWidth(limitTextwidth);

    QVariant fontvariant = settings.value("font", font);
    font = fontvariant.value<QFont>();
    textEdit->setFont(font);

    fontsize = settings.value("fontsize", fontsize).toInt();
    textEdit->setFontSize(fontsize);

    autosaveInterval = settings.value("autosave_interval", autosaveInterval).toInt();
    this->setAutosaveInterval(autosaveInterval);

    useAutosave = settings.value("use_autosave", useAutosave).toBool();
    this->setEnableAutosave(useAutosave);

    wordsPerPage = settings.value("words_per_page", wordsPerPage).toInt();
    charactersPerPage = settings.value("characters_per_page", charactersPerPage).toInt();
    wordsPerMinute = settings.value("characters_per_minute", wordsPerMinute).toInt();
    showWordcount = settings.value("show_wordcount", showWordcount).toBool();
    showPagecount = settings.value("show_pagecount", showPagecount).toBool();
    pagecountFromCharacters = settings.value("pagecount_from_characters", pagecountFromCharacters).toBool();
    showReadtime = settings.value("show_readtime", showReadtime).toBool();
    showDifficulty = settings.value("show_difficulty", showDifficulty).toBool();
    this->statisticsChanged(data);
}


void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("locale", locale);
    settings.setValue("use_spellchecker", useSpellChecker);
    settings.setValue("use_light_theme", useLightTheme);
    settings.setValue("limit_textwidth", limitTextwidth);
    settings.setValue("font", font);
    settings.setValue("fontsize", fontsize);
    settings.setValue("textwidth", textwidth);
    settings.setValue("autosave_interval", autosaveInterval);
    settings.setValue("use_autosave", useAutosave);
    settings.setValue("words_per_page", wordsPerPage);
    settings.setValue("characters_per_page", charactersPerPage);
    settings.setValue("words_per_minute", wordsPerMinute);
    settings.setValue("show_wordcount", showWordcount);
    settings.setValue("show_pagecount", showPagecount);
    settings.setValue("pagecount_from_characters", pagecountFromCharacters);
    settings.setValue("show_readtime", showReadtime);
    settings.setValue("show_difficulty", showDifficulty);
}


void MainWindow::open(QString &filename) {
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
    translator->load(":/translations/Editor_" + locale + ".qm");
    QApplication::installTranslator(translator);

    delete spellchecker;
    spellchecker = new SpellChecker(this->getDictionary(locale), "");
    textEdit->setSpellChecker(spellchecker);

    this->locale = locale;
}


QString MainWindow::getDictionary(QString locale) {
    return temporaryDictDir.path() + "/" + locale.left(2) + "_index";
}


void MainWindow::setDarkTheme() {
    QFile style(":/styles/dark_style.qss");
    style.open(QFile::ReadOnly);
    this->setStyleSheet(style.readAll());

    useLightTheme = false;
}


void MainWindow::setLightTheme() {
    QFile style(":/styles/light_style.qss");
    style.open(QFile::ReadOnly);
    this->setStyleSheet(style.readAll());

    useLightTheme = true;
}


void MainWindow::setAutosaveInterval(int autosaveInterval) {
    if(autosaveInterval < 1) {
        this->autosaveInterval = 1;
    } else {
        this->autosaveInterval = autosaveInterval;
    }

    autosaveTimer->start(autosaveInterval * 60000);
}


void MainWindow::statisticsChanged(const TextEditor::TextData data) {
    this->data = data;

    QString showntext = "";

    // wordcount
    if(showWordcount) {
        if(data.wordcount == 1) {
            showntext += tr("1 word");
        } else {
            showntext += QString::number(data.wordcount) + tr(" words");
        }
    }

    // pagecount
    if(pagecountFromCharacters) {
        pagecount = data.charactercount / charactersPerPage;
    } else {
        pagecount = data.wordcount / wordsPerPage;
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

    // readtime
    readtime = data.wordcount / wordsPerMinute;

    if(showReadtime) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        showntext += QString::number(readtime / 60) + tr("h ")
                     + QString::number(readtime % 60) + tr("m");
    }

    // difficulty
    if(locale.startsWith("de")) {
        difficulty = static_cast<int>(180 - data.avg_sentence_length - (58.5 * data.avg_word_length));
    } else {
        difficulty = static_cast<int>(206.835 - (1.015 * data.avg_sentence_length) - (84.6 * data.avg_word_length));
    }

    if(difficulty > 100) {
        difficulty = 100;
    }

    if(difficulty < 0) {
        difficulty = 0;
    }

    if(showDifficulty) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        showntext += tr(" level: ") + QString::number(difficulty);
    }

    statisticsLabel->setText(showntext);
}


MainWindow::~MainWindow() {
    delete spellchecker;
}

