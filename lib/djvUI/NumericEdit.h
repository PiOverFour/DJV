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

#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        //! This class provides buttons for incrementing/decrementing a value.
        class NumericEditButtons : public Widget
        {
            DJV_NON_COPYABLE(NumericEditButtons);

        protected:
            void _init(const std::shared_ptr<Core::Context>&);
            NumericEditButtons();

        public:
            virtual ~NumericEditButtons();

            static std::shared_ptr<NumericEditButtons> create(const std::shared_ptr<Core::Context>&);

            void setIncrementEnabled(bool);
            void setDecrementEnabled(bool);

            void setIncrementCallback(const std::function<void(void)>&);
            void setDecrementCallback(const std::function<void(void)>&);

        protected:
            void _preLayoutEvent(Core::Event::PreLayout&) override;
            void _layoutEvent(Core::Event::Layout&) override;

        private:
            DJV_PRIVATE();
        };

        //! This class provides the base functionality for numeric editor widgets.
        class NumericEdit : public Widget
        {
            DJV_NON_COPYABLE(NumericEdit);

        protected:
            void _init(const std::shared_ptr<Core::Context>&);
            NumericEdit();

        public:
            virtual ~NumericEdit() = 0;

        protected:
            virtual void _textEdit(const std::string&, TextEditReason) = 0;
            virtual bool _keyPress(int) = 0;
            virtual void _scroll(float) = 0;
            virtual void _incrementValue() = 0;
            virtual void _decrementValue() = 0;
            void _textUpdate(const std::string&, const std::string&);
            void _setIsMin(bool);
            void _setIsMax(bool);

            void _preLayoutEvent(Core::Event::PreLayout &) override;
            void _layoutEvent(Core::Event::Layout &) override;
            void _paintEvent(Core::Event::Paint&) override;
            void _keyPressEvent(Core::Event::KeyPress&) override;
            void _scrollEvent(Core::Event::Scroll&) override;

        private:
            DJV_PRIVATE();
        };

    } // namespace UI
} // namespace djv
