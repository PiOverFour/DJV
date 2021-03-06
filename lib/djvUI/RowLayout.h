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

#include <djvUI/Margin.h>
#include <djvUI/Spacing.h>
#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        namespace Layout
        {
            //! This enumeration provides how widgets use space in a layout.
            enum class RowStretch
            {
                None,
                Expand,

                Count,
                First = None
            };
            DJV_ENUM_HELPERS(RowStretch);

            //! This class provides a layout that arranges it's children in a row.
            class Row : public Widget
            {
                DJV_NON_COPYABLE(Row);

            protected:
                void _init(Orientation, const std::shared_ptr<Core::Context>&);
                Row();

            public:
                virtual ~Row();

                static std::shared_ptr<Row> create(Orientation, const std::shared_ptr<Core::Context>&);

                void addSeparator();
                void addSpacer();
                void addExpander();

                Orientation getOrientation() const;
                void setOrientation(Orientation);

                const Spacing & getSpacing() const;
                void setSpacing(const Spacing &);

                RowStretch getStretch(const std::shared_ptr<Widget> &) const;
                void setStretch(const std::shared_ptr<Widget> &, RowStretch);

                float getHeightForWidth(float) const override;

                void addChild(const std::shared_ptr<IObject> &) override;
                void removeChild(const std::shared_ptr<IObject> &) override;

            protected:
                void _preLayoutEvent(Core::Event::PreLayout &) override;
                void _layoutEvent(Core::Event::Layout &) override;

            private:
                DJV_PRIVATE();
            };

            //! This class provides a layout that arranges it's children in a horizontal row.
            class Horizontal : public Row
            {
                DJV_NON_COPYABLE(Horizontal);

            protected:
                void _init(const std::shared_ptr<Core::Context>&);
                Horizontal();

            public:
                static std::shared_ptr<Horizontal> create(const std::shared_ptr<Core::Context>&);
            };

            //! This class provides a layout that arranges it's children in a vertical row.
            class Vertical : public Row
            {
                DJV_NON_COPYABLE(Vertical);

            protected:
                void _init(const std::shared_ptr<Core::Context>&);
                Vertical();

            public:
                static std::shared_ptr<Vertical> create(const std::shared_ptr<Core::Context>&);
            };

        } // namespace Layout

        typedef Layout::Horizontal HorizontalLayout;
        typedef Layout::Vertical VerticalLayout;
        using Layout::RowStretch;

    } // namespace UI

    DJV_ENUM_SERIALIZE_HELPERS(UI::Layout::RowStretch);
    DJV_ENUM_SERIALIZE_HELPERS(UI::Layout::RowStretch);

} // namespace Gp
