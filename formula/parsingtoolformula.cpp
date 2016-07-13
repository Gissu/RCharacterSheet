/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                                      *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify     *
    *   it under the terms of the GNU General Public License as published by  *
    *   the Free Software Foundation; either version 2 of the License, or     *
    *   (at your option) any later version.                                   *
    *                                                                         *
    *   This program is distributed in the hope that it will be useful,       *
    *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
    *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
    *   GNU General Public License for more details.                          *
    *                                                                         *
    *   You should have received a copy of the GNU General Public License     *
    *   along with this program; if not, write to the                         *
    *   Free Software Foundation, Inc.,                                       *
    *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
    ***************************************************************************/
#include "parsingtoolformula.h"
#include "nodes/operator.h"

ParsingToolFormula::ParsingToolFormula()
    : m_variableHash(NULL)
{
    //ABS,MIN,MAX,IF,FLOOR,CEIL,AVG
    m_hashOp = new QHash<QString,ParsingToolFormula::FormulaOperator>();
    m_hashOp->insert(QStringLiteral("abs"),ABS);
    m_hashOp->insert(QStringLiteral("min"),MIN);
    m_hashOp->insert(QStringLiteral("max"),MAX);
    m_hashOp->insert(QStringLiteral("if"),IF);
    m_hashOp->insert(QStringLiteral("floor"),FLOOR);
    m_hashOp->insert(QStringLiteral("ceil"),CEIL);
    m_hashOp->insert(QStringLiteral("avg"),AVG);
}

bool ParsingToolFormula::readFormula(QString& str, FormulaNode* previous)
{
    if(readOperand(str,previous))
    {
        while(readScalarOperator(str,previous))
        {
            if(!readOperand(str,previous))
            {
                return false;
            }
        }
        return true;
    }
    else
        return false;
}

bool ParsingToolFormula::readScalarOperator(QString& str, FormulaNode* previous)
{

}

bool ParsingToolFormula::readOperand(QString & str, FormulaNode * previous)
{
    if(readNumber(str,previous))
    {
        return true;
    }
    else if(readFieldRef(str,previous))
    {
        return true;
    }
    else if(readOperator(str,previous))
    {
        return true;
    }
    return false;
}

bool ParsingToolFormula::readOperator(QString& str, FormulaNode* previous)
{
    bool found = false;
    for(int i = 0; i < m_hashOp->keys().size() && !found ; ++i)
    {
        QString key = m_hashOp->keys().at(i);
        if(str.startsWith(key))
        {
            str=str.remove(0,key.size());
            OperatorFNode* node = new OperatorFNode();
            previous->setNext(node);
            node->setOperator(m_hashOp->value(key));
            FormulaNode* nextNode=NULL;
            if(str.startsWith("("))
            {
                while(readFormula(str,nextNode))
                {
                   node->addParameter(nextNode);
                   nextNode=NULL;
                   if(str.startsWith(","))
                   {
                       str=str.remove(0,1);
                   }
                }
                if(str.startsWith(")"))
                {
                    str=str.remove(0,1);
                }
            }

        }
    }

}

bool ParsingToolFormula::readFieldRef(QString& str, FormulaNode* previous)
{
    if(str.isEmpty())
        return false;
    if(str.startsWith("${"))
    {
        str=str.remove(0,2);
    }
    QString key;
    int post = str.indexOf('}');
    key = str.left(post);

    if(NULL!=m_variableHash)
    {
        if(m_variableHash->contains(key))
        {
            QString value = m_variableHash->value(key);
            bool ok;
            int valueInt = value.toInt(&ok);
            if(ok)
            {
               // myNumber = valueInt;
                str=str.remove(0,post+1);
                return true;
            }

        }
    }


    return false;
}

bool ParsingToolFormula::readNumber(QString& str, FormulaNode* previous)
{
    if(str.isEmpty())
        return false;

    QString number;
    int i=0;
    while(i<str.length() && ((str[i].isNumber()) || (str[i]=='.') || ( (i==0) && (str[i]=='-'))))
    {
        number+=str[i];
        ++i;
    }

    bool ok;
    qreal r = number.toDouble(&ok);
    if(ok)
    {
        str=str.remove(0,number.size());
        return true;
    }


    return false;
}

