/////////////////////////////////////////////////////////////////////
//
// Graphs
//
// Copyright (c) 2016-2017 DonRumata710
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

#include "wavelet.h"
#include "rangemaster.h"


WaveletWindow::WaveletWindow (QWidget* parent) : QDialog (parent)
{
    m_wavelet.setupUi (this);

    m_list << "HAAR";
    m_list << "FHAT";
    m_list << "MORLET";
    m_wavelet.cbWaveletFunction->addItems (m_list);

    connect (m_wavelet.cbWaveletFunction, SIGNAL(currentIndexChanged(QString)), SLOT(look_wavelet_changing(QString)));
    m_wavelet.label_14->setHidden(true);
    m_wavelet.sbLength->setHidden(true);

    RangeMaster* range = new RangeMaster (this);
    range->set_spin_box (m_wavelet.sbMax, m_wavelet.sbMin);
}

WaveletInitParams WaveletWindow::get_wavelet_info() const
{
    return {
        m_wavelet.cbWaveletFunction->currentText ().toStdString(),
        (size_t) m_wavelet.sbMin->value (),
        (size_t) m_wavelet.sbMax->value (),
        (size_t) m_wavelet.sbLength->value ()
    };
}

void WaveletWindow::look_wavelet_changing (const QString& str)
{
    if (str == "MORLET")
    {
        m_wavelet.label_14->setHidden(false);
        m_wavelet.sbLength->setHidden(false);
    }
    else
    {
        m_wavelet.label_14->setHidden(true);
        m_wavelet.sbLength->setHidden(true);
    }
}
