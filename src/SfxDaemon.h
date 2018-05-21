/* 
 * Essex Engine
 * 
 * Copyright (C) 2018 Nathan Mentley - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the BSD license.
 *
 * You should have received a copy of the BSD license with
 * this file. If not, please visit: https://github.com/nathanmentley/EssexEngine
 */
#pragma once

#include <EssexEngineCore/BaseProcessDaemon.h>
#include <EssexEngineCore/LogDaemon.h>
#include <EssexEngineCore/CachedPointer.h>
#include <EssexEngineCore/ResourceCache.h>

#include <EssexEngineSfxDaemon/SfxDaemonMessage.h>
#include <EssexEngineSfxDaemon/ISfxDriver.h>
#include <EssexEngineSfxDaemon/MusicCacheKey.h>
#include <EssexEngineSfxDaemon/AudioCacheKey.h>

namespace EssexEngine{
namespace Daemons{
namespace Sfx{
    class SfxDaemon:public BaseProcessDaemon<ISfxDriver>
    {
        public:
            SfxDaemon(WeakPointer<Context> _context);
            ~SfxDaemon();
            
            void Init() {
                if(GetContext()->HasDaemon<Core::Logging::LogDaemon>()) {
                    GetContext()->GetDaemon<Core::Logging::LogDaemon>()->LogLine(
                        "Loading Daemon [%s] [%s]",
                        GetDaemonName().c_str(),
                        GetDaemonVersion().c_str()
                    );
                }
            }
            std::string GetDaemonName() { return "Sfx"; }
            std::string GetDaemonVersion() { return ESSEX_ENGINE_VERSION; }
            
            void SetupSfx();
            void SetAudioListenerLocation(int _x, int _y, int _z);
            void PlayAudio(WeakPointer<IAudio> audio);
            void PlayMusic(WeakPointer<IMusic> music);
            void UpdateAudioPosition(WeakPointer<IAudio> audio, int _x, int _y, int _z);

            CachedPointer<AudioCacheKey, IAudio> GetAudio(CachedPointer<std::string, FileSystem::IFileBuffer> fileContent, int _x, int _y, int _z);
            CachedPointer<MusicCacheKey, IMusic> GetMusic(CachedPointer<std::string, FileSystem::IFileBuffer> fileContent);
        protected:
            WeakPointer<Core::Models::IMessageResponse> ProcessMessage(WeakPointer<Core::Models::IMessage> message);
            
            WeakPointer<SfxDaemonMessageResponse> _SetupSfx(WeakPointer<SfxDaemonMessage> message);
            WeakPointer<SfxDaemonMessageResponse> _SetAudioListenerLocation(WeakPointer<SfxDaemonMessage> message);
            WeakPointer<SfxDaemonMessageResponse> _PlayAudio(WeakPointer<SfxDaemonMessage> message);
            WeakPointer<SfxDaemonMessageResponse> _PlayMusic(WeakPointer<SfxDaemonMessage> message);
            WeakPointer<SfxDaemonMessageResponse> _UpdateAudioPosition(WeakPointer<SfxDaemonMessage> message);
        private:
            Core::Utils::ResourceCache<AudioCacheKey, IAudio> audioCache;
            Core::Utils::ResourceCache<MusicCacheKey, IMusic> musicCache;
    };
}}};
