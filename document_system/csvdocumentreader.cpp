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

#include "csvdocumentreader.h"


ADD_DOCUMENT_READER(CsvDocumentReader, "csv")


pDocumentReader CsvDocumentReader::create(const std::string& filename)
{
    return pDocumentReader(new CsvDocumentReader (filename));
}

AxisType CsvDocumentReader::get_x_axis_type()
{
    return TYPE_NUM;
}

size_t CsvDocumentReader::get_columns_number()
{
    return 0;
}

size_t CsvDocumentReader::get_rows_number()
{
    return 0;
}

std::vector<std::string> CsvDocumentReader::get_headers()
{
    return std::vector<std::string> ();
}

double CsvDocumentReader::get_item(size_t row, size_t column)
{
    return 0.0;
}

CsvDocumentReader::CsvDocumentReader(const std::string& filename)
{

}
