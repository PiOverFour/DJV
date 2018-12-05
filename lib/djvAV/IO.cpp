//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
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

#include <djvAV/IO.h>

#include <djvAV/FFmpeg.h>
#if defined(PNG_FOUND)
#include <djvAV/PNG.h>
#endif // PNG_FOUND

#include <djvCore/Path.h>
#include <djvCore/String.h>

using namespace djv::Core;

namespace djv
{
    namespace AV
    {
        namespace IO
        {
            VideoInfo::VideoInfo()
            {}

            VideoInfo::VideoInfo(const Pixel::Info & info, const Speed & speed, Duration duration) :
                _info(info),
                _speed(speed),
                _duration(duration)
            {}

            void VideoInfo::setInfo(const Pixel::Info & info)
            {
                _info = info;
            }

            void VideoInfo::setSpeed(const Speed & speed)
            {
                _speed = speed;
            }

            void VideoInfo::setDuration(Duration duration)
            {
                _duration = duration;
            }

            bool VideoInfo::operator == (const VideoInfo & other) const
            {
                return _info == other._info && _speed == other._speed && _duration == other._duration;
            }

            AudioInfo::AudioInfo()
            {}

            AudioInfo::AudioInfo(const Audio::DataInfo & info, Duration duration) :
                _info(info),
                _duration(duration)
            {}

            void AudioInfo::setInfo(const Audio::DataInfo & info)
            {
                _info = info;
            }

            void AudioInfo::setDuration(Duration duration)
            {
                _duration = duration;
            }

            bool AudioInfo::operator == (const AudioInfo & other) const
            {
                return _info == other._info && _duration == other._duration;
            }

            Info::Info()
            {}

            Info::Info(const std::string & fileName, const VideoInfo & video, const AudioInfo & audio) :
                _fileName(fileName)
            {
                _video.push_back(video);
                _audio.push_back(audio);
            }

            Info::Info(const std::string & fileName, const std::vector<VideoInfo> & video, const std::vector<AudioInfo> & audio) :
                _fileName(fileName),
                _video(video),
                _audio(audio)
            {}

            void Info::setFileName(const std::string & value)
            {
                _fileName = value;
            }

            void Info::setVideo(const VideoInfo & info)
            {
                _video.clear();
                _video.push_back(info);
            }

            void Info::setVideo(const std::vector<VideoInfo> & info)
            {
                _video = info;
            }

            void Info::setAudio(const AudioInfo & info)
            {
                _audio.clear();
                _audio.push_back(info);
            }

            void Info::setAudio(const std::vector<AudioInfo> & info)
            {
                _audio = info;
            }

            void Info::setTags(const Tags & tags)
            {
                _tags = tags;
            }

            bool Info::operator == (const Info & other) const
            {
                return
                    _fileName == other._fileName &&
                    _video == other._video &&
                    _audio == other._audio &&
                    _tags == other._tags;
            }

            ReadQueue::ReadQueue()
            {}

            std::shared_ptr<ReadQueue> ReadQueue::create()
            {
                auto out = std::shared_ptr<ReadQueue>(new ReadQueue);
                return out;
            }

            std::mutex & ReadQueue::getMutex()
            {
                return _mutex;
            }

            bool ReadQueue::isEnabled() const
            {
                return _enabled;
            }

            void ReadQueue::setEnabled(bool value)
            {
                _enabled = value;
            }

            size_t ReadQueue::getVideoMax() const
            {
                return _enabled ? _videoMax : 0;
            }

            size_t ReadQueue::getAudioMax() const
            {
                return _enabled ? _audioMax : 0;
            }

            size_t ReadQueue::getVideoCount() const
            {
                return _video.size();
            }

            size_t ReadQueue::getAudioCount() const
            {
                return _audio.size();
            }

            bool ReadQueue::hasVideo() const
            {
                return _video.size() > 0;
            }

            bool ReadQueue::hasAudio() const
            {
                return _audio.size() > 0;
            }

            void ReadQueue::addVideo(Timestamp ts, const std::shared_ptr<Image> & data)
            {
                _video.push_back(std::make_pair(ts, data));
            }

            void ReadQueue::addAudio(Timestamp ts, const std::shared_ptr<Audio::Data> & data)
            {
                _audio.push_back(std::make_pair(ts, data));
            }

            VideoFrame ReadQueue::getVideo() const
            {
                return _video.size() > 0 ? _video.front() : VideoFrame();
            }

            AudioFrame ReadQueue::getAudio() const
            {
                return _audio.size() > 0 ? _audio.front() : AudioFrame();
            }

            void ReadQueue::popVideo()
            {
                _video.pop_front();
            }

            void ReadQueue::popAudio()
            {
                _audio.pop_front();
            }

            void ReadQueue::clear()
            {
                _video.clear();
                _audio.clear();
            }

            void IRead::_init(
                const std::string & fileName,
                const std::shared_ptr<ReadQueue> & queue,
                const std::shared_ptr<Core::Context> & context)
            {
                _context = context;
                _fileName = fileName;
                _queue = queue;
            }

            IRead::IRead()
            {}

            IRead::~IRead()
            {}

            void IRead::seek(Timestamp)
            {}

            void IWrite::_init(
                const std::string & fileName,
                const Info & info,
                const std::shared_ptr<Core::Context> & context)
            {
                _context = context;
                _fileName = fileName;
                _info = info;
            }

            IWrite::IWrite()
            {}

            IWrite::~IWrite()
            {}

            void IPlugin::_init(
                const std::string & pluginName,
                const std::string & pluginInfo,
                const std::set<std::string> & fileExtensions,
                const std::shared_ptr<Core::Context> & context)
            {
                _context = context;
                _pluginName = pluginName;
                _pluginInfo = pluginInfo;
                _fileExtensions = fileExtensions;
            }

            IPlugin::IPlugin()
            {}

            IPlugin::~IPlugin()
            {}

            namespace
            {
                bool checkExtension(const std::string & value, const std::set<std::string> & extensions)
                {
                    std::string extension = Path(value).getExtension();
                    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
                    return std::find(extensions.begin(), extensions.end(), extension) != extensions.end();
                }

            } // namespace

            bool IPlugin::canRead(const std::string & fileInfo) const
            {
                return checkExtension(fileInfo, _fileExtensions);
            }

            bool IPlugin::canWrite(const std::string & fileInfo, const Info & info) const
            {
                return checkExtension(fileInfo, _fileExtensions);
            }

            picojson::value IPlugin::getOptions() const
            {
                return picojson::value();
            }

            void IPlugin::setOptions(const picojson::value &)
            {}

            struct System::Private
            {
                std::map<std::string, std::shared_ptr<IPlugin> > plugins;
            };

            void System::_init(const std::shared_ptr<Context> & context)
            {
                ISystem::_init("djv::AV::IO::System", context);

                _p->plugins[FFmpeg::pluginName] = FFmpeg::Plugin::create(context);
#if defined(PNG_FOUND)
                _p->plugins[PNG::pluginName] = PNG::Plugin::create(context);
#endif // PNG_FOUND

                /*
                _p->plugins[Cineon::pluginName].reset(new Cineon::Plugin(context));
                _p->plugins[DPX::pluginName].reset(new DPX::Plugin(context));
                _p->plugins[IFF::pluginName].reset(new IFF::Plugin(context));
                _p->plugins[IFL::pluginName].reset(new IFL::Plugin(context));
                _p->plugins[LUT::pluginName].reset(new LUT::Plugin(context));
                _p->plugins[PIC::pluginName].reset(new PIC::Plugin(context));
                _p->plugins[PPM::pluginName].reset(new PPM::Plugin(context));
                _p->plugins[RLA::pluginName].reset(new RLA::Plugin(context));
                _p->plugins[SGI::pluginName].reset(new SGI::Plugin(context));
                _p->plugins[Targa::pluginName].reset(new Targa::Plugin(context));
#if defined(DJV_THIRD_PARTY_OPENEXR)
                _p->plugins[EXR::pluginName].reset(new EXR::Plugin(context));
#endif // DJV_THIRD_PARTY_OPENEXR
#if defined(DJV_THIRD_PARTY_JPEG)
                _p->plugins[JPEG::pluginName].reset(new JPEG::Plugin(context));
#endif // DJV_THIRD_PARTY_JPEG
#if defined(DJV_THIRD_PARTY_PNG)
                _p->plugins[PNG::pluginName].reset(new PNG::Plugin(context));
#endif // DJV_THIRD_PARTY_PNG
#if defined(DJV_THIRD_PARTY_TIFF)
                _p->plugins[TIFFPlugin::pluginName].reset(new TIFFPlugin::Plugin(context));
#endif // DJV_THIRD_PARTY_TIFF
                */

                for (const auto & i : _p->plugins)
                {
                    std::stringstream s;
                    s << "Plugin: " << i.second->getPluginName() << '\n';
                    s << "    Information: " << i.second->getPluginInfo() << '\n';
                    s << "    File extensions: " << String::joinSet(i.second->getFileExtensions(), ", ") << '\n';
                    _log(s.str());
                }
            }

            System::System() :
                _p(new Private)
            {}

            System::~System()
            {}

            std::shared_ptr<System> System::create(const std::shared_ptr<Context> & context)
            {
                auto out = std::shared_ptr<System>(new System);
                out->_init(context);
                return out;
            }

            picojson::value System::getOptions(const std::string & pluginName) const
            {
                const auto i = _p->plugins.find(pluginName);
                if (i != _p->plugins.end())
                {
                    return i->second->getOptions();
                }
                return picojson::value();
            }

            void System::setOptions(const std::string & pluginName, const picojson::value & value)
            {
                const auto i = _p->plugins.find(pluginName);
                if (i != _p->plugins.end())
                {
                    i->second->setOptions(value);
                }
            }

            bool System::canRead(const std::string & fileName) const
            {
                for (const auto & i : _p->plugins)
                {
                    if (i.second->canRead(fileName))
                    {
                        return true;
                    }
                }
                return false;
            }

            bool System::canWrite(const std::string & fileName, const Info & info) const
            {
                for (const auto & i : _p->plugins)
                {
                    if (i.second->canWrite(fileName, info))
                    {
                        return true;
                    }
                }
                return false;
            }

            std::shared_ptr<IRead> System::read(
                const std::string & fileName,
                const std::shared_ptr<ReadQueue> & queue)
            {
                for (const auto & i : _p->plugins)
                {
                    if (i.second->canRead(fileName))
                    {
                        return i.second->read(fileName, queue);
                    }
                }
                std::stringstream s;
                s << "Cannot read: " << fileName;
                throw std::runtime_error(s.str());
                return nullptr;
            }

            std::shared_ptr<IWrite> System::write(
                const std::string & fileName,
                const Info & info)
            {
                for (const auto & i : _p->plugins)
                {
                    if (i.second->canWrite(fileName, info))
                    {
                        return i.second->write(fileName, info);
                    }
                }
                std::stringstream s;
                s << "Cannot write: " << fileName;
                throw std::runtime_error(s.str());
                return nullptr;
            }

            void System::_exit()
            {
                ISystem::_exit();
                _p->plugins.clear();
            }

        } // namespace IO
    } // namespace AV
} // namespace djv
