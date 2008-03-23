/*****************************************************************************
 * cssedit.cpp - QStarDict, a StarDict clone written with using Qt           *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#include "cssedit.h"

#include <QApplication>
#include <QColorDialog>

namespace QStarDict
{
CSSEdit::CSSEdit(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(m_elementCombo, SIGNAL(currentIndexChanged(int)), SLOT(setCurrentElement(int)));
    connect(m_fontCombo, SIGNAL(currentFontChanged(const QFont&)), SLOT(propertyChanged()));
    connect(m_sizeSpin, SIGNAL(valueChanged(int)), SLOT(propertyChanged()));
    connect(m_boldButton, SIGNAL(toggled(bool)), SLOT(propertyChanged()));
    connect(m_italicButton, SIGNAL(toggled(bool)), SLOT(propertyChanged()));
    connect(m_underlineButton, SIGNAL(toggled(bool)), SLOT(propertyChanged()));
    connect(m_colorButton, SIGNAL(clicked()), SLOT(colorSelectClicked()));
    connect(m_backgroundButton, SIGNAL(clicked()), SLOT(colorSelectClicked()));
}

void CSSEdit::setCSS(const QString &css)
{
    m_elements.clear();
    bool inBlock = false;
    QString element;
    QString currentProperty;
    QString currentValue;
    for (int i = 0; i < css.length(); ++i)
    {
        if (! inBlock)
        {
            element.clear();
            for (; i < css.length() && css[i] != '{'; ++i)
                if (! css[i].isSpace())
                    element += css[i];
            if (css[i] == '{')
            {
                inBlock = true;
                ++i;
            }
        }
        else
        {
            currentProperty.clear();
            for (; i < css.length() && css[i] != ':'; ++i)
                if (! css[i].isSpace())
                    currentProperty += css[i];
            if (! inBlock)
                continue;
            currentValue.clear();
            char quote = '\0';
            for (++i; i < css.length() && css[i] != ';'; ++i)
            {
                if (quote)
                {
                    if (css[i] == quote)
                    {
                        while (i < css.length() && css[i] != ';')
                            ++i;
                        break;
                    }
                    else
                        currentValue += css[i];
                }
                else if (! css[i].isSpace())
                {
                    if (css[i] == '\'' || css[i] == '\"')
                        quote = css[i].toAscii();
                    else
                        currentValue += css[i];
                }
            }
            m_elements[element][currentProperty] = currentValue;
            while (css[i + 1].isSpace())
                ++i;
            if (css[i + 1] == '}')
            {
                ++i;
                inBlock = false;
            }
        }
    }

    updateElementCombo();
    updatePreview();
}

void CSSEdit::setElementsAliases(const QHash<QString, QString> &aliases)
{
    m_elementsAliases = aliases;
    updateElementCombo();
    updatePreview();
}

QString CSSEdit::css() const
{
    QString result;
    for (QHash<QString, Element>::const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
    {
        result += i.key() + "\n{\n";
        for (Element::const_iterator j = i->begin(); j != i->end(); ++j)
        {
            result += j.key() + ": ";
            if (j->contains(' '))
                result += "\"" + *j + "\";\n";
            else
                result += *j + ";\n";
        }
        result += "}\n";
    }
    return result;
}

void CSSEdit::setCurrentElement(int index)
{
    if (! m_elements.contains(m_elementCombo->itemData(index).toString()))
        return;
    m_currentElement = m_elementCombo->itemData(index).toString();
    Element *element = &m_elements[m_currentElement];
    Element parentElement = getParentElement(m_currentElement);

    QColor color(element->contains("color") ? element->value("color") : parentElement["color"]);
    m_colorButton->setText(color.name());
    QPalette palette = m_colorButton->palette();
    palette.setColor(QPalette::Normal, QPalette::ButtonText, color);
    m_colorButton->setPalette(palette);

    color = QColor(element->contains("background-color") ?
        element->value("background-color") : parentElement["background-color"]);
    m_backgroundButton->setText(color.name());
    palette = m_backgroundButton->palette();
    palette.setColor(QPalette::Normal, QPalette::ButtonText, color);
    m_backgroundButton->setPalette(palette);

    m_fontCombo->setCurrentFont(element->contains("font-family") ?
            element->value("font-family") : parentElement["font-family"]);

    QString value = element->contains("font-size") ?
        element->value("font-size") : parentElement["font-size"];
    if (! value.endsWith("pt"))
        value = parentElement["font-size"];
    m_sizeSpin->setValue(value.left(value.length() - 2).toInt());

    value = element->contains("font-weight") ? element->value("font-weight") : parentElement["font-weight"];
    m_boldButton->setChecked(value == "bold");

    value = element->contains("font-style") ? element->value("font-style") : parentElement["font-style"];
    m_italicButton->setChecked(value == "italic");

    value = element->contains("text-decoration") ? element->value("text-decoration") : parentElement["text-decoration"];
    m_underlineButton->setChecked(value == "underline");

    updatePreview();
}

void CSSEdit::propertyChanged()
{
    if (! sender())
        return;
    if (! m_elements.contains(m_currentElement))
        return;
    Element *element = &m_elements[m_currentElement];
    Element parentElement = getParentElement(m_currentElement);

    if (sender() == m_fontCombo)
    {
        QString font = m_fontCombo->currentText();
        if (parentElement["font-family"] == font)
            element->remove("font-family");
        else
            element->insert("font-family", font);
    }
    else if (sender() == m_sizeSpin)
    {
        QString size = QString::number(m_sizeSpin->value()) + "pt";
        if (parentElement["font-size"] == size)
            element->remove("font-size");
        else
            element->insert("font-size", size);
    }
    else if (sender() == m_boldButton)
    {
        QString weight = (m_boldButton->isChecked() ? "bold" : "normal");
        if (parentElement["font-weight"] == weight)
            element->remove("font-weight");
        else
            element->insert("font-weight", weight);
    }
    else if(sender() == m_italicButton)
    {
        QString style = (m_italicButton->isChecked() ? "italic" : "normal");
        if (parentElement["font-style"] == style)
            element->remove("font-style");
        else
            element->insert("font-style", style);
    }
    else if(sender() == m_underlineButton)
    {
        QString decoration = (m_underlineButton->isChecked() ? "underline" : "none");
        if (parentElement["text-decoration"] == decoration)
            element->remove("text-decoration");
        else
            element->insert("text-decoration", decoration);
    }

    updatePreview();
}

void CSSEdit::colorSelectClicked()
{
    QToolButton *colorButton;
    QString propertyName;
    if (sender() == m_colorButton)
    {
        colorButton = m_colorButton;
        propertyName = "color";
    }
    else if (sender() == m_backgroundButton)
    {
        colorButton = m_backgroundButton;
        propertyName = "background-color";
    }
    else
        return;

    if (! m_elements.contains(m_currentElement))
        return;
    Element *element = &m_elements[m_currentElement];
    Element parentElement = getParentElement(m_currentElement);
    QColor color = QColorDialog::getColor(QColor(colorButton->text()), this);
    if (color.isValid())
    {
        colorButton->setText(color.name());
        QPalette palette = colorButton->palette();
        palette.setColor(QPalette::Normal, QPalette::ButtonText, color);
        m_colorButton->setPalette(palette);
        if (parentElement[propertyName] == color.name())
            element->remove(propertyName);
        else
            element->insert(propertyName, color.name());
        updatePreview();
    }
}

void CSSEdit::updatePreview()
{
    QString html = "<style>" +  css()  +  "</style>";
    for (QHash<QString, Element>::const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
    {
        QString alias;
        if (m_elementsAliases.contains(i.key()))
            alias = m_elementsAliases.value(i.key());
        else
            alias = i.key();
        int pos = i.key().indexOf('.');
        if (pos == -1)
            html += "<" + i.key() + ">" + alias + "</" + i.key() + "><br>";
        else
        {
            QString parent = i.key().left(pos);
            QString class_ = i.key().mid(pos + 1);
            html += "<" + parent + " class=\'" + class_ + "\'>" + alias + "</" + parent + "><br>";
        }
    }
    m_preview->setHtml(html);
}

void CSSEdit::updateElementCombo()
{
    m_elementCombo->clear();
    for (QHash<QString, Element>::const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
    {
        QString alias;
        if (m_elementsAliases.contains(i.key()))
            alias = m_elementsAliases.value(i.key());
        else
            alias = i.key();
        m_elementCombo->addItem(alias, i.key());
    }
    if (m_elements.begin() != m_elements.end())
        m_currentElement = m_elements.begin().key();
    else
        m_currentElement.clear();
}

CSSEdit::Element CSSEdit::getParentElement(const QString &elementName)
{
    Element body = m_elements.value("body");

    if (elementName == "body")
    {
        body["color"] = QApplication::palette().color(QPalette::Normal, QPalette::Text).name();
        body["background-color"] = QApplication::palette().color(QPalette::Normal, QPalette::Base).name();
        body["font-family"] = QApplication::font().family();
        body["font-size"] = QString::number(QApplication::font().pointSize()) + "pt";
        body["font-weight"] = (QApplication::font().weight() == QFont::Bold) ? "bold" : "normal";
        body["font-style"] = (QApplication::font().style() == QFont::StyleItalic) ? "italic" : "normal";
        body["text-decoration"] = QApplication::font().underline() ? "underline" : "none";
        return body;
    }
    else
    {
        if (! body.contains("color"))
            body["color"] = QApplication::palette().color(QPalette::Normal, QPalette::Text).name();
        if (! body.contains("background-color"))
            body["background-color"] = QApplication::palette().color(QPalette::Normal, QPalette::Base).name();
        if (! body.contains("font-family"))
            body["font-family"] = QApplication::font().family();
        if (! body.contains("font-size"))
            body["font-size"] = QString::number(QApplication::font().pointSize()) + "pt";
        if (! body.contains("font-weight"))
            body["font-weight"] = (QApplication::font().weight() == QFont::Bold) ? "bold" : "normal";
        if (! body.contains("font-style"))
            body["font-style"] = (QApplication::font().style() == QFont::StyleItalic) ? "italic" : "normal";
        if (! body.contains("text-decoration"))
            body["text-decoration"] = QApplication::font().underline() ? "underline" : "none";
    }

    QString parentName;
    int pos = elementName.indexOf('.');
    if (pos != -1)
    {
        parentName = elementName.left(pos);
        if (m_elements.contains(parentName))
        {
            Element parent = m_elements[parentName];
            for (Element::const_iterator i = body.begin(); i != body.end(); ++i)
                if (! parent.contains(i.key()))
                    parent[i.key()] = *i;
            return parent;
        }
        else
            return body;
    }
    return body;
}
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

