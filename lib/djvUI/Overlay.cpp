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

#include <djvUI/Overlay.h>

#include <djvUI/Action.h>
#include <djvUI/StackLayout.h>

#include <djvCore/Animation.h>
#include <djvCore/Timer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        namespace Layout
        {
            struct Overlay::Private
            {
                bool capturePointer = true;
                bool captureKeyboard = true;
                std::weak_ptr<Widget> anchor;
                bool fadeIn = true;
                bool closeOnEscape = true;
                std::shared_ptr<Animation::Animation> fadeAnimation;
                std::map<Event::PointerID, bool> pressedIDs;
                std::function<void(void)> closeCallback;
            };

            void Overlay::_init(const std::shared_ptr<Core::Context>& context)
            {
                Widget::_init(context);
                DJV_PRIVATE_PTR();

                setClassName("djv::UI::Layout::Overlay");
                setBackgroundRole(ColorRole::Overlay);
                setPointerEnabled(true);

                p.fadeAnimation = Animation::Animation::create(context);
            }

            Overlay::Overlay() :
                _p(new Private)
            {}

            Overlay::~Overlay()
            {}

            std::shared_ptr<Overlay> Overlay::create(const std::shared_ptr<Core::Context>& context)
            {
                auto out = std::shared_ptr<Overlay>(new Overlay);
                out->_init(context);
                return out;
            }

            bool Overlay::hasCapturePointer() const
            {
                return _p->capturePointer;
            }

            bool Overlay::hasCaptureKeyboard() const
            {
                return _p->captureKeyboard;
            }

            void Overlay::setCapturePointer(bool value)
            {
                _p->capturePointer = value;
                setPointerEnabled(value);
            }

            void Overlay::setCaptureKeyboard(bool value)
            {
                _p->captureKeyboard = value;
            }

            const std::weak_ptr<Widget> & Overlay::getAnchor() const
            {
                return _p->anchor;
            }

            void Overlay::setAnchor(const std::weak_ptr<Widget> & value)
            {
                _p->anchor = value;
            }

            bool Overlay::hasFadeIn() const
            {
                return _p->fadeIn;
            }

            void Overlay::setFadeIn(bool value)
            {
                _p->fadeIn = value;
            }

            void Overlay::setCloseCallback(const std::function<void(void)> & callback)
            {
                _p->closeCallback = callback;
            }

            void Overlay::setVisible(bool value)
            {
                Widget::setVisible(value);
                DJV_PRIVATE_PTR();
                if (p.fadeIn && p.fadeAnimation)
                {
                    setOpacity(0.F);
                    if (value)
                    {
                        auto weak = std::weak_ptr<Overlay>(std::dynamic_pointer_cast<Overlay>(shared_from_this()));
                        p.fadeAnimation->start(
                            getOpacity(),
                            1.F,
                            std::chrono::milliseconds(100),
                            [weak](float value)
                        {
                            if (auto widget = weak.lock())
                            {
                                widget->setOpacity(value);
                            }
                        },
                            [weak](float value)
                        {
                            if (auto widget = weak.lock())
                            {
                                widget->setOpacity(value);
                            }
                        });
                    }
                }
                else
                {
                    setOpacity(1.F);
                }
            }

            float Overlay::getHeightForWidth(float value) const
            {
                const auto& style = _getStyle();
                const glm::vec2 m = getMargin().getSize(style);
                float out = Stack::heightForWidth(value - m.x, getChildWidgets(), Margin(), style) + m.y;
                return out;
            }

            void Overlay::_preLayoutEvent(Event::PreLayout & event)
            {
                const auto& style = _getStyle();
                _setMinimumSize(Stack::minimumSize(getChildWidgets(), Margin(), style) + getMargin().getSize(style));
            }

            void Overlay::_layoutEvent(Event::Layout & event)
            {
                const auto& style = _getStyle();
                Stack::layout(getMargin().bbox(getGeometry(), style), getChildWidgets(), Margin(), style);
            }

            void Overlay::_pointerEnterEvent(Core::Event::PointerEnter & event)
            {
                if (_p->capturePointer && !event.isRejected())
                {
                    event.accept();
                }
            }

            void Overlay::_pointerLeaveEvent(Core::Event::PointerLeave & event)
            {
                if (_p->capturePointer)
                {
                    event.accept();
                }
            }

            void Overlay::_pointerMoveEvent(Core::Event::PointerMove & event)
            {
                if (_p->capturePointer)
                {
                    event.accept();
                }
            }

            void Overlay::_buttonPressEvent(Event::ButtonPress & event)
            {
                DJV_PRIVATE_PTR();
                if (p.capturePointer)
                {
                    event.accept();
                    p.pressedIDs[event.getPointerInfo().id] = true;
                    _doCloseCallback();
                }
            }

            void Overlay::_buttonReleaseEvent(Core::Event::ButtonRelease & event)
            {
                DJV_PRIVATE_PTR();
                const auto i = p.pressedIDs.find(event.getPointerInfo().id);
                if (i != p.pressedIDs.end())
                {
                    event.accept();
                    p.pressedIDs.erase(i);
                }
            }

            void Overlay::_keyPressEvent(Event::KeyPress & event)
            {
                Widget::_keyPressEvent(event);
                DJV_PRIVATE_PTR();
                if (!event.isAccepted())
                {
                    if (p.closeOnEscape && GLFW_KEY_ESCAPE == event.getKey())
                    {
                        event.accept();
                        _doCloseCallback();
                    }
                    else if (p.captureKeyboard)
                    {
                        event.accept();
                    }
                }
            }

            void Overlay::_doCloseCallback()
            {
                DJV_PRIVATE_PTR();
                if (p.closeCallback)
                {
                    p.closeCallback();
                }
            }

        } // namespace Layout
    } // namespace UI
} // namespace djv
