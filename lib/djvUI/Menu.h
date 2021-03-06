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

#include <djvUI/Style.h>

#include <djvCore/IObject.h>
#include <djvCore/ValueObserver.h>

namespace djv
{
    namespace UI
    {
        class Action;
        class Widget;

        namespace Button
        {
            class Menu;
        
        } // namespace Button

        //! This class provides a popup menu widget.
        class Menu : public Core::IObject
        {
            DJV_NON_COPYABLE(Menu);

        protected:
            void _init(const std::shared_ptr<Core::Context>&);
            Menu();

        public:
            virtual ~Menu();

            static std::shared_ptr<Menu> create(const std::shared_ptr<Core::Context>&);

            std::shared_ptr<Core::IValueSubject<std::string> > observeIcon() const;
            std::shared_ptr<Core::IValueSubject<std::string> > observeText() const;
            void setIcon(const std::string &);
            void setText(const std::string &);

            void addAction(const std::shared_ptr<Action> &);
            void removeAction(const std::shared_ptr<Action> &);
            void clearActions();
            void addSeparator();

            MetricsRole getMinimumSizeRole() const;
            void setMinimumSizeRole(MetricsRole);

            ColorRole getBackgroundRole() const;
            void setBackgroundRole(ColorRole);

            std::shared_ptr<Widget> popup(const glm::vec2 &);
            std::shared_ptr<Widget> popup(const std::weak_ptr<Button::Menu> & button);
            std::shared_ptr<Widget> popup(const std::weak_ptr<Button::Menu> & button, const std::weak_ptr<Widget> & anchor);

            bool isOpen() const;
            void close();

            void setCloseCallback(const std::function<void(void)> &);

        private:
            void _createWidgets();

            DJV_PRIVATE();
        };

    } // namespace UI
} // namespace djv

