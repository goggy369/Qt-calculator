#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QStackedWidget>
#include <QComboBox>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE
class Button;

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);

private slots:
    void digitClicked();
    void pointClicked();
    void constantClicked();
    void changeSignClicked();
    void unitClicked();
    void unaryOperatorClicked();    //平方 开方 倒数 绝对值 阶乘 10^x log ln
    void trigonometricClicked();
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalClicked();
    void backspaceClicked();
    void clear();                   //CE
    void clearAll();                //C
    void clearMemory();             //MC
    void readMemory();              //MR
    void addToMemory();             //M+
    void substractFromMemory();     //M-
    // void setMemory();               //MS
    // void showMemory();

private:
    int displayMaxLength;

    template<typename PointerToMemberFunction>
    Button *createButton(const QString &text, const PointerToMemberFunction &member);
    void createShortcut(QKeySequence key, Button *button);
    void addUnary(QString op, QString text);
    void removeUnary();
    void abortOperation_0();
    void abortOperation_1();
    bool calculate(double rightOperand, const QString &pendingOperator);

    double memory;
    double sumSoFar;
    double factorSoFar;
    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    bool waitingForOperand;
    QString last;   // 上一步操作

    QLineEdit *displayIn;
    QLineEdit *displayOut;

    Button *clearMemoryBtn;
    Button *readMemoryBtn;
    Button *addToMemoryBtn;
    Button *substractFromMemoryBtn;
    // Button *setMemoryBtn;
    // Button *showMemoryBtn;

    Button *piBtn;
    Button *eBtn;
    Button *backspaceBtn;
    Button *clearBtn;
    Button *clearAllBtn;

    Button *unitBtn;
    QComboBox *TrigonometricComboBox;
    QStackedWidget *stackedWidget_1;
    QStackedWidget *stackedWidget_2;
    QStackedWidget *stackedWidget_3;
    Button *sinBtn;
    Button *cosBtn;
    Button *tanBtn;
    Button *arcsinBtn;
    Button *arccosBtn;
    Button *arctanBtn;
    Button *sinhBtn;
    Button *coshBtn;
    Button *tanhBtn;
    Button *asinhBtn;
    Button *acoshBtn;
    Button *atanhBtn;

    Button *squareBtn;
    Button *reciprocalBtn;
    Button *absoluteValueBtn;
    Button *factorialBtn;
    Button *squareRootBtn;
    Button *_1exBtn;
    Button *logarithmBtn;
    Button *naturalLogarithmBtn;

    Button *moduloBtn;
    Button *divisionBtn;
    Button *timesBtn;
    Button *minusBtn;
    Button *plusBtn;
    Button *equalBtn;

    enum { NumDigitButtons = 10 };
    Button *digitBtns[NumDigitButtons];
    Button *pointBtn;
    Button *changeSignBtn;

};
#endif // CALCULATOR_H
