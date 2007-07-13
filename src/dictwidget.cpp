#include "dictwidget.h"

#include <QTextBrowser>
#include <QHBoxLayout>
#include <cassert>

DictWidget::DictWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    translationView = new QTextBrowser(this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(translationView);
    setLayout(layout);
    m_dict = 0;
    m_flags = DictCore::Html | DictCore::Reformat;
}

void DictWidget::setDict(DictCore *dict)
{
    m_dict = dict;
}

const DictCore* DictWidget::dict() const
{
    return m_dict;
}

void DictWidget::setTranslationFlags(DictCore::TranslationFlags flags)
{
    m_flags = flags | DictCore::Html;
    translate(m_translatedWord);
}

DictCore::TranslationFlags DictWidget::translationFlags() const
{
    return m_flags;
}

bool DictWidget::translate(const QString &str)
{
    const QString htmlStyle = 
        "<style>"
        "font.normal { "
            "color: black; } "
        "font.dict_name { "
            "color: blue; "
            "font-style: italic; }"
        "font.title { "
            "font-size: 16pt; "
            "font-weight: bold; }"
        "font.explanation { "
            "color: #7f7f7f; "
            "font-style: italic; }"
        "font.abbreviature { "
            "font-weight: bold; }"
        "font.example { "
            "font-style: italic; } "
        "</style>";
    m_translatedWord = str;
    QString result = m_dict->translate(str, m_flags);
    translationView->setHtml(htmlStyle + result);
    return result.isEmpty();
}

QString DictWidget::translatedWord() const
{
    return m_translatedWord;
}

