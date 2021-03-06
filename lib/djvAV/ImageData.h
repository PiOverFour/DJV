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

#pragma once

#include <djvAV/Pixel.h>

#include <djvCore/Memory.h>
#include <djvCore/PicoJSON.h>
#include <djvCore/UID.h>

#include <memory>

namespace djv
{
    namespace Core
    {
        namespace FileSystem
        {
            class FileIO;
    
        } // namespace FileSystem
    } // namespace Core

    namespace AV
    {
        namespace Image
        {
            //! This struct provides information about mirroring the image.
            class Mirror
            {
            public:
                constexpr Mirror();
                constexpr Mirror(bool x, bool y);

                bool x = false;
                bool y = false;

                constexpr bool operator == (const Mirror&) const;
                constexpr bool operator != (const Mirror&) const;
            };

            //! This struct provides information about the data layout.
            class Layout
            {
            public:
                Layout();
                constexpr Layout(const Mirror&, GLint alignment = 1, Core::Memory::Endian = Core::Memory::getEndian());

                Mirror mirror;
                GLint alignment = 1;
                Core::Memory::Endian endian = Core::Memory::getEndian();

                constexpr bool operator == (const Layout&) const;
                constexpr bool operator != (const Layout&) const;
            };

            //! This struct provides the image size.
            class Size
            {
            public:
                Size(uint16_t w = 0, uint16_t h = 0);

                uint16_t w = 0;
                uint16_t h = 0;
                
                float getAspectRatio() const;

                bool operator == (const Size&) const;
                bool operator != (const Size&) const;
            };

            //! This constant provides the default name.
            const std::string nameDefault = "Default";
            
            //! This struct provides image data information.
            class Info
            {
            public:
                Info();
                Info(const Size&, Type, const Layout& = Layout());
                Info(uint16_t width, uint16_t height, Type, const Layout& = Layout());

                std::string name;
                Size size;
                float pixelAspectRatio = 1.F;
                Type type = Type::None;
                Layout layout;

                float getAspectRatio() const;
                GLenum getGLFormat() const;
                GLenum getGLType() const;
                bool isValid() const;
                size_t getPixelByteCount() const;
                size_t getScanlineByteCount() const;
                size_t getDataByteCount() const;

                bool operator == (const Info&) const;
                bool operator != (const Info&) const;
            };

            //! This struct provides image data.
            class Data
            {
                DJV_NON_COPYABLE(Data);

            protected:
                void _init(const Info&, const std::shared_ptr<Core::FileSystem::FileIO>&);
                Data();

            public:
                ~Data();

#if defined(DJV_MMAP)
                static std::shared_ptr<Data> create(const Info&, const std::shared_ptr<Core::FileSystem::FileIO>& = nullptr);
#else // DJV_MMAP
                static std::shared_ptr<Data> create(const Info&);
#endif // DJV_MMAP

                Core::UID getUID() const;

                const Info& getInfo() const;
                const Size& getSize() const;
                uint16_t getWidth() const;
                uint16_t getHeight() const;
                float getAspectRatio() const;

                Type getType() const;
                GLenum getGLFormat() const;
                GLenum getGLType() const;
                const Layout& getLayout() const;

                bool isValid() const;
                uint8_t getPixelByteCount() const;
                size_t getScanlineByteCount() const;
                size_t getDataByteCount() const;

                const uint8_t* getData() const;
                const uint8_t* getData(uint16_t y) const;
                const uint8_t* getData(uint16_t x, uint16_t y) const;
                uint8_t* getData();
                uint8_t* getData(uint16_t y);
                uint8_t* getData(uint16_t x, uint16_t y);

                void zero();

#if defined(DJV_MMAP)
                void detach();
#endif // DJV_MMAP

                bool operator == (const Data&) const;
                bool operator != (const Data&) const;

            private:
                Core::UID _uid = 0;
                Info _info;
                uint8_t _pixelByteCount = 0;
                size_t _scanlineByteCount = 0;
                size_t _dataByteCount = 0;
                uint8_t* _data = nullptr;
                const uint8_t* _p = nullptr;
#if defined(DJV_MMAP)
                std::shared_ptr<Core::FileSystem::FileIO> _fileIO;
#endif // DJV_MMAP
            };

        } // namespace Image
    } // namespace AV

    picojson::value toJSON(const AV::Image::Size&);

    //! Throws:
    //! - std::exception
    void fromJSON(const picojson::value&, AV::Image::Size&);

    std::ostream& operator << (std::ostream&, const AV::Image::Size&);
    std::istream& operator >> (std::istream&, AV::Image::Size&);

} // namespace djv

#include <djvAV/ImageDataInline.h>
