#include "settingsdock.h"

SettingsDock::SettingsDock(QWidget *parent, QString locale, bool useSpellChecker, int textwidth, bool limitTextwidth, bool useAutosave, int autosaveInterval, QFont font, int fontsize, int wordsPerPage,
                           int charactersPerPage, int wordsPerMinute, bool showWordcount, bool showPagecount, bool pagecountFromCharacters, bool showReadtime, bool showDifficulty)
    : QDockWidget(parent)
{
    this->setAllowedAreas(Qt::RightDockWidgetArea);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    this->setFloating(false);
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);

    this->setTitleBarWidget(new QWidget());

    scrollArea = new QScrollArea(this);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(new QWidget());
    scrollArea->widget()->setMinimumSize(300, 600);
    this->setWidget(scrollArea);

    QGridLayout *gridLayout = new QGridLayout(scrollArea->widget());
    scrollArea->widget()->setLayout(gridLayout);
    gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
    gridLayout->setContentsMargins(20, 20, 20, 20);
    gridLayout->setVerticalSpacing(10);


    // select theme
    QHBoxLayout *themeHBox = new QHBoxLayout();
    themeHBox->setSpacing(20);
    gridLayout->addLayout(themeHBox, 0, 0, 1, 4);

    QPushButton *lightThemeButton = new QPushButton();
    lightThemeButton->setFlat(true);
    lightThemeButton->setStyleSheet("border-style: solid;"
                                    "border-width: 2px;"
                                    "border-color: #000000;"
                                    "background-color: #FFFFFF;");
    connect(lightThemeButton, &QPushButton::clicked,
            this, [=]() {emit lightThemeRequested(); emit settingsChangeRequested();});
    themeHBox->addWidget(lightThemeButton);

    QPushButton *darkThemeButton = new QPushButton();
    darkThemeButton->setFlat(true);
    darkThemeButton->setStyleSheet("border-style: solid;"
                                   "border-width: 2px;"
                                   "border-color: #FFFFFF;"
                                   "background-color: #202020;");
    connect(darkThemeButton, &QPushButton::clicked,
            this, [=]() {emit darkThemeRequested(); emit settingsChangeRequested();});
    themeHBox->addWidget(darkThemeButton);


    // select language
    QComboBox *languageSelector = new QComboBox();
    languageSelector->setEditable(false);
    languageSelector->addItem("English");
    languageSelector->addItem("Deutsch");

    if(locale == "de_DE") {
        languageSelector->setCurrentIndex(1);
    }

    connect(languageSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDock::newLanguageSelected);
    gridLayout->addWidget(languageSelector, 1, 0, 1, 4);


    // enable spellchecking
    useSpellCheckerCheck = new QCheckBox();
    useSpellCheckerCheck->setChecked(useSpellChecker);
    connect(useSpellCheckerCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit useSpellCheckerRequested(checked); emit settingsChangeRequested();});
    gridLayout->addWidget(useSpellCheckerCheck, 2, 0, 1, 4);


    // select font
    QFontComboBox *fontComboBox = new QFontComboBox();
    fontComboBox->setCurrentFont(font);
    fontComboBox->setFontFilters(QFontComboBox::ScalableFonts | QFontComboBox::MonospacedFonts | QFontComboBox::ProportionalFonts);
    fontComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    fontComboBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(fontComboBox, &QFontComboBox::currentFontChanged,
            this, [=](const QFont font) {emit fontChangeRequested(font); emit settingsChangeRequested();});
    gridLayout->addWidget(fontComboBox, 3, 0, 1, 3);

    QComboBox *fontSizeSelector = new QComboBox();
    fontSizeSelector->setEditable(false);
    QList<int> sizes = QFontDatabase::standardSizes();

    if(sizes.size() > 0 && fontsize < sizes[0]) {
        fontSizeSelector->addItem(QString::number(fontsize));
    }

    for(decltype (sizes.size()) i = 0; i < sizes.size(); ++i) {
        if(i > 0 && sizes[i - 1] < fontsize && sizes[i] > fontsize) {
            fontSizeSelector->addItem(QString::number(fontsize));
        }

        fontSizeSelector->addItem(QString::number(sizes[i]));
    }

    fontSizeSelector->setCurrentText(QString::number(fontsize));
    connect(fontSizeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index) {emit fontSizeChangeRequested(fontSizeSelector->itemText(index).toInt()); emit settingsChangeRequested();});
    gridLayout->addWidget(fontSizeSelector, 3, 3, 1, 1);


    // enabling limited textwidth
    enableTextwidthCheck = new QCheckBox();
    enableTextwidthCheck->setChecked(limitTextwidth);
    connect(enableTextwidthCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit setEnableFixedTextwidthRequested(checked); emit settingsChangeRequested();});
    gridLayout->addWidget(enableTextwidthCheck, 4, 0, 1, 4);


    // set text width
    textwidthLabel = new QLabel();
    textwidthLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    gridLayout->addWidget(textwidthLabel, 5, 0, 1, 3);

    QLineEdit *textwidthEdit = new QLineEdit();
    textwidthEdit->setMaxLength(4);
    textwidthEdit->setMaximumWidth(60);
    textwidthEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    textwidthEdit->setText(QString::number(textwidth));
    textwidthEdit->setAlignment(Qt::AlignCenter);
    textwidthEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{1,3}")));
    connect(textwidthEdit, &QLineEdit::inputRejected, textwidthEdit, [=]() {textwidthEdit->setText(QString::number(textwidth));});
    connect(textwidthEdit, &QLineEdit::editingFinished,
            this, [=]() {emit textwidthChangeRequested(textwidthEdit->text().toInt()); emit settingsChangeRequested();});
    gridLayout->addWidget(textwidthEdit, 5, 3, 1, 1);


    // select displayed statistics
    wordcountCheck = new QCheckBox();
    wordcountCheck->setChecked(showWordcount);
    connect(wordcountCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showWordcountRequested(checked); emit settingsChangeRequested();});
    gridLayout->addWidget(wordcountCheck, 6, 0, 1, 4);


    // page statistics
    pagecountCheck = new QCheckBox();
    pagecountCheck->setChecked(showPagecount);
    connect(pagecountCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showPagecountRequested(checked); emit settingsChangeRequested();});
    gridLayout->addWidget(pagecountCheck, 7, 0, 1, 4);

    wordsPerPageLabel = new QLabel();
    wordsPerPageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    gridLayout->addWidget(wordsPerPageLabel, 9, 0, 1, 3);

    QLineEdit *wordsPerPageEdit = new QLineEdit();
    wordsPerPageEdit->setMaxLength(4);
    wordsPerPageEdit->setMaximumWidth(60);
    wordsPerPageEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    wordsPerPageEdit->setText(QString::number(wordsPerPage));
    wordsPerPageEdit->setEnabled(!pagecountFromCharacters);
    wordsPerPageEdit->setAlignment(Qt::AlignCenter);
    wordsPerPageEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{0,5}")));
    connect(wordsPerPageEdit, &QLineEdit::inputRejected, wordsPerPageEdit, [=]() {wordsPerPageEdit->setText(QString::number(wordsPerPage));});
    connect(wordsPerPageEdit, &QLineEdit::editingFinished,
            this, [=]() {emit wordsPerPageChangeRequested(wordsPerPageEdit->text().toInt()); emit settingsChangeRequested();});
    gridLayout->addWidget(wordsPerPageEdit, 9, 3, 1, 1);


    charactersPerPageLabel = new QLabel();
    charactersPerPageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    gridLayout->addWidget(charactersPerPageLabel, 10, 0, 1, 3);

    QLineEdit *charactersPerPageEdit = new QLineEdit();
    charactersPerPageEdit->setMaxLength(4);
    charactersPerPageEdit->setMaximumWidth(60);
    charactersPerPageEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    charactersPerPageEdit->setText(QString::number(charactersPerPage));
    charactersPerPageEdit->setEnabled(pagecountFromCharacters);
    charactersPerPageEdit->setAlignment(Qt::AlignCenter);
    charactersPerPageEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{0,6}")));
    connect(charactersPerPageEdit, &QLineEdit::inputRejected, charactersPerPageEdit, [=]() {charactersPerPageEdit->setText(QString::number(charactersPerPage));});
    connect(charactersPerPageEdit, &QLineEdit::editingFinished,
            this, [=]() {emit charactersPerPageChangeRequested(charactersPerPageEdit->text().toInt()); emit settingsChangeRequested();});
    gridLayout->addWidget(charactersPerPageEdit, 10, 3, 1, 1);


    useCharactersPerPageCheck = new QCheckBox();
    useCharactersPerPageCheck->setChecked(pagecountFromCharacters);
    connect(useCharactersPerPageCheck, &QCheckBox::clicked,
            this, [=](bool checked) {
                                        wordsPerPageEdit->setEnabled(!checked);
                                        charactersPerPageEdit->setEnabled(checked);
                                        emit useCharactersPerPage(checked);
                                        emit settingsChangeRequested();
                                    });
    gridLayout->addWidget(useCharactersPerPageCheck, 8, 0, 1, 4);


    // readtime statistics
    readtimeCheck = new QCheckBox();
    readtimeCheck->setChecked(showReadtime);
    connect(readtimeCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showReadtimeRequested(checked); emit settingsChangeRequested();});
    gridLayout->addWidget(readtimeCheck, 11, 0, 1, 4);

    wordsPerMinuteLabel = new QLabel();
    wordsPerMinuteLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    gridLayout->addWidget(wordsPerMinuteLabel, 12, 0, 1, 3);

    QLineEdit *wordsPerMinuteEdit = new QLineEdit();
    wordsPerMinuteEdit->setMaxLength(4);
    wordsPerMinuteEdit->setMaximumWidth(60);
    wordsPerMinuteEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    wordsPerMinuteEdit->setText(QString::number(wordsPerMinute));
    wordsPerMinuteEdit->setAlignment(Qt::AlignCenter);
    wordsPerMinuteEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{0,3}")));
    connect(wordsPerMinuteEdit, &QLineEdit::inputRejected, wordsPerMinuteEdit, [=]() {wordsPerMinuteEdit->setText(QString::number(wordsPerMinute));});
    connect(wordsPerMinuteEdit, &QLineEdit::editingFinished,
            this, [=]() {emit wordsPerMinuteChangeRequested(wordsPerMinuteEdit->text().toInt()); emit settingsChangeRequested();});
    gridLayout->addWidget(wordsPerMinuteEdit, 12, 3, 1, 1);


    // difficulty statistics
    difficultyCheck = new QCheckBox();
    difficultyCheck->setChecked(showDifficulty);
    connect(difficultyCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showDifficultyRequested(checked); emit settingsChangeRequested();});
    gridLayout->addWidget(difficultyCheck, 13, 0, 1, 4);


    // enable auto-save
    enableAutosaveCheck = new QCheckBox();
    enableAutosaveCheck->setChecked(useAutosave);
    connect(enableAutosaveCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit setEnableAutosaveRequested(checked); emit settingsChangeRequested();});
    gridLayout->addWidget(enableAutosaveCheck, 14, 0, 1, 4);


    // auto-save interval
    autosaveIntervalLabel = new QLabel();
    autosaveIntervalLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    gridLayout->addWidget(autosaveIntervalLabel, 15, 0, 1, 3);

    QLineEdit *autosaveEdit = new QLineEdit();
    autosaveEdit->setMaxLength(4);
    autosaveEdit->setMaximumWidth(60);
    autosaveEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    autosaveEdit->setText(QString::number(autosaveInterval));
    autosaveEdit->setAlignment(Qt::AlignCenter);
    autosaveEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{0,3}")));
    connect(autosaveEdit, &QLineEdit::inputRejected, autosaveEdit, [=]() {autosaveEdit->setText(QString::number(autosaveInterval));});
    connect(autosaveEdit, &QLineEdit::editingFinished,
            this, [=]() {emit autosaveIntervalChangeRequested(autosaveEdit->text().toInt()); emit settingsChangeRequested();});
    gridLayout->addWidget(autosaveEdit, 15, 3, 1, 1);

    gridLayout->setRowStretch(20, 100);

    retranslate();
}


void SettingsDock::retranslate() {
    useSpellCheckerCheck->setText(tr("use spellchecker"));
    enableTextwidthCheck->setText(tr("limit editor width"));
    textwidthLabel->setText(tr("characters per line:"));
    wordcountCheck->setText(tr("show word count"));
    pagecountCheck->setText(tr("show page count"));
    readtimeCheck->setText(tr("show read time"));
    difficultyCheck->setText(tr("show Flesch-level"));
    useCharactersPerPageCheck->setText(tr("page count from characters"));
    wordsPerPageLabel->setText(tr("words per page:"));
    charactersPerPageLabel->setText(tr("characters per page:"));
    wordsPerMinuteLabel->setText(tr("words per minute:"));
    enableAutosaveCheck->setText(tr("enable auto-save"));
    autosaveIntervalLabel->setText(tr("auto-save interval (min):"));
}


void SettingsDock::newLanguageSelected(int index) {
    switch(index) {
    case 1:
        emit languageChangeRequested("de_DE");
        emit settingsChangeRequested();
        break;
    default:
        emit languageChangeRequested("en_EN");
        emit settingsChangeRequested();
    }
}
