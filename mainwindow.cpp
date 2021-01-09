#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->setMinimumSize(450, 300);
    this->setUnifiedTitleAndToolBarOnMac(true);

    textwidth = 80;

    // install translator
    translator = new QTranslator();
    locale = QLocale::system().name();
    translator->load("Editor_" + locale + ".qm");
    QApplication::installTranslator(translator);

    // TextEdit
    textedit = new TextEditor(this);
    this->setCentralWidget(textedit);

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
    connect(findAction, &QAction::triggered, this, &MainWindow::find);
    toolbar->addAction(findAction);

    statisticsLabel = new QLabel("*stats...*");
    statisticsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    statisticsLabel->setAlignment(Qt::AlignRight);
    statisticsLabel->setTextFormat(Qt::TextFormat::MarkdownText);
    statisticsLabel->setContentsMargins(20, 5, 20, 5);
    toolbar->addWidget(statisticsLabel);

    optionsAction = new QAction(this);
    connect(optionsAction, &QAction::triggered, this, &MainWindow::selectFont);
    toolbar->addAction(optionsAction);

    this->addToolBar(toolbar);

    this->retranslate();
    this->setDarkTheme();
}


void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();
    event->accept();
}


bool MainWindow::event(QEvent *event) {
    switch(event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd: {
        QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
        int numberTouchPoints = touchEvent->touchPoints().count();

        if(numberTouchPoints == 3) {
            textedit->undo();
        } else if(numberTouchPoints == 4) {
            textedit->redo();
        } else {
            return QMainWindow::event(event);
        }

        statisticsLabel->setText(QString(numberTouchPoints));
        return true;
    }
    default:
        return QMainWindow::event(event);
    }
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
    textedit->setPlainText(in.readAll());

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
    QString text = textedit->toPlainText();
    out << text;
    file.flush();
    file.close();
}


void MainWindow::find() {
}


void MainWindow::selectLanguage(QString locale) {
    QApplication::removeTranslator(translator);
    translator->load("Editor_" + locale + ".qm");
    QApplication::installTranslator(translator);

    this->locale = locale;
}


void MainWindow::retranslate() {
    openAction->setText(tr("Open"));
    saveAction->setText(tr("Save"));
    saveasAction->setText(tr("Save as"));
    findAction->setText(tr("Find"));
    optionsAction->setText(tr("Options"));
}


void MainWindow::selectFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, textedit->font(), this, tr("Select Font"));

    if (fontSelected) {
        textedit->setFont(font);
    }


    if(locale.endsWith("de_DE")) {
        selectLanguage("en_EN");
        qDebug("Englisch");
    } else {
        selectLanguage("de_DE");
    }
}


void MainWindow::setDarkTheme() {
    this->setStyleSheet(
        "QWidget { \
            background-color: #363636; \
            color: #FFFFFF; \
        } \
        QPlainTextEdit { \
            border-style: none; \
            selection-color: #000000; \
            selection-background-color: #CCB026; \
        } \
        QLabel { \
            border-style: none; \
            color: #C0C0C0 \
        } \
        QToolButton:hover { \
            background-color: #363636; \
            border-style: none; \
        } \
        QToolBar { \
            border-style: none; \
        } \
        QScrollBar:vertical { \
            width: 8px; \
            border-style: none; \
        } \
        QScrollBar:handle:vertical { \
            background-color: #C0C0C0; \
            border-radius: 4px; \
        } \
        QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { \
            background: none; \
        } \
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { \
            background: none; \
        } \
        QScrollBar::add-line { \
            border: none; \
            background: none; \
        } \
        QScrollBar::sub-line { \
            border: none; \
            background: none; \
        }"
    );
}


void MainWindow::setLightTheme() {
    this->setStyleSheet(
        "QWidget { \
            background-color: #FFFFFF; \
            color: #000000; \
        } \
        QPlainTextEdit { \
            border-style: none; \
            selection-color: #FFFFFF; \
            selection-background-color: #9226CC; \
        } \
        QLabel { \
            color: #A0A0A0 \
        } \
        QToolButton:hover { \
            background-color: #FFFFFF; \
            border-style: none; \
        } \
        QToolBar { \
            border-style: none; \
        } \
        QScrollBar:vertical { \
            width: 8px; \
            border-style: none; \
        } \
        QScrollBar:handle:vertical { \
            background-color: #A0A0A0; \
            border-radius: 4px; \
        } \
        QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { \
            background: none; \
        } \
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { \
            background: none; \
        } \
        QScrollBar::add-line { \
            border: none; \
            background: none; \
        } \
        QScrollBar::sub-line { \
            border: none; \
            background: none; \
        }"
    );
}


MainWindow::~MainWindow()
{
}

