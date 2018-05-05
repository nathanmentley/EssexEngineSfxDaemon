/* 
 * Essex Engine
 * 
 * Copyright (C) 2017 Nathan Mentley - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the BSD license.
 *
 * You should have received a copy of the BSD license with
 * this file. If not, please visit: https://github.com/nathanmentley/EssexEngine
 */

#include <EssexEngineSfxDaemon/SfxDaemon.h>

using EssexEngine::CachedPointer;
using EssexEngine::WeakPointer;

using EssexEngine::Core::Logging::LogDaemon;

using EssexEngine::Daemons::Sfx::SfxDaemon;
using EssexEngine::Daemons::Sfx::IAudio;
using EssexEngine::Daemons::Sfx::IMusic;
using EssexEngine::Daemons::Sfx::AudioCacheKey;
using EssexEngine::Daemons::Sfx::MusicCacheKey;

using EssexEngine::Daemons::FileSystem::IFileBuffer;

SfxDaemon::SfxDaemon(WeakPointer<Context> _context):
    BaseDaemon(_context),
    audioCache(_context->GetDaemon<LogDaemon>()),
    musicCache(_context->GetDaemon<LogDaemon>()) {}

SfxDaemon::~SfxDaemon() {}

void SfxDaemon::PlayAudio(WeakPointer<IAudio> audio) {
    if(HasDriver()) {
        return GetDriver()->PlayAudio(audio);
    }
}

void SfxDaemon::PlayMusic(WeakPointer<IMusic> music) {
    if(HasDriver()) {
        return GetDriver()->PlayMusic(music);
    }
}

CachedPointer<AudioCacheKey, IAudio> SfxDaemon::GetAudio(CachedPointer<std::string, IFileBuffer> fileContent) {
    AudioCacheKey key = AudioCacheKey(fileContent->GetFileName());

    if (!audioCache.HasKey(key)) {
        WeakPointer<IAudio> audio = GetDriver()->GetAudio(std::move(fileContent));
        audioCache.Cache(key, audio);
    }

    return audioCache.Get(key);
}

CachedPointer<MusicCacheKey, IMusic> SfxDaemon::GetMusic(CachedPointer<std::string, IFileBuffer> fileContent) {
    MusicCacheKey key = MusicCacheKey(fileContent->GetFileName());

    if (!musicCache.HasKey(key)) {
        WeakPointer<IMusic> music = GetDriver()->GetMusic(std::move(fileContent));
        musicCache.Cache(key, music);
    }

    return musicCache.Get(key);
}
