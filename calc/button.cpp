#include "button.h"

Button::Button(const QString &text, QWidget *parent)
    : QToolButton(parent)
{
    setText(text);
    setMinimumWidth(62);
    setMinimumHeight(34);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet(
        "QToolButton {"
        "  background: #F9F9F9;"
        "  font: normal normal 15px comic sans ms;"
        // "  font: normal normal 16px consolas;"
        // "  font: normal normal 16px century;"
        // "  font: normal normal 16px courier new;"
        "}"

        "QToolButton:hover {"               //滑过
        "  background: #F6F6F6;"
        "}"
        "QToolButton:pressed {"             //点击
        "  background: #F4F4F4;"
        "  color: #5D5D5D;"
        "}"
        );
}
