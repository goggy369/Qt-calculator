#include "calculator.h"
#include "button.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QShortcut>
#include <QStandardItemModel>

QString squareLabel = "x\u00b2";
QString squareRootLabel = "\u221Ax";
QString reciprocalLabel = "\u00b9/x";
QString absoluteValueLabel = "|x|";
QString factorialLabel = "n!";
QString _1exLabel = "10^x";
QString logarithmLabel = "log";
QString naturalLogarithmLabel = "ln";
QString moduloLabel = "mod";
QString divisionLabel = "\303\267";
QString timesLabel = "\303\227";
QString minusLabel = "\u2212";
QString plusLabel = "+";

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), memory(0.0), sumSoFar(0.0), factorSoFar(0.0),
    waitingForOperand(true), last("equal")
{
    displayMaxLength = 21;

    displayIn = new QLineEdit("0");
    displayIn->setReadOnly(true);
    displayIn->setAlignment(Qt::AlignRight);
    displayIn->setStyleSheet("QLineEdit { background-color: transparent; }");
    displayIn->setFrame(false);
    displayIn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    displayOut = new QLineEdit("");
    displayOut->setReadOnly(true);
    displayOut->setAlignment(Qt::AlignRight);
    displayOut->setStyleSheet("QLineEdit { "
                              "  background-color: transparent;"
                              "  color: #616161;"
                              "  selection-background-color: transparent;"
                              "  selection-color: #616161;"
                              "}");
    displayOut->setFrame(false);
    displayOut->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont font = displayIn->font();
    font.setFamily("comic sans ms");
    font.setPointSize(font.pointSize() + 2);
    displayOut->setFont(font);
    font.setPointSize(font.pointSize() + 8);
    displayIn->setFont(font);

    clearMemoryBtn = createButton(tr("MC"), &Calculator::clearMemory);
    readMemoryBtn = createButton(tr("MR"), &Calculator::readMemory);
    addToMemoryBtn = createButton(tr("M+"), &Calculator::addToMemory);
    substractFromMemoryBtn = createButton(tr("M-"), &Calculator::substractFromMemory);
    // setMemoryBtn = createButton(tr("MS"), &Calculator::setMemory);
    // showMemoryBtn = createButton(tr("M"), &Calculator::showMemory);

    piBtn = createButton("π", &Calculator::constantClicked);
    eBtn = createButton("e", &Calculator::constantClicked);

    backspaceBtn = createButton("\u232B", &Calculator::backspaceClicked);
    clearBtn = createButton(tr("CE"), &Calculator::clear);
    clearAllBtn = createButton(tr("C"), &Calculator::clearAll);

    sinBtn = createButton("sin", &Calculator::trigonometricClicked);
    cosBtn = createButton("cos", &Calculator::trigonometricClicked);
    tanBtn = createButton("tan", &Calculator::trigonometricClicked);
    arcsinBtn = createButton("arcsin", &Calculator::trigonometricClicked);
    arccosBtn = createButton("arccos", &Calculator::trigonometricClicked);
    arctanBtn = createButton("arctan", &Calculator::trigonometricClicked);
    sinhBtn = createButton("sinh", &Calculator::trigonometricClicked);
    coshBtn = createButton("cosh", &Calculator::trigonometricClicked);
    tanhBtn = createButton("tanh", &Calculator::trigonometricClicked);
    asinhBtn = createButton("asinh", &Calculator::trigonometricClicked);
    acoshBtn = createButton("acosh", &Calculator::trigonometricClicked);
    atanhBtn = createButton("atanh", &Calculator::trigonometricClicked);

    squareBtn = createButton(squareLabel, &Calculator::unaryOperatorClicked);
    reciprocalBtn = createButton(reciprocalLabel, &Calculator::unaryOperatorClicked);
    absoluteValueBtn = createButton(absoluteValueLabel, &Calculator::unaryOperatorClicked);
    factorialBtn = createButton(factorialLabel, &Calculator::unaryOperatorClicked);
    squareRootBtn = createButton(squareRootLabel, &Calculator::unaryOperatorClicked);
    _1exBtn = createButton(_1exLabel, &Calculator::unaryOperatorClicked);
    logarithmBtn = createButton(logarithmLabel, &Calculator::unaryOperatorClicked);
    naturalLogarithmBtn = createButton(naturalLogarithmLabel, &Calculator::unaryOperatorClicked);

    moduloBtn = createButton(moduloLabel, &Calculator::multiplicativeOperatorClicked);
    divisionBtn = createButton(divisionLabel, &Calculator::multiplicativeOperatorClicked);
    timesBtn = createButton(timesLabel, &Calculator::multiplicativeOperatorClicked);
    minusBtn = createButton(minusLabel, &Calculator::additiveOperatorClicked);
    plusBtn = createButton(plusLabel, &Calculator::additiveOperatorClicked);
    equalBtn = createButton("=", &Calculator::equalClicked);
    equalBtn->setStyleSheet(
        "QToolButton:hover {"               //滑过
        "  color: white;"
        "  background: #196EBD;"
        "}"
        "QToolButton:pressed {"                  //点击
        // "  color: #7A7A7A;"
        "  background: #307DC6;"
        "  color: #C1D8EE;"
        "}"
        "QToolButton {"
        "  border: 1px solid #E5E5E5;"
        "  border-radius: 4px;"
        "  margin: 1px;"
        "  font: normal normal 16px comic sans ms;"
        "  background: #005FBA;"
        "  color: white;"
        "}"
        );

    for (int i = 0; i < NumDigitButtons; ++i) {
        digitBtns[i] = createButton(QString::number(i), &Calculator::digitClicked);
        digitBtns[i]->setStyleSheet(
            "QToolButton:hover {"           //滑过
            "  background: #FCFCFC;"
            "}"
            "QToolButton:pressed {"         //点击
            "  background: #F9F9F9;"
            "  color: #5F5F5F;"
            "}"
            "QToolButton {"
            "  border: 1px solid #E5E5E5;"
            "  border-radius: 4px;"
            "  margin: 1px;"
            "  font: normal normal 16px comic sans ms;"
            "  background: white;"
            "}"
            );
    }

    pointBtn = createButton(".", &Calculator::pointClicked);
    changeSignBtn = createButton("\u207A/\u208B", &Calculator::changeSignClicked);
    pointBtn->setStyleSheet(
        "QToolButton:hover {"               //滑过
        "  background: #FCFCFC;"
        "}"
        "QToolButton:pressed {"             //点击
        "  background: #F9F9F9;"
        "  color: #5F5F5F;"
        "}"
        "QToolButton {"
        "  border: 1px solid #E5E5E5;"
        "  border-radius: 4px;"
        "  margin: 1px;"
        "  font: normal normal 16px comic sans ms;"
        "  background: white;"
        "}"
        );
    changeSignBtn->setStyleSheet(
        "QToolButton:hover {"               //滑过
        "  background: #FCFCFC;"
        "}"
        "QToolButton:pressed {"             //点击
        "  background: #F9F9F9;"
        "  color: #5F5F5F;"
        "}"
        "QToolButton {"
        "  border: 1px solid #E5E5E5;"
        "  border-radius: 4px;"
        "  margin: 1px;"
        "  font: normal normal 16px comic sans ms;"
        "  background: white;"
        "}"
        );

    unitBtn = createButton("DEG", &Calculator::unitClicked);

    TrigonometricComboBox = new QComboBox(this);

    QStringList items;
    items << "Basic" << "Arc" << "Hyp" << "A H";
    QStandardItemModel *model = new QStandardItemModel();
    for (const QString &itemText : items) {
        QStandardItem *item = new QStandardItem(itemText);
        item->setTextAlignment(Qt::AlignCenter);
        model->appendRow(item);
    }

    TrigonometricComboBox->setModel(model);
    TrigonometricComboBox->setCurrentIndex(0);

    stackedWidget_1 = new QStackedWidget(this);
    stackedWidget_2 = new QStackedWidget(this);
    stackedWidget_3 = new QStackedWidget(this);

    stackedWidget_1->addWidget(sinBtn);
    stackedWidget_1->addWidget(arcsinBtn);
    stackedWidget_1->addWidget(sinhBtn);
    stackedWidget_1->addWidget(asinhBtn);
    stackedWidget_1->setCurrentIndex(0);

    stackedWidget_2->addWidget(cosBtn);
    stackedWidget_2->addWidget(arccosBtn);
    stackedWidget_2->addWidget(coshBtn);
    stackedWidget_2->addWidget(acoshBtn);
    stackedWidget_2->setCurrentIndex(0);

    stackedWidget_3->addWidget(tanBtn);
    stackedWidget_3->addWidget(arctanBtn);
    stackedWidget_3->addWidget(tanhBtn);
    stackedWidget_3->addWidget(atanhBtn);
    stackedWidget_3->setCurrentIndex(0);

    TrigonometricComboBox->setMinimumSize(54, 30);
    TrigonometricComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TrigonometricComboBox->setStyleSheet(
        "QComboBox:drop-down {"                 //下拉箭头框
        "  width: 0;"
        "  border: none;"
        "}"

        "QComboBox QAbstractItemView {"         //下拉框整体
        "  border: none;" // 去除边框
        "  border-radius: 3px;"
        "  margin: 2px;"
        "  background: #B3CFEA;"
        "}"

        "QComboBox QAbstractItemView:item {"    //下拉框中各项
        /* 项的高度（设置
         * pComboBox->setView(new QListView());
         * 后，该项才起作用） */
        "  height: 25px;"
        "}"

        "QComboBox:hover {"                     //滑过
        "  background: #196EBD;"
        "}"

        "QComboBox:on {"                        //点击
        "  background: #307DC6;"
        "  color: #C1D8EE;"
        "}"

        "QComboBox {"
        "  border: none;"
        "  border-radius: 13px;"
        "  margin: 2px;"
        "  padding: 0px 2px;"
        "  background: #005FBA;"
        "  color: white;"
        "  font: normal bold 16px comic sans ms;"
        "}"
        );


    // 连接下拉菜单的信号到槽函数，以便在选项变化时更新按钮的显示
    connect(TrigonometricComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        // 根据下拉菜单的当前索引显示对应的按钮
        switch (index) {
        case 0:
            stackedWidget_1->setCurrentIndex(0);
            stackedWidget_2->setCurrentIndex(0);
            stackedWidget_3->setCurrentIndex(0);
            break;
        case 1:
            stackedWidget_1->setCurrentIndex(1);
            stackedWidget_2->setCurrentIndex(1);
            stackedWidget_3->setCurrentIndex(1);
            break;
        case 2:
            stackedWidget_1->setCurrentIndex(2);
            stackedWidget_2->setCurrentIndex(2);
            stackedWidget_3->setCurrentIndex(2);
            break;
        case 3:
            stackedWidget_1->setCurrentIndex(3);
            stackedWidget_2->setCurrentIndex(3);
            stackedWidget_3->setCurrentIndex(3);
            break;
        default:
            // 默认情况，可以隐藏所有按钮或显示第一个按钮
            stackedWidget_1->setCurrentIndex(0);
            stackedWidget_2->setCurrentIndex(0);
            stackedWidget_3->setCurrentIndex(0);
            break;
        }
    });

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    mainLayout->setSpacing(1);

    mainLayout->addWidget(displayOut, 0, 0, 1, 5);
    mainLayout->addWidget(displayIn, 1, 0, 1, 5);

    mainLayout->addWidget(clearMemoryBtn, 3, 1);
    mainLayout->addWidget(readMemoryBtn, 3, 2);
    mainLayout->addWidget(addToMemoryBtn, 3, 3);
    mainLayout->addWidget(substractFromMemoryBtn, 3, 4);

    mainLayout->addWidget(clearAllBtn, 5, 2);
    mainLayout->addWidget(clearBtn, 5, 3);
    mainLayout->addWidget(backspaceBtn, 5, 4);

    mainLayout->addWidget(unitBtn, 3, 0);
    mainLayout->addWidget(TrigonometricComboBox, 6, 0);
    mainLayout->addWidget(stackedWidget_1, 6, 1);
    mainLayout->addWidget(stackedWidget_2, 6, 2);
    mainLayout->addWidget(stackedWidget_3, 6, 3);

    mainLayout->addWidget(squareBtn, 7, 0);
    mainLayout->addWidget(reciprocalBtn, 7, 1);
    mainLayout->addWidget(absoluteValueBtn, 7, 2);
    mainLayout->addWidget(factorialBtn, 7, 3);
    mainLayout->addWidget(squareRootBtn, 8, 0);
    mainLayout->addWidget(_1exBtn, 9, 0);
    mainLayout->addWidget(logarithmBtn, 10, 0);
    mainLayout->addWidget(naturalLogarithmBtn, 11, 0);

    mainLayout->addWidget(moduloBtn, 6, 4);
    mainLayout->addWidget(divisionBtn, 7, 4);
    mainLayout->addWidget(timesBtn, 8, 4);
    mainLayout->addWidget(minusBtn, 9, 4);
    mainLayout->addWidget(plusBtn, 10, 4);
    mainLayout->addWidget(equalBtn, 11, 4);

    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 8;
        int col = ((i - 1) % 3) + 1;
        mainLayout->addWidget(digitBtns[i], row, col);
    }
    mainLayout->addWidget(digitBtns[0], 11, 2);

    mainLayout->addWidget(piBtn, 5, 0);
    mainLayout->addWidget(eBtn, 5, 1);

    mainLayout->addWidget(changeSignBtn, 11, 1);
    mainLayout->addWidget(pointBtn, 11, 3);

    setLayout(mainLayout);
    setWindowTitle(tr("科学计算器"));

    //快捷键
    // 数字键
    createShortcut(QKeySequence("0"), digitBtns[0]);
    createShortcut(QKeySequence("1"), digitBtns[1]);
    createShortcut(QKeySequence("2"), digitBtns[2]);
    createShortcut(QKeySequence("3"), digitBtns[3]);
    createShortcut(QKeySequence("4"), digitBtns[4]);
    createShortcut(QKeySequence("5"), digitBtns[5]);
    createShortcut(QKeySequence("6"), digitBtns[6]);
    createShortcut(QKeySequence("7"), digitBtns[7]);
    createShortcut(QKeySequence("8"), digitBtns[8]);
    createShortcut(QKeySequence("9"), digitBtns[9]);
    // 小数点
    createShortcut(QKeySequence("."), pointBtn);
    // 常量
    createShortcut(QKeySequence("P"), piBtn);
    createShortcut(QKeySequence("E"), eBtn);
    // 一元运算
    createShortcut(QKeySequence("Q"), squareBtn);
    createShortcut(QKeySequence("@"), squareRootBtn);
    createShortcut(QKeySequence("R"), reciprocalBtn);
    createShortcut(QKeySequence("|"), absoluteValueBtn);
    createShortcut(QKeySequence("!"), factorialBtn);
    createShortcut(QKeySequence("L"), logarithmBtn);
    createShortcut(QKeySequence("N"), naturalLogarithmBtn);
    //三角函数
    createShortcut(QKeySequence("s"), sinBtn);
    createShortcut(QKeySequence("o"), cosBtn);
    createShortcut(QKeySequence("t"), tanBtn);
    createShortcut(QKeySequence("Shift+s"), arcsinBtn);
    createShortcut(QKeySequence("Shift+o"), arccosBtn);
    createShortcut(QKeySequence("Shift+t"), arctanBtn);
    createShortcut(QKeySequence("Ctrl+s"), sinhBtn);
    createShortcut(QKeySequence("Ctrl+o"), coshBtn);
    createShortcut(QKeySequence("Ctrl+t"), tanhBtn);
    createShortcut(QKeySequence("Ctrl+Shift+s"), asinhBtn);
    createShortcut(QKeySequence("Ctrl+Shift+o"), acoshBtn);
    createShortcut(QKeySequence("Ctrl+Shift+t"), atanhBtn);
    //  s   o   t   u   i   j
    // sin cos tan sec csc cot
    // arc shift
    // hyp ctrl
    // createShortcut(QKeySequence("u"), secBtn);
    // createShortcut(QKeySequence("i"), cscBtn);
    // createShortcut(QKeySequence("j"), cotBtn);
    // createShortcut(QKeySequence("Shift+u"), arcsecBtn);
    // createShortcut(QKeySequence("Shift+i"), arccscBtn);
    // createShortcut(QKeySequence("Shift+j"), arccotBtn);
    // createShortcut(QKeySequence("Ctrl+u"), sechBtn);
    // createShortcut(QKeySequence("Ctrl+i"), cschBtn);
    // createShortcut(QKeySequence("Ctrl+j"), cothBtn);
    // createShortcut(QKeySequence("Ctrl+Shift+u"), asechBtn);
    // createShortcut(QKeySequence("Ctrl+Shift+i"), acschBtn);
    // createShortcut(QKeySequence("Ctrl+Shift+j"), acothBtn);
    // 二元运算
    createShortcut(QKeySequence("+"), plusBtn);
    createShortcut(QKeySequence("-"), minusBtn);
    createShortcut(QKeySequence("*"), timesBtn);
    createShortcut(QKeySequence("/"), divisionBtn);
    createShortcut(QKeySequence("%"), moduloBtn);
    // case Qt::Key_AsciiCircum:    //乘方
    // case Qt::Key_Y:
    //     break;
    // 等号键
    createShortcut(QKeySequence("Enter"), equalBtn);
    createShortcut(QKeySequence("Return"), equalBtn);
    createShortcut(QKeySequence("="), equalBtn);
    // 删除
    createShortcut(QKeySequence("Backspace"), backspaceBtn);
    createShortcut(QKeySequence("Delete"), clearBtn);
    // 记忆
    createShortcut(QKeySequence("Ctrl+L"), clearMemoryBtn);
    createShortcut(QKeySequence("Ctrl+R"), readMemoryBtn);
    createShortcut(QKeySequence("Ctrl+P"), addToMemoryBtn);
    createShortcut(QKeySequence("Ctrl+Q"), substractFromMemoryBtn);
    // 括号 - 如果需要特殊处理，可以在这里添加
    // createShortcut(QKeySequence("("), ...);
    // createShortcut(QKeySequence(")"), ...);
}

QString QStringdigitFormat(const QString &number) {
    if (number.toDouble() < 1000) {
        return number;
    }
    QString result = number; // 复制输入字符串以避免修改原始数据
    bool isNegative = result.startsWith('-'); // 检查负号
    int decimalPos = result.indexOf('.'); // 查找小数点位置

    // 如果有小数点，只处理小数点左边的部分
    if (decimalPos != -1) {
        result = result.left(decimalPos); // 获取小数点左边的部分
    }
    // 如果是负数，先去掉负号
    if (isNegative) {
        result.remove("-");
    }

    int length = result.length();
    int commaInsertPos = result.length() - 1; // 确定插入逗号的位置

    // 从右边开始，每三位添加一个逗号
    for (int i = 2; i < length; i += 3) {
        if (commaInsertPos - i > 0) { // 避免在负号或小数点前插入逗号
            result.insert(commaInsertPos - i, ',');
        }
    }

    // 如果原始字符串包含小数点，将小数部分添加回去
    if (decimalPos != -1) {
        result += number.mid(decimalPos);
    }
    // 如果数字是负数，添加负号
    if (isNegative) {
        result.prepend('-');
    }

    return result;
}

QString removeTrailingZeros(QString text) {
    if (!text.contains('.')) {
        return text;
    }
    int i = text.length() - 1;
    while (text.at(i) == '0' && i > 0) {
        --i;
    }
    if (text.at(i) == '.')
        --i;
    return text.left(i + 1);
}

QString DoubledigitFormat(double number) {
    if (number >= 1e16 || (number <= 1e-19 && number > 0) || number <= -1e16 || (number >= -1e-19 && number < 0)) {
        // 如果数值大于等于1e16或小于等于1e-19，使用科学记数法
        QString text = QString::number(number, 'g');
        return text;
    }

    QString result = QString::number(number, 'f', 19); // 复制输入字符串以避免修改原始数据
    bool isNegative = result.startsWith('-'); // 检查负号
    int decimalPos = result.indexOf('.'); // 查找小数点位置

    // 如果有小数点，只处理小数点左边的部分
    if (decimalPos != -1) {
        result = result.left(decimalPos); // 获取小数点左边的部分
    }
    // 如果是负数，去掉负号
    if (isNegative) {
        result.remove("-");
    }

    int length = result.length();
    int commaInsertPos = result.length() - 1; // 确定插入逗号的位置

    // 从右边开始，每三位添加一个逗号
    for (int i = 2; i < length; i += 3) {
        if (commaInsertPos - i > 0) { // 避免在负号或小数点前插入逗号
            result.insert(commaInsertPos - i, ',');
        }
    }

    // 如果原始字符串包含小数点，将小数部分添加回去
    if (decimalPos != -1) {
        result += QString::number(number, 'f', 19).mid(decimalPos);
        result = removeTrailingZeros(result);
    }
    // 如果数字是负数，添加负号
    if (isNegative) {
        result.prepend('-');
    }

    return result;
}

void Calculator::addUnary(QString op, QString text)
{
    QString textAbove = displayOut->text();
    int length = textAbove.length();
    int i = length - 2;
    int bracket = 1;

    //一元后/二元后（同数字）/等号后
    if (last == "unary") {
        for ( ; i >= 0 && bracket > 0; --i) {
            if (textAbove.at(i) == ')')
                ++bracket;
            else if (textAbove.at(i) == '(')
                --bracket;
        }
        while (i >= 0 && textAbove.at(i) != ' ')
            --i;
        ++i;
        textAbove.insert(i, op + "( ");
        textAbove += " )";
    }
    else if(last == "equal") {
        textAbove = op + "( " + removeTrailingZeros(text) + " )";
    }
    else {
        textAbove += op + "( " + removeTrailingZeros(text) + " )";
    }

    displayOut->setText(textAbove);
    waitingForOperand = true;
    last = "unary";
}

void Calculator::removeUnary()
{
    QString textAbove = displayOut->text();
    int length = textAbove.length();
    int i = length - 2;
    int bracket = 1;
    for ( ; i >= 0 && bracket > 0; --i) {
        if (textAbove.at(i) == ')')
            ++bracket;
        else if (textAbove.at(i) == '(')
            --bracket;
    }
    while (i >= 0 && textAbove.at(i) != ' ')
        --i;
    ++i;
    displayOut->setText(textAbove.left(i));
    last = "";
}

//点击数字键
void Calculator::digitClicked()
{
    Button *clickedBtn = qobject_cast<Button *>(sender());
    int digitValue = clickedBtn->text().toInt();
    if (last == "equal") {
        displayOut->clear();
    }
    else if (last == "unary") {
        removeUnary();
    }
    if (waitingForOperand) {
        displayIn->clear();
        if (digitValue == 0) {
            displayIn->setText("0");
            return;
        }
    }
    if (displayIn->text().length() >= displayMaxLength - 1)
        return;

    displayIn->setText(QStringdigitFormat(displayIn->text().remove(',') + QString::number(digitValue)));

    waitingForOperand = false;
    last = "";
}

//点击小数点
void Calculator::pointClicked()
{
    if (waitingForOperand)
        displayIn->setText("0");
    if (displayIn->text().length() >= displayMaxLength - 1)
        return;
    if (last == "equal") {
        displayOut->clear();
    }
    else if (last == "unary") {
        removeUnary();
    }
    if (!displayIn->text().contains('.'))
        displayIn->setText(displayIn->text() + ".");
    waitingForOperand = false;
    last = "";
}

//点击常数键
void Calculator::constantClicked()
{
    Button *clickedBtn = qobject_cast<Button *>(sender());
    QString clickedConstant = clickedBtn->text();
    double constant = 0;

    if (clickedConstant == "π") {
        constant = M_PI;
    }
    else if (clickedConstant == "e") {
        constant = M_E;
    }

    if (last == "equal") {
        displayOut->clear();
    }
    else if (last == "unary") {
        removeUnary();
    }
    displayIn->setText(DoubledigitFormat(constant));
    waitingForOperand = true;
    last = "";
}

//点击正负切换键
void Calculator::changeSignClicked()
{
    if (last == "abort") return;
    QString text = displayIn->text();

    if (text == "0") {
    }
    else if (text.contains('-')) {
        displayIn->setText(text.mid(1));
    }
    else {
        displayIn->setText('-' + text);
    }

    if (displayOut->text().isEmpty()) return;

    addUnary("negate", text);
}

//点击一元运算符
//平方 开方 倒数 绝对值 阶乘 10^x log ln
void Calculator::unaryOperatorClicked()
{
    if (last == "abort") return;
    Button *clickedBtn = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedBtn->text();
    QString text = displayIn->text().remove(',');
    double operand = text.toDouble();
    double result = 0.0;
    QString op;

    if (clickedOperator == squareLabel)                 //平方square
    {
        result = std::pow(operand, 2.0);
        op = "sqr";
    }
    else if (clickedOperator == squareRootLabel)        //开方squareRoot
    {
        if (operand < 0.0) {
            abortOperation_1();
            return;
        }
        result = std::sqrt(operand);
        op = "\u221A";
    }
    else if (clickedOperator == reciprocalLabel)        //倒数reciprocal
    {
        if (operand == 0.0) {
            abortOperation_0();
            return;
        }
        result = 1.0 / operand;
        op = "1/";
    }
    else if (clickedOperator == absoluteValueLabel)     //绝对值absoluteValue
    {
        result = std::abs(operand);
        op = "abs";
    }
    else if (clickedOperator == factorialLabel)
    {
        if (operand < 0) {
            abortOperation_1();
            return;
        }
        unsigned long long res = 1;
        for (unsigned int i = 2; i <= operand; ++i) {
            res *= i;
        }
        result = res;
        op = "fact";
    }
    else if (clickedOperator == _1exLabel)              //10^x _1ex
    {
        result = std::pow(10.0, operand);
        op = "10^";
    }
    else if (clickedOperator == logarithmLabel)         //对数logarithm
    {
        if (operand <= 0.0) {
            abortOperation_1();
            return;
        }
        result = std::log10(operand);
        op = "log";
    }
    else if (clickedOperator == naturalLogarithmLabel)  //自然对数naturalLogarithm
    {
        if (operand <= 0.0) {
            abortOperation_1();
            return;
        }
        result = std::log(operand);
        op = "ln";
    }

    if (std::isinf(result)) {
        abortOperation_1();
        return;
    }

    displayIn->setText(DoubledigitFormat(result));

    addUnary(op, text);
}

void Calculator::unitClicked()
{
    QString text = unitBtn->text();
    if (text == "DEG") {
        unitBtn->setText("RAD");
    }
    else if (text == "RAD") {
        unitBtn->setText("DEG");
    }
}

//点击三角函数运算符
void Calculator::trigonometricClicked()
{
    if (last == "abort") return;
    Button *clickedBtn = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedBtn->text();
    QString text = displayIn->text().remove(',');
    double operand = text.toDouble();
    double result = 0.0;
    QString op = clickedOperator;
    QString unit = unitBtn->text();

    if (unit == "DEG") {
        if (!clickedOperator.startsWith("a"))
            operand = operand * (M_PI / 180.0);
        op += "₀";
    }
    else if (unit == "RAD") {
        op += "ᵣ";
    }

    if (clickedOperator == "sin")
    {
        result = sin(operand);
    }
    else if (clickedOperator == "cos")
    {
        result = cos(operand);
    }
    else if (clickedOperator == "tan")
    {
        result = tan(operand);
    }
    else if (clickedOperator == "arcsin")
    {
        result = asin(operand);
    }
    else if (clickedOperator == "arccos")
    {
        result = acos(operand);
    }
    else if (clickedOperator == "arctan")
    {
        result = atan(operand);
    }
    else if (clickedOperator == "sinh")
    {
        result = sinh(operand);
    }
    else if (clickedOperator == "cosh")
    {
        result = cosh(operand);
    }
    else if (clickedOperator == "tanh")
    {
        result = tanh(operand);
    }
    else if (clickedOperator == "asinh")
    {
        result = asinh(operand);
    }
    else if (clickedOperator == "acosh")
    {
        result = acosh(operand);
    }
    else if (clickedOperator == "atanh")
    {
        result = atanh(operand);
    }

    if (unit == "DEG" && clickedOperator.startsWith("a")) {
        result = result * (180.0 / M_PI);
    }

    displayIn->setText(DoubledigitFormat(result));

    addUnary(op, text);
}

//double类型取模
double mod(double a, double b) {
    double remainder = std::fmod(a, b);
    if (remainder * b < 0.0) {
        remainder = b + remainder;
    }
    return remainder;
}

//点击加法运算符
void Calculator::additiveOperatorClicked()
{
    if (last == "abort") return;
    Button *clickedBtn = qobject_cast<Button *>(sender());
    if (!clickedBtn)
        return;
    QString clickedOperator = clickedBtn->text();
    QString textAbove = displayOut->text();
    // 二元后 切换运算符处理
    if (waitingForOperand && last == "binary") {
        // 乘法换加法
        if (textAbove.endsWith(timesLabel+' ') || textAbove.endsWith(divisionLabel+' ') || textAbove.endsWith(moduloLabel+' ')) {
            calculate(factorSoFar, pendingAdditiveOperator);
            factorSoFar = 0.0;
            pendingMultiplicativeOperator.clear();
        }
        if (textAbove.endsWith(moduloLabel+' ')) {
            textAbove.chop(4);
        }
        else {
            textAbove.chop(2);
        }
        textAbove += clickedOperator + ' ';
        displayOut->setText(textAbove);
        pendingAdditiveOperator = clickedOperator;
        return;
    }
    QString displayText = displayIn->text().remove(',');
    double operand = displayText.toDouble();

    //处理乘法运算符
    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation_0();
            return;
        }
        displayIn->setText(DoubledigitFormat(factorSoFar));
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }

    //处理加法运算符
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation_1();
            return;
        }
        displayIn->setText(DoubledigitFormat(sumSoFar));
    }
    else {
        sumSoFar = operand;
        displayIn->setText(removeTrailingZeros(displayText));
    }

    //一元后/数字后/等号后（同归零）
    if (last == "unary") {
        displayOut->setText(textAbove + ' ' + clickedOperator + ' ');
    }
    else if (last == "equal") {
        displayOut->setText(removeTrailingZeros(displayText) + ' ' + clickedOperator + ' ');
    }
    else
        displayOut->setText(textAbove + removeTrailingZeros(displayText) + ' ' + clickedOperator + ' ');

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
    last = "binary";
}

//点击乘法运算符
void Calculator::multiplicativeOperatorClicked()
{
    if (last == "abort") return;
    Button *clickedBtn = qobject_cast<Button *>(sender());
    if (!clickedBtn)
        return;
    QString clickedOperator = clickedBtn->text();
    QString textAbove = displayOut->text();
    // 二元后 切换运算符处理
    if (waitingForOperand && last == "binary") {
        // 加法换乘法
        if (textAbove.endsWith(plusLabel+' ') || textAbove.endsWith(minusLabel+' ')) {
            factorSoFar = sumSoFar;
            sumSoFar = 0.0;
            pendingAdditiveOperator.clear();
            textAbove.chop(2);
            textAbove = "( " + textAbove + ") ";
        }
        else if (textAbove.endsWith(moduloLabel+' ')) {
            textAbove.chop(4);
        }
        else {
            textAbove.chop(2);
        }
        textAbove += clickedOperator + ' ';
        displayOut->setText(textAbove);
        pendingMultiplicativeOperator = clickedOperator;
        return;
    }
    QString displayText = displayIn->text().remove(',');
    double operand = displayText.toDouble();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation_0();
            return;
        }
        displayIn->setText(DoubledigitFormat(factorSoFar));
    }
    else {
        factorSoFar = operand;
        displayIn->setText(removeTrailingZeros(displayText));
    }

    //一元后/数字后/等号后（同归零）
    if (last == "unary") {
        displayOut->setText(textAbove + ' ' + clickedOperator + ' ');
    }
    else if (last == "equal") {
        displayOut->setText(removeTrailingZeros(displayText) + ' ' + clickedOperator + ' ');
    }
    else
        displayOut->setText(textAbove + removeTrailingZeros(displayText) + ' ' + clickedOperator + ' ');

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
    last = "binary";
}

//点击等号
void Calculator::equalClicked()
{
    if (last == "abort") return;
    QString displayText = displayIn->text().remove(',');
    double operand = displayText.toDouble();
    QString textAbove = displayOut->text();

    if (!pendingMultiplicativeOperator.isEmpty()) {
        if (!calculate(operand, pendingMultiplicativeOperator)) {
            abortOperation_0();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation_1();
            return;
        }
        pendingAdditiveOperator.clear();
    }
    else {
        sumSoFar = operand;
    }

    displayIn->setText(DoubledigitFormat(sumSoFar));

    //###一元后/二元后（同数字）/（等号（同归零））
    if (last == "unary") {
        displayOut->setText(textAbove + " = ");
    }
    else if (last == "equal") {
        displayOut->setText(removeTrailingZeros(displayText) + " = ");
    }
    else {
        displayOut->setText(textAbove + removeTrailingZeros(displayText) + " = ");
    }

    sumSoFar = 0.0;
    waitingForOperand = true;
    last = "equal";
}

bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == plusLabel) {
        sumSoFar += rightOperand;
    }
    else if (pendingOperator == minusLabel) {
        sumSoFar -= rightOperand;
    }
    else if (pendingOperator == timesLabel) {
        factorSoFar *= rightOperand;
    }
    else if (pendingOperator == divisionLabel) {
        if (rightOperand == 0.0)
            return false;
        factorSoFar /= rightOperand;
    }
    else if (pendingOperator == moduloLabel) {
        if (rightOperand == 0.0)
            return false;
        factorSoFar = mod(factorSoFar, rightOperand);
    }
    else sumSoFar += rightOperand;
    return true;
}

//点击退格键
void Calculator::backspaceClicked()
{
    if (waitingForOperand && last != "abort")
        return;
    if (last == "abort") clear();

    QString text = displayIn->text();
    text.chop(1);
    if (text.isEmpty() || text == '-') {
        text = "0";
        waitingForOperand = true;
    }
    displayIn->setText(QStringdigitFormat(text.remove(',')));
}

//点击CE键，重置操作数为0
void Calculator::clear()
{
    displayIn->setText("0");
    if (last == "unary") {
        removeUnary();
    }
    waitingForOperand = true;
    if (last == "abort") last = "equal";
}

//点击C键，重置计算器
void Calculator::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    displayOut->setText("");
    displayIn->setText("0");
    waitingForOperand = true;
    last = "equal";
}

// void Calculator::setMemory(){}               //MS

//MC
void Calculator::clearMemory(){
    memory = 0.0;
}

//MR
void Calculator::readMemory(){
    displayIn->setText(DoubledigitFormat(memory));
    waitingForOperand = true;
    if (last == "unary") {
        removeUnary();
    }
}

//M+
void Calculator::addToMemory(){
    memory += displayIn->text().toDouble();
}

//M-
void Calculator::substractFromMemory(){
    memory -= displayIn->text().toDouble();
}

template<typename PointerToMemberFunction>
Button *Calculator::createButton(const QString &text, const PointerToMemberFunction &member)
{
    Button *btn = new Button(text);
    connect(btn, &Button::clicked, this, member);
    return btn;
}

void Calculator::createShortcut(QKeySequence key, Button *button)
{
    QShortcut *shortcut = new QShortcut(key, this);
    connect(shortcut, &QShortcut::activated, this, [button]() {
        button->click();
    });
}

void Calculator::abortOperation_0()
{
    clearAll();
    displayIn->setText(tr("除数不能为零"));
    waitingForOperand = true;
    last = "abort";
}

void Calculator::abortOperation_1()
{
    clearAll();
    displayIn->setText(tr("无效输入"));
    waitingForOperand = true;
    last = "abort";
}
