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

#include <djvViewApp/FileSettingsWidget.h>

#include <djvViewApp/FileSettings.h>

#include <djvUI/CheckBox.h>
#include <djvUI/RowLayout.h>
#include <djvUI/SettingsSystem.h>

#include <djvCore/Context.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewApp
    {
        struct SequenceSettingsWidget::Private
        {
            std::shared_ptr<UI::CheckBox> checkBox;
            std::shared_ptr<UI::VerticalLayout> layout;
            std::shared_ptr<ValueObserver<bool> > observer;
        };

        void SequenceSettingsWidget::_init(const std::shared_ptr<Context>& context)
        {
            ISettingsWidget::_init(context);

            DJV_PRIVATE_PTR();
            setClassName("djv::ViewApp::SequenceSettingsWidget");

            p.checkBox = UI::CheckBox::create(context);

            p.layout = UI::VerticalLayout::create(context);
            p.layout->addChild(p.checkBox);
            addChild(p.layout);

            auto contextWeak = std::weak_ptr<Context>(context);
            auto weak = std::weak_ptr<SequenceSettingsWidget>(std::dynamic_pointer_cast<SequenceSettingsWidget>(shared_from_this()));
            p.checkBox->setCheckedCallback(
                [weak, contextWeak](bool value)
            {
                if (auto context = contextWeak.lock())
                {
                    if (auto widget = weak.lock())
                    {
                        auto settingsSystem = context->getSystemT<UI::Settings::System>();
                        if (auto fileSettings = settingsSystem->getSettingsT<FileSettings>())
                        {
                            fileSettings->setAutoDetectSequences(value);
                        }
                    }
                }
            });

            auto settingsSystem = context->getSystemT<UI::Settings::System>();
            if (auto fileSettings = settingsSystem->getSettingsT<FileSettings>())
            {
                p.observer = ValueObserver<bool>::create(
                    fileSettings->observeAutoDetectSequences(),
                    [weak](bool value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->checkBox->setChecked(value);
                    }
                });
            }
        }

        SequenceSettingsWidget::SequenceSettingsWidget() :
            _p(new Private)
        {}

        std::shared_ptr<SequenceSettingsWidget> SequenceSettingsWidget::create(const std::shared_ptr<Context>& context)
        {
            auto out = std::shared_ptr<SequenceSettingsWidget>(new SequenceSettingsWidget);
            out->_init(context);
            return out;
        }

        std::string SequenceSettingsWidget::getSettingsName() const
        {
            return DJV_TEXT("settings_file_sequences");
        }

        std::string SequenceSettingsWidget::getSettingsGroup() const
        {
            return DJV_TEXT("settings_title_file");
        }

        std::string SequenceSettingsWidget::getSettingsSortKey() const
        {
            return "A";
        }

        void SequenceSettingsWidget::_initEvent(Event::Init& event)
        {
            ISettingsWidget::_initEvent(event);
            DJV_PRIVATE_PTR();
            p.checkBox->setText(_getText(DJV_TEXT("settings_file_auto-detect_file_sequences")));
        }

    } // namespace ViewApp
} // namespace djv

