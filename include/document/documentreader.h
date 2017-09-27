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

#pragma once
#ifndef DOCUMENT_READER_H
#define DOCUMENT_READER_H


#include "axistype.h"

#include <vector>
#include <string>


class iDocumentReader
{
public:
    virtual ~iDocumentReader () {}
    virtual pPage get_page (const std::string& name) const = 0;

protected:
    static AxisType get_str_type (const std::string& str)
    {
        auto s (str.begin ());
        unsigned i (1);
        char sep = '\n';
        int lng[5] = { 0 };
        bool lettersLong (false);
        bool letters (false);
        if (!isdigit(*s)) return TYPE_NUM;
        ++s;
        //if (!(*s++).isDigit ())
        while (s != str.end ())
        {
            if (isdigit(*s)) i++;
            else
            {
                if (isalpha(*s))
                {
                    if (letters && lettersLong)
                        return TYPE_NUM;
                    else
                    {
                        unsigned j (0);
                        while (isalpha(*s) && s++ != str.end ())
                            ++j;
                        if (j > 2 && !lettersLong)
                            lettersLong = TYPE_TIME;
                        else if (!letters)
                            letters = TYPE_TIME;
                        else
                            return TYPE_NUM;
                    }
                }
                else if (sep == *s || *s == ',') sep = '\n';
                else if (sep == '\n') sep = *s;
                else return TYPE_NUM;
                if (i > 4) return TYPE_NUM;
                ++lng[i];
                i = 0;
            }
            ++s;
        }
        if (lng[4] < 2 && lng[2] != 0 &&
            ((lng[1] + lng[2] + lng[4] > 1 && lettersLong && lng[1] + lng[2] + lng[4] < 5) ||
            (lng[1] + lng[2] + lng[4] > 2 && !lettersLong && lng[1] + lng[2] + lng[4] < 6)))
            return TYPE_TIME;
        return TYPE_NUM;
    }

    static bool is_text (const std::string& str)
    {
        return !(isdigit (*str.begin ()) && (isdigit (*(str.end () - 1))));
    }
};

typedef iDocumentReader* pDocumentReader;


#endif // DOCUMENT_H
