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

#include <djvAV/ImageData.h>
#include <djvAV/OpenGL.h>

#include <djvCore/Range.h>
#include <djvCore/UID.h>

namespace djv
{
    namespace AV
    {
        namespace Render
        {
            //! This struct provides information about a texture atlas item.
            struct TextureAtlasItem
            {
                uint16_t w = 0;
                uint16_t h = 0;
                uint8_t textureIndex = 0;
                Core::FloatRange textureU;
                Core::FloatRange textureV;
            };

            //! This class provides a texture atlas.
            class TextureAtlas
            {
                DJV_NON_COPYABLE(TextureAtlas);

            public:
                TextureAtlas(uint8_t textureCount, uint16_t textureSize, Image::Type, GLenum filter = GL_LINEAR, uint8_t border = 1);
                ~TextureAtlas();

                uint8_t getTextureCount() const;
                uint16_t getTextureSize() const;
                Image::Type getTextureType() const;
                std::vector<GLuint> getTextures() const;

                bool getItem(Core::UID, TextureAtlasItem &);
                Core::UID addItem(const std::shared_ptr<Image::Data> &, TextureAtlasItem &);

                float getPercentageUsed() const;

            private:
                class BoxPackingNode;

                void _getAllNodes(
                    const std::shared_ptr<BoxPackingNode>&,
                    std::vector<std::shared_ptr<BoxPackingNode> >&);
                void _getLeafNodes(
                    const std::shared_ptr<BoxPackingNode>&,
                    std::vector<std::shared_ptr<BoxPackingNode> >&) const;
                void _toTextureAtlasItem(
                    const std::shared_ptr<BoxPackingNode>&,
                    TextureAtlasItem&);
                void _removeFromAtlas(const std::shared_ptr<BoxPackingNode>&);

                DJV_PRIVATE();
            };

        } // namespace Render
    } // namespace AV
} // namespace djv
