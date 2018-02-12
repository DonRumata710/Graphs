/////////////////////////////////////////////////////////////////////
//
// Graphs
//
// Copyright (c) 2016-2018 DonRumata710
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/////////////////////////////////////////////////////////////////////

#include "multygraph.h"


MultyGraphTools::MultyGraphTools (QWidget* parent) :
    QToolBar (parent),
    m_curves (new QComboBox (this)),
    m_bAdd (new QPushButton (this)),
    m_bRem (new QPushButton (this))
{
    m_bAdd->setText (tr ("Add curve"));
    m_bRem->setText (tr ("Remove curve"));

    addWidget (m_curves.get ());
    addWidget (m_bAdd.get ());
    addWidget (m_bRem.get ());
}

void MultyGraphTools::set_headers (QStringList h)
{
    m_curves->addItems (h);
}

void MultyGraphTools::connect_widget (QWidget* w)
{
    connect (m_curves.get (), SIGNAL (currentTextChanged (const QString&)), w, SLOT (set_current_curve (const QString&)));
    connect (m_bAdd.get (), SIGNAL (clicked ()), w, SLOT (attach_curve ()));
    connect (m_bRem.get (), SIGNAL (clicked ()), w, SLOT (detach_curve ()));
}
