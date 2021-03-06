/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://www.rolisteam.org/                                                *
*                                                                          *
*  This file is part of rcse                                               *
*                                                                          *
* rcse is free software; you can redistribute it and/or modify             *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* rcse is distributed in the hope that it will be useful,                  *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the                            *
* Free Software Foundation, Inc.,                                          *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
***************************************************************************/
#ifndef TABLEFIELD_H
#define TABLEFIELD_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsItem>
#include "charactersheetitem.h"
#include "field.h"
#include "qqmlobjectlistmodel.h"
#include "qqmlhelpers.h"
#include "propertyhelpers.h"
#include <QObject>
#include <QStandardItemModel>
#ifdef RCSE
#include "tablecanvasfield.h"
#else
class TableCanvasField : public QGraphicsObject
{
    TableCanvasField();
};
#endif

/**
 * @brief The LineFieldItem class
 */
class LineFieldItem : public QObject
{
    Q_OBJECT
public:
    explicit LineFieldItem (QObject * parent = nullptr);
    ~LineFieldItem();
    void insertField(Field* field);

    Q_INVOKABLE Field* getField(int k) const;

    QList<Field *> getFields() const;
    void setFields(const QList<Field *> &fields);

private:
    QList<Field*> m_fields;
};

/**
 * @brief The LineModel class
 */
class LineModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum customRole {LineRole = Qt::UserRole+1};
    LineModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(QModelIndex& index, QVariant data,int role);
    QHash<int, QByteArray>  roleNames() const;
    void generateAllField(int count);
private:
    QList<LineFieldItem*> m_lines;
};


/**
 * @brief The Field class managed text field in qml and datamodel.
 */
class TableField : public Field
{
    Q_OBJECT
    Q_PROPERTY (QAbstractItemModel* model READ getModel CONSTANT)
public:
    enum TextAlign {TopRight, TopMiddle, TopLeft, CenterRight,CenterMiddle,CenterLeft,BottomRight,BottomMiddle,BottomLeft};
    explicit TableField(bool addCount = true,QGraphicsItem* parent = 0);
    explicit TableField(QPointF topleft,bool addCount = true,QGraphicsItem* parent = 0);


    void generateQML(QTextStream &out, CharacterSheetItem::QMLSection sec,int i, bool isTable=false);
    virtual ~TableField();

    LineModel* getModel () const;

    virtual bool mayHaveChildren()const;

    int getRowCount() const;
    void setRowCount(int rowCount);

    void makeLines();
public slots:
    void addLine();
    void removeLine(int);
protected:
    void init();



protected:
    TableCanvasField* m_tableCanvasField = nullptr;
    LineModel* m_model = nullptr;
    int m_rowCount;
};

#endif // TABLEFIELD_H
