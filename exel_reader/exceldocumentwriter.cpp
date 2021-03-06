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


#include "exceldocumentwriter.h"
#include "excelpage.h"

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>

#include <memory>


struct ExcelDocumentWriter::PrivateData
{
    std::shared_ptr<ExcelFile> file;

    PrivateData (const std::string& filename) :
        file (std::make_shared<ExcelFile> ())
    {
        file->create_file (filename);
    }
};


pDocumentWriter ExcelDocumentWriter::create(const std::string& filename)
{
    return pDocumentWriter (new ExcelDocumentWriter (filename));
}

ExcelDocumentWriter::~ExcelDocumentWriter ()
{
    if (data)
    {
        data->file->save();
        delete data;
    }
    data = nullptr;
}

pPage ExcelDocumentWriter::get_page(const std::string& name) const
{
    return std::unique_ptr<ExcelPage> (new ExcelPage (data->file, data->file->create_page (name)));
}

ExcelDocumentWriter::ExcelDocumentWriter(const std::string& filename) :
    data (new PrivateData (filename))
{}
