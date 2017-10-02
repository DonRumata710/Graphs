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

#include "plotdata.h"
#include "threadscontrol.h"

#include <algorithm>
#include <iterator>


struct PlotData::PrivateData
{
    std::string name = "Data";
    std::vector<Row> series;
};


class Function
{
public:
    Function () {}

    void set_func (double (*func) (double)) { m_func = func; }

    double operator() (double x) const      { return func (x); }

    void set_friquency (double friquency)   { m_omega = friquency * m_2PI; }
    void set_phase (double phase)           { m_phi = phase * m_2PI; }
    void set_amplitude (double amplitude)   { m_a = amplitude; }

    double func (double x) const
    {
        return m_func ((x * m_omega) + m_phi) * m_a;
    }

private:
    double (*m_func) (double) = nullptr;

    const double m_2PI = 6.283185307179586476925286766559;

    double m_omega = m_2PI;
    double m_phi = 0.0;
    double m_a = 1.0;
};


/////////////////////////////////////////////////////////////////////
PlotData::PlotData () :
    m_data (new PrivateData ())
{}

PlotData::PlotData (PlotData::pPrivateData data) :
    m_data (data)
{}

PlotData::PlotData (const PlotData& data) :
    m_data (data.m_data)
{}

void PlotData::load_data(pPage page)
{
    std::vector<std::string> headers;
    page->get_headers (&headers);
    m_data->series.reserve (headers.size ());
    for (const std::string& header : headers)
    {
        m_data->series.push_back (Row (header));
        page->get_data (m_data->series.size () - 1, &m_data->series.back ());
    }
    std::sort (m_data->series.begin () + 1, m_data->series.end ());
}

void PlotData::save_data(pPage page) const
{
    for (Row row : m_data->series)
        page->push_data_back (row.get_name(), row);
}

PlotData& PlotData::operator= (const PlotData& plotData)
{
    m_data = plotData.m_data;
    return *this;
}

/*
void PlotData::set_data (QAxObject* StatSheet)
{
    QAxObject* cell (StatSheet->querySubObject ("Cells(QVariant,QVariant)", QVariant (1), QVariant (1)));
    QString info (cell->property ("Value").toString ());
    delete cell;

    bool noHeader (!is_text (info));

    cell = StatSheet->querySubObject ("Cells(Rows.Count,QVariant)", QVariant (1));
    QAxObject* numRowsEx = cell->querySubObject ("End (xlDown)");
    size_t dataSize = numRowsEx->property ("Row").toInt ();
    delete numRowsEx;
    delete cell;

    cell = StatSheet->querySubObject ("Cells(QVariant,Columns.Count)", QVariant (1));
    QAxObject* numColumnsEx = cell->querySubObject ("End (xlToRight)");
    size_t numColumns = numColumnsEx->property ("Column").toInt ();
    delete numColumnsEx;
    delete cell;

    if (!noHeader)
    {
        for (unsigned i = 1; i <= numColumns; ++i)
        {
            cell = StatSheet->querySubObject ("Cells(QVariant,QVariant)", QVariant (1), QVariant (i));
            get_series ().push_back (Row (cell->property ("Value").toString ().toStdString(), dataSize - 1, 0.0));
            delete cell;
        }
    }
    else
    {
        for (unsigned i = 1; i <= numColumns; ++i)
            get_series ().push_back (Row (QString::number (i).toStdString (), dataSize, 0.0));
    }

    QAxObject* Cell1 (StatSheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (noHeader ? 1 : 2), QVariant (1)));
    QAxObject* Cell2 (StatSheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (dataSize), QVariant (numColumns)));
    QAxObject* range (StatSheet->querySubObject ("Range(const QVariant&,const QVariant&)", Cell1->asVariant (), Cell2->asVariant ()));
    range->setProperty ("NumberFormat", QVariant ("Double"));

    QList<QVariant> list = range->property ("Value").toList ();
    for (unsigned row = 0; row < dataSize - (noHeader ? 0 : 1); ++row)
    {
        QList<QVariant> rowList (list[row].toList ());
        for (unsigned col = 0; col < numColumns; ++col)
            get_series ()[col][row] = rowList[col].toDouble ();
    }

    delete Cell1;
    delete Cell2;
    delete range;

    std::sort (get_series ().begin () + 1, get_series ().end ());
    set_name ("Source row");
}

void PlotData::save_data (QAxObject* sheet) const
{
    if (m_data->series.empty ()) return;

    QAxObject* Cell1 (sheet->querySubObject ("Cells(QVariant&,QVariant&)", QVariant (1), QVariant (1)));
    QAxObject* Cell2 (sheet->querySubObject ("Cells(QVariant&,QVariant&)",
        QVariant (m_data->series[0].size () + 1),
        QVariant (m_data->series.size ())
    ));
    QAxObject* range (sheet->querySubObject ("Range(const QVariant&,const QVariant&)",
        Cell1->asVariant (),
        Cell2->asVariant ()
    ));

    QList<QVariant> cellsList;
    QList<QVariant> rowsList;

    for (Row c : m_data->series) cellsList << QString (c.get_name ().c_str());
    rowsList << QVariant (cellsList);

    for (size_t i = 0; i < m_data->series[0].size (); i++)
    {
        cellsList.clear ();
        for (size_t j = 0; j < m_data->series.size (); j++) cellsList << m_data->series[j][i];
        rowsList << QVariant (cellsList);
    }

    range->setProperty ("Value", QVariant (rowsList));
}
*/

size_t PlotData::get_size () const { return m_data->series.size () - 1; }

bool PlotData::empty () const { return m_data->series.empty (); }

const std::string& PlotData::get_name () const
{
    return m_data->name;
}

void PlotData::set_name (const std::string& name)
{
    m_data->name = name;
}

StringList PlotData::get_headers () const
{
    StringList headers;
    for (auto i = m_data->series.begin () + 1; i < m_data->series.end (); ++i)
        headers.push_back(i->get_name ());
    return headers;
}


PlotData PlotData::get_approx () const
{
    pPrivateData newdata (std::make_shared<PrivateData> ());
    newdata->series.reserve (m_data->series.size ());
    newdata->series.push_back (Row (m_data->series[0].get_name (), 2));
    newdata->series[0][0] = m_data->series[0][0];
    newdata->series[0][1] = m_data->series[0][m_data->series[0].size () - 1];

    prepare_threads ();

    #pragma omp parallel for
    for (int numrow = 1; numrow < m_data->series.size (); ++numrow)
    {
        const Row& curve = m_data->series[numrow];
        const Row& axisX (m_data->series[0]);

        double sumX (0.0);
        double sumX2 (0.0);
        double sumY (0.0);
        double sumXY (0.0);

        size_t n (axisX.size ());

        for (unsigned i = 0; i < n; ++i)
        {
            double x (axisX[i]);
            double y (curve[i]);
            sumX += x;
            sumX2 += x * x;
            sumY += y;
            sumXY += x * y;
        }

        double a ((n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX));
        double b ((sumY - a * sumX) / n);

        newdata->series.push_back (Row (curve.get_name (), 2));
        Row& newRow = newdata->series.back ();

        newRow.push_back (a * (*axisX.begin ()) +  b);
        newRow.push_back (a * (*axisX.rbegin ()) + b);
    }

    return newdata;
}

PlotData PlotData::get_smoothing (int points) const
{
    pPrivateData d (std::make_shared<PrivateData> ());
    d->series.reserve (m_data->series.size ());
    d->series.push_back(m_data->series[0]);

    prepare_threads ();

    #pragma omp parallel for
    for (int numrow = 1; numrow < m_data->series.size (); ++numrow)
    {
        d->series.push_back (Row (m_data->series[numrow].get_name (), m_data->series[numrow].size (), 0.0));
        Row& row (d->series[numrow]);
        int branch (points / 2);
        int fullpoints (branch * 2 + 1);

        for (unsigned i = 0; i < row.size (); ++i)
        {
            size_t points (fullpoints);
            for (int j = 0; j < fullpoints; ++j)
            {
                int offset (i + j - branch);
                if (offset < 0 || offset >= row.size ())
                {
                    --points;
                    continue;
                }

                row[i] += m_data->series[numrow][offset];
            }
            row[i] /= points;
        }
    }

    char buf[10] = { '\0' };
    d->name = m_data->name + " smoothed by " + itoa (points, buf, 10);

    return d;
}

PlotData PlotData::get_deviations () const
{
    pPrivateData d (std::make_shared<PrivateData> ());
    d->series.reserve (m_data->series.size ());
    d->series.push_back (m_data->series[0]);

    prepare_threads ();

    #pragma omp parallel for
    for (int col = 1; col < m_data->series.size (); ++col)
    {
        double medium (0.0);

        for (unsigned i = 0; i < m_data->series[col].size (); ++i)
            medium += m_data->series[col][i];
        medium /= m_data->series[col].size ();

        d->series.push_back (Row (m_data->series[col].get_name (), m_data->series[0].size (), 0.0));
        for (unsigned i = 0; i < m_data->series[col].size (); ++i)
            d->series[col][i] = m_data->series[col][i] - medium;
    }
    d->name = "Deviations row";

    return d;
}


///////////////////////////////////////////////////////////
void PlotData::add_row (const Row& col)
{
    if (find_column (col.get_name ()) != m_data->series.end ())
        return;
    m_data->series.push_back (col);
    std::sort (m_data->series.begin () + 1, m_data->series.end ());
}


//////////////////////////////////////////////
const Row& PlotData::get_axis () const
{
    if (m_data->series.empty ())
        throw std::string ("PlotData is empty!");
    return m_data->series[0];
}

const Row* PlotData::get_row (const std::string& col) const
{
    auto it = find_column (col);

    if (it != m_data->series.end ())
        return &(*it);
    else
        return nullptr;
}


/////////////////////////////////////////////////
void PlotData::remove_spaces ()
{
    double tstep (m_data->series[0][1] - m_data->series[0][0]);
    double cstep (tstep * 1.000001);

    for (unsigned i = 1; i < m_data->series[0].size (); ++i)
    {
        double distance (m_data->series[0][i] - m_data->series[0][i - 1]);
        if (distance > cstep)
            for (int numsteps = (distance + 0.000001) / cstep; numsteps > 1; --numsteps)
            {
                for (unsigned row = 0; row < m_data->series.size (); ++row)
                    m_data->series[row].insert (m_data->series[row].begin () + i, (m_data->series[row][i + 1] - m_data->series[row][i]) / numsteps);
                ++i;
            }
    }
}


////////////////////////////////////////////////////

std::vector<Row>::iterator PlotData::find_column (const std::string& col) const
{
    if (m_data->series.empty ())
        return m_data->series.end ();
    else
        return quick_search (m_data->series.begin () + 1, m_data->series.end (), col);
}

PlotData::iterRow PlotData::quick_search(const iterRow& begin, const iterRow& end, const std::string& col) const
{
    if (begin == end)
        return begin;

    const iterRow mid (begin + std::distance(begin, end) / 2);

    if (mid->get_name () > col)
        return quick_search (begin, mid, col);
    else if (mid->get_name () < col)
        return quick_search (mid, end, col);
    else
        return mid;
}



//////////////////////////////////////////////////////

PlotData PlotData::get_relative_sp (double from_lin, double to_lin, unsigned numstep) const
{
    if (from_lin > to_lin)
    {
        double tmp (from_lin);
        from_lin = to_lin;
        to_lin = tmp;
    }

    double from (log10 (from_lin));
    double to (log10 (to_lin));
    double step ((to - from) / (numstep - 1));
    double max (to + step / 4.0);

    pPrivateData sp (std::make_shared<PrivateData> ());
    sp->series.reserve (m_data->series.size () * m_data->series.size ());
    sp->series.push_back (Row ("Friquency"));
    Row& friquency (sp->series[0]);

    while (from <= max)
    {
        friquency.push_back (pow (10.0, from));
        from += step;
    }

    const size_t count (m_data->series[0].size ());

    prepare_threads ();

    for (unsigned rowFst = 1; rowFst < m_data->series.size (); ++rowFst)
    for (unsigned rowSnd = rowFst; rowSnd < m_data->series.size (); ++rowSnd)
    {
        std::string f (m_data->series[rowFst].get_name ());
        std::string s (m_data->series[rowSnd].get_name ());
        std::string name ((f == s) ? f : (f + "-" + s));

        sp->series.push_back (Row (name, sp->series[0].size (), 0.0));
        Row& row (sp->series[sp->series.size () - 1]);
        const std::vector<double>& fRow (m_data->series[rowFst]);
        const std::vector<double>& sRow (m_data->series[rowSnd]);

        #pragma omp parallel for
        for (int i = 0; i < sp->series[0].size (); ++i)
        {
            Function fSin;
            Function fCos;
            fSin.set_friquency (sp->series[0][i]);
            fCos.set_friquency (sp->series[0][i]);
            fSin.set_func (&sin);
            fCos.set_func (&cos);

            double a (0.0);
            double b (0.0);
            double c (0.0);
            double d (0.0);

            for (unsigned j = 0; j < count; ++j)
            {
                a += fRow[j] * fSin (m_data->series[0][j]);
                b += fRow[j] * fCos (m_data->series[0][j]);
            }

            if (rowFst != rowSnd)
            {
                for (unsigned j = 0; j < count; ++j)
                {
                    c += sRow[j] * fSin (m_data->series[0][j]);
                    d += sRow[j] * fCos (m_data->series[0][j]);
                }
            }
            else
            {
                c = a;
                d = b;
            }

            row[i] = (a * c + b * d) / (count * count);
        }
    }
    sp->name = "Spectr";
    std::sort (sp->series.begin () + 1, sp->series.end ());
    return sp;
}

PlotData PlotData::get_correlations (double from_lin, double to_lin, unsigned numstep) const
{
    if (from_lin > to_lin)
    {
        double tmp (from_lin);
        from_lin = to_lin;
        to_lin = tmp;
    }

    double from (log10 (from_lin));
    double to (log10 (to_lin));
    double step ((to - from) / (numstep - 1));
    double max (to + step / 4.0);

    pPrivateData corr (std::make_shared<PrivateData> ());
    corr->series.reserve ((m_data->series.size () - 1) * (m_data->series.size () - 1) / 2);
    corr->series.push_back (Row ("Friquency"));
    Row& friquency (corr->series[0]);

    while (from <= max)
    {
        friquency.push_back (pow (10.0, from));
        from += step;
    }

    const size_t count (m_data->series[0].size ());

    for (unsigned rowFst = 1; rowFst < m_data->series.size (); ++rowFst)
    {
        for (unsigned rowSnd = rowFst + 1; rowSnd < m_data->series.size (); ++rowSnd)
        {
            const std::vector<double>& fRow (m_data->series[rowFst]);
            const std::vector<double>& sRow (m_data->series[rowSnd]);

            corr->series.push_back (Row (
                m_data->series[rowFst].get_name () + "-" + m_data->series[rowSnd].get_name (),
                corr->series[0].size (),
                0.0
            ));
            std::vector<double>& row (corr->series[1]);

            #pragma omp parallel for
            for (int i = 0; i < corr->series[0].size (); ++i)
            {
                Function fSin;
                Function fCos;
                fSin.set_friquency (corr->series[0][i]);
                fCos.set_friquency (corr->series[0][i]);
                fSin.set_func (&sin);
                fCos.set_func (&cos);

                double a (0.0);
                double b (0.0);
                double c (0.0);
                double d (0.0);

                for (unsigned j = 0; j < count; ++j)
                {
                    a += fRow[j] * fSin (m_data->series[0][j]);
                    b += fRow[j] * fCos (m_data->series[0][j]);

                    c += sRow[j] * fSin (m_data->series[0][j]);
                    d += sRow[j] * fCos (m_data->series[0][j]);
                }

                row[i] = ((a * c + b * d) / (count * count)) /
                    sqrt (((a * a + b * b) / (count * count)) * ((c * c + d * d) / (count * count)));
            }
        }
    }
    corr->name = "Correlation";
    std::sort (corr->series.begin () + 1, corr->series.end ());
    return corr;
}

PlotData PlotData::get_power () const
{
    pPrivateData pow (std::make_shared<PrivateData> ());

    pow->series = m_data->series;

    for (int i = 1; i < pow->series.size (); ++i)
        for (size_t j = 0; j < pow->series[0].size (); ++j)
            pow->series[i][j] *= pow->series[0][j];

    return pow;
}
