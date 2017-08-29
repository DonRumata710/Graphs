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


#include "waveletstep.h"
#include "intervals.h"

#include <vector>
#include <complex>
#include <string>


class WaveletFunction
{
public:
    virtual ~WaveletFunction () {}

    virtual Intervals compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const = 0;
    virtual WaveletStep get_step () const = 0;
    virtual std::string get_name () const = 0;
};


class HaarWavelet final : public WaveletFunction
{
public:
    HaarWavelet (const size_t min_points, const size_t max_points);
    virtual Intervals compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const override;
    virtual WaveletStep get_step () const override { return WaveletStep::TWO; }
    virtual std::string get_name () const override { return "HAAF"; }

private:
    const size_t m_min_points;
    const size_t m_max_points;
};


class FhatWavelet final : public WaveletFunction
{
public:
    FhatWavelet (const size_t min_points, const size_t max_points);
    virtual Intervals compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const override;
    virtual WaveletStep get_step () const override { return WaveletStep::THREE; }
    virtual std::string get_name () const override { return "FHAT"; }

private:
    const size_t m_min_points;
    const size_t m_max_points;
};


class MorletWavelet final : public WaveletFunction
{
public:
    MorletWavelet (size_t num_periods);
    virtual Intervals compute (const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& wavelet, size_t& num_columns, size_t& min_points, size_t& max_points) const override;
    virtual WaveletStep get_step () const override { return WaveletStep::ONE; }
    virtual std::string get_name () const override { return "MORLET"; }

private:
    void m_calc_morlet_factors (const int num_points, std::vector<std::complex<double>>& data) const;

    const size_t m_num_periods;
    const size_t m_min_points;
};
