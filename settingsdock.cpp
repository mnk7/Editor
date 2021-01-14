#include "settingsdock.h"

SettingsDock::SettingsDock(QWidget *parent, QString locale)
    : QDockWidget(parent)
{
    this->setAllowedAreas(Qt::RightDockWidgetArea);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    this->setFloating(false);
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);

    this->setTitleBarWidget(new QWidget());

    this->setWidget(new QWidget());
    this->widget()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    QGridLayout *gridLayout = new QGridLayout(this->widget());
    this->widget()->setLayout(gridLayout);
    gridLayout->setContentsMargins(10, 10, 10, 10);
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
            this, [=]() {emit lightThemeRequested();});
    themeHBox->addWidget(lightThemeButton);

    QPushButton *darkThemeButton = new QPushButton();
    darkThemeButton->setFlat(true);
    darkThemeButton->setStyleSheet("border-style: solid;"
                                   "border-width: 2px;"
                                   "border-color: #FFFFFF;"
                                   "background-color: #202020;");
    connect(darkThemeButton, &QPushButton::clicked,
            this, [=]() {emit darkThemeRequested();});
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


    // select font
    QFontComboBox *fontComboBox = new QFontComboBox();
    fontComboBox->setFontFilters(QFontComboBox::ScalableFonts | QFontComboBox::MonospacedFonts | QFontComboBox::ProportionalFonts);
    fontComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    fontComboBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(fontComboBox, &QFontComboBox::currentFontChanged,
            this, [=](const QFont font) {emit fontChangeRequested(font);});
    gridLayout->addWidget(fontComboBox, 2, 0, 1, 3);

    QComboBox *fontSizeSelector = new QComboBox();
    fontSizeSelector->setEditable(false);
    int currentSize = this->fontInfo().pointSize();
    QList<int> sizes = QFontDatabase::standardSizes();

    if(sizes.size() > 0 && currentSize < sizes[0]) {
        fontSizeSelector->addItem(QString::number(currentSize));
    }

    for(decltype (sizes.size()) i = 0; i < sizes.size(); ++i) {
        if(i > 0 && sizes[i - 1] < currentSize && sizes[i] > currentSize) {
            fontSizeSelector->addItem(QString::number(currentSize));
        }

        fontSizeSelector->addItem(QString::number(sizes[i]));
    }

    fontSizeSelector->setCurrentText(QString::number(currentSize));
    connect(fontSizeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index) {emit fontSizeChangeRequested(fontSizeSelector->itemText(index).toInt());});
    gridLayout->addWidget(fontSizeSelector, 2, 3, 1, 1);


    // enabling limited textwidth
    enableTextwidthCheck = new QCheckBox();
    enableTextwidthCheck->setChecked(true);
    connect(enableTextwidthCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit setEnableFixedTextwidthRequested(checked);});
    gridLayout->addWidget(enableTextwidthCheck, 3, 0, 1, 4);


    // set text width
    textwidthLabel = new QLabel();
    textwidthLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    gridLayout->addWidget(textwidthLabel, 4, 0, 1, 3);

    QLineEdit *textwidthEdit = new QLineEdit();
    textwidthEdit->setMaxLength(4);
    textwidthEdit->setMaximumWidth(60);
    textwidthEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    textwidthEdit->setText("80");
    textwidthEdit->setAlignment(Qt::AlignCenter);
    textwidthEdit->setValidator(new QIntValidator(0, 1000));
    connect(textwidthEdit, &QLineEdit::textChanged,
            this, [=]() {emit textwidthChangeRequested(textwidthEdit->text().toInt());});
    gridLayout->addWidget(textwidthEdit, 4, 3, 1, 1);


    // select displayed statistics
    wordcountCheck = new QCheckBox();
    wordcountCheck->setChecked(true);
    connect(wordcountCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showWordcountRequested(checked);});
    gridLayout->addWidget(wordcountCheck, 5, 0, 1, 4);

    pagecountCheck = new QCheckBox();
    pagecountCheck->setChecked(false);
    connect(pagecountCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showPagecountRequested(checked);});
    gridLayout->addWidget(pagecountCheck, 6, 0, 1, 4);

    readtimeCheck = new QCheckBox();
    readtimeCheck->setChecked(false);
    connect(readtimeCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showReadtimeRequested(checked);});
    gridLayout->addWidget(readtimeCheck, 7, 0, 1, 4);

    difficultyCheck = new QCheckBox();
    difficultyCheck->setChecked(false);
    connect(difficultyCheck, &QCheckBox::clicked,
            this, [=](bool checked) {emit showDifficultyRequested(checked);});
    gridLayout->addWidget(difficultyCheck, 8, 0, 1, 4);

    gridLayout->setRowStretch(20, 100);

    retranslate();
}


void SettingsDock::retranslate() {
    enableTextwidthCheck->setText(tr("limit editor width"));
    textwidthLabel->setText(tr("characters per line:"));
    wordcountCheck->setText(tr("show word count"));
    pagecountCheck->setText(tr("show page count"));
    readtimeCheck->setText(tr("show read time"));
    difficultyCheck->setText(tr("show Flesch-level"));
}


void SettingsDock::newLanguageSelected(int index) {
    switch(index) {
    case 1:
        emit languageChangeRequested("de_DE");
        break;
    default:
        emit languageChangeRequested("en_EN");
    }
}
