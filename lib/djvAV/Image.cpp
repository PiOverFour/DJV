//------------------------------------------------------------------------------
// Copyright (c) 2004-2020 Darby Johnston
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

#include <djvAV/Image.h>

namespace djv
{
    namespace AV
    {
        namespace Image
        {
            void Image::_init(const Info & value, const std::shared_ptr<Core::FileSystem::FileIO>& io)
            {
                Data::_init(value, io);
            }

            Image::Image()
            {}

            Image::~Image()
            {}

#if defined(DJV_MMAP)
            std::shared_ptr<Image> Image::create(const Info& value, const std::shared_ptr<Core::FileSystem::FileIO>& io)
            {
                auto out = std::shared_ptr<Image>(new Image);
                out->_init(value, io);
                return out;
            }
#else // DJV_MMAP
            std::shared_ptr<Image> Image::create(const Info & value)
            {
                auto out = std::shared_ptr<Image>(new Image);
                out->_init(value, nullptr);
                return out;
            }
#endif // DJV_MMAP

            const std::string& Image::getPluginName() const
            {
                return _pluginName;
            }

            void Image::setPluginName(const std::string& value)
            {
                _pluginName = value;
            }

            const Tags& Image::getTags() const
            {
                return _tags;
            }

            void Image::setTags(const Tags& value)
            {
                _tags = value;
            }

        } // namespace Image
    } // namespace AV
} // namespace djv

