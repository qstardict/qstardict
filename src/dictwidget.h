#ifndef DICTWIDGET_H
#define DICTWIDGET_H

#include <QWidget>
#include "dictcore.h"

class QTextBrowser;

class DictWidget: public QWidget
{
    public:
        DictWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

        void setDict(DictCore *dict);
        const DictCore* dict() const;

        void setTranslationFlags(DictCore::TranslationFlags flags);
        DictCore::TranslationFlags translationFlags() const;

        bool translate(const QString &str);
        QString translatedWord() const;

    private:
        QTextBrowser *translationView;
        DictCore *m_dict;
        DictCore::TranslationFlags m_flags;
        QString m_translatedWord;
};

#endif // DICTWIDGET_H

