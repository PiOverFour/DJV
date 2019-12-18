//------------------------------------------------------------------------------
// Copyright (c) 2019 Darby Johnston
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

#include <djvScene/IPrimitive.h>

#include <djvAV/PointList.h>

namespace djv
{
    namespace Scene
    {
        //! This class provides a poly-line primitive.
        class PolyLinePrimitive : public IPrimitive
        {
            DJV_NON_COPYABLE(PolyLinePrimitive);

        protected:
            PolyLinePrimitive();

        public:
            static std::shared_ptr<PolyLinePrimitive> create();

            const std::vector<AV::Geom::PointList>& getPointLists() const;
            void setPointLists(const std::vector<AV::Geom::PointList>&);
            void addPointList(const AV::Geom::PointList&);

            std::string getClassName() const override;
            bool isShaded() const override;
            void render(const glm::mat4x4&, const std::shared_ptr<AV::Render3D::Render>&) override;
            size_t getPointCount() const override;

        private:
            std::vector<AV::Geom::PointList> _pointLists;
            size_t _pointCount = 0;
        };

    } // namespace Scene
} // namespace djv

#include <djvScene/PolyLinePrimitiveInline.h>