#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setAttribute(Qt::WA_AcceptTouchEvents, true);
    this->setMinimumSize(300, 300);
    this->setUnifiedTitleAndToolBarOnMac(true);

    // TextEdit
    textedit = new QPlainTextEdit(this);
    textedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textedit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textedit->setStyleSheet("border-style: none;"
                            "background-color: #363636;"
                            "color: #FFFFFF;");

    this->setCentralWidget(textedit);

    // ToolBar
    toolbar = new QToolBar(this);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    toolbar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextOnly);
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setAllowedAreas(Qt::ToolBarArea::TopToolBarArea);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolbar->setStyleSheet("border-style: none;"
                           "background-color: #363636;"
                           "color: #FFFFFF;");

    QAction *open = new QAction(tr("Open"), this);
    open->setShortcut(QKeySequence::Open);
    connect(open, &QAction::triggered, this, &MainWindow::open);
    toolbar->addAction(open);

    QAction *save = new QAction(tr("Save"), this);
    save->setShortcut(QKeySequence::Save);
    connect(save, &QAction::triggered, this, &MainWindow::save);
    toolbar->addAction(save);

    QAction *saveas = new QAction(tr("Save as"), this);
    saveas->setShortcut(QKeySequence::SaveAs);
    connect(saveas, &QAction::triggered, this, &MainWindow::saveas);
    toolbar->addAction(saveas);

    QAction *find = new QAction(tr("Find"), this);
    find->setShortcut(QKeySequence::Find);
    connect(find, &QAction::triggered, this, &MainWindow::find);
    toolbar->addAction(find);

    statisticsLabel = new QLabel("*stats...*");
    statisticsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    statisticsLabel->setAlignment(Qt::AlignCenter);
    statisticsLabel->setTextFormat(Qt::TextFormat::MarkdownText);
    statisticsLabel->setStyleSheet("color: #C0C0C0");
    statisticsLabel->setContentsMargins(20, 5, 20, 5);
    toolbar->addWidget(statisticsLabel);

    QAction *options = new QAction(tr("Options"), this);
    connect(options, &QAction::triggered, this, &MainWindow::selectFont);
    toolbar->addAction(options);
    toolbar->addAction(options);

    this->addToolBar(toolbar);
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


void MainWindow::selectFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);

    if (fontSelected) {
        textedit->setFont(font);
    }
}


MainWindow::~MainWindow()
{
}

