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

#include "rangemaster.h"


RangeMaster::RangeMaster (QObject *parent) :
    QObject (parent)
{}


void RangeMaster::set_spin_box (QDoubleSpinBox* max, QDoubleSpinBox* min)
{
    maximum = max;
    minimum = min;
    connect (max, SIGNAL (valueChanged (double)), SLOT (change_maximum (double)));
    connect (min, SIGNAL (valueChanged (double)), SLOT (change_minimum (double)));
    max->setMinimum (min->value ());
    min->setMaximum (max->value ());
}

void RangeMaster::set_spin_box (QSpinBox* max, QSpinBox* min)
{
    maximum = max;
    minimum = min;
    connect (max, SIGNAL (valueChanged (int)), SLOT (change_maximum (int)));
    connect (min, SIGNAL (valueChanged (int)), SLOT (change_minimum (int)));
    max->setMinimum (min->value ());
    min->setMaximum (max->value ());
}


void RangeMaster::change_maximum (double val)
{
    qobject_cast<QDoubleSpinBox*> (minimum)->setMaximum (val);
}

void RangeMaster::change_minimum (double val)
{
    qobject_cast<QDoubleSpinBox*> (maximum)->setMinimum (val);
}

void RangeMaster::change_maximum (int val)
{
    qobject_cast<QSpinBox*> (minimum)->setMaximum (val);
}

void RangeMaster::change_minimum (int val)
{
    qobject_cast<QSpinBox*> (maximum)->setMinimum (val);
}
