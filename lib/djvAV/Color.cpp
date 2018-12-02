//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvAV/Color.h>

namespace djv
{
    namespace AV
    {
        namespace Image
        {
            Color::Color()
            {}

            Color::Color(Pixel pixel) :
                _pixel(pixel),
                _data(getByteCount(pixel))
            {
                zero();
            }

            Color::Color(U8_T r, U8_T g, U8_T b, U8_T a) :
                _pixel(Pixel::RGBA_U8),
                _data(getByteCount(_pixel))
            {
                _data[0] = r;
                _data[1] = g;
                _data[2] = b;
                _data[3] = a;
            }

            Color::Color(F32_T r, F32_T g, F32_T b, F32_T a) :
                _pixel(Pixel::RGBA_F32),
                _data(getByteCount(_pixel))
            {
                float * p = reinterpret_cast<float *>(_data.data());
                p[0] = r;
                p[1] = g;
                p[2] = b;
                p[3] = a;
            }

            void Color::zero()
            {
                memset(_data.data(), 0, getByteCount(_pixel));
            }

            Color Color::convert(const Pixel & pixel) const
            {
                auto out = Color(pixel);
                Image::convert(_data.data(), _pixel, out._data.data(), pixel, 1);
                return out;
            }

        } // namespace Image
    } // namespace AV
} // namespace djv

