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

#include <djvViewApp/Enum.h>

namespace djv
{
    namespace ViewApp
    {
        float getScrollWheelZoomSpeed(ScrollWheelZoomSpeed value)
        {
            const float values[] =
            {
                .1,
                .25,
                .5
            };
            return values[static_cast<size_t>(value)];
        }

    } // namespace ViewApp

    DJV_ENUM_SERIALIZE_HELPERS_IMPLEMENTATION(
        ViewApp,
        ImageViewLock,
        DJV_TEXT("view_lock_none"),
        DJV_TEXT("view_lock_fill"),
        DJV_TEXT("view_lock_frame"),
        DJV_TEXT("view_lock_center"));

    DJV_ENUM_SERIALIZE_HELPERS_IMPLEMENTATION(
        ViewApp,
        ScrollWheelZoomSpeed,
        DJV_TEXT("settings_scroll_wheel_slow"),
        DJV_TEXT("settings_scroll_wheel_medium"),
        DJV_TEXT("settings_scroll_wheel_fast"));

    DJV_ENUM_SERIALIZE_HELPERS_IMPLEMENTATION(
        ViewApp,
        Playback,
        DJV_TEXT("playback_stop"),
        DJV_TEXT("playback_forward"),
        DJV_TEXT("playback_reverse"));

    DJV_ENUM_SERIALIZE_HELPERS_IMPLEMENTATION(
        ViewApp,
        PlaybackMode,
        DJV_TEXT("playback_once"),
        DJV_TEXT("playback_loop"),
        DJV_TEXT("playback_pingpong"));

    picojson::value toJSON(ViewApp::ImageViewLock value)
    {
        std::stringstream ss;
        ss << value;
        return picojson::value(ss.str());
    }

    picojson::value toJSON(ViewApp::ScrollWheelZoomSpeed value)
    {
        std::stringstream ss;
        ss << value;
        return picojson::value(ss.str());
    }

    picojson::value toJSON(ViewApp::PlaybackMode value)
    {
        std::stringstream ss;
        ss << value;
        return picojson::value(ss.str());
    }

    void fromJSON(const picojson::value& value, ViewApp::ImageViewLock& out)
    {
        if (value.is<std::string>())
        {
            std::stringstream ss(value.get<std::string>());
            ss >> out;
        }
        else
        {
            throw std::invalid_argument(DJV_TEXT("error_cannot_parse_the_value"));
        }
    }

    void fromJSON(const picojson::value& value, ViewApp::ScrollWheelZoomSpeed& out)
    {
        if (value.is<std::string>())
        {
            std::stringstream ss(value.get<std::string>());
            ss >> out;
        }
        else
        {
            throw std::invalid_argument(DJV_TEXT("error_cannot_parse_the_value"));
        }
    }

    void fromJSON(const picojson::value& value, ViewApp::PlaybackMode& out)
    {
        if (value.is<std::string>())
        {
            std::stringstream ss(value.get<std::string>());
            ss >> out;
        }
        else
        {
            throw std::invalid_argument(DJV_TEXT("error_cannot_parse_the_value"));
        }
    }

} // namespace djv

