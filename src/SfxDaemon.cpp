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

#include <EssexEngineSfxDaemon/SfxDaemon.h>

using EssexEngine::CachedPointer;
using EssexEngine::WeakPointer;

using EssexEngine::Core::Logging::LogDaemon;

using EssexEngine::Daemons::Sfx::SfxDaemon;
using EssexEngine::Daemons::Sfx::IAudio;
using EssexEngine::Daemons::Sfx::IMusic;
using EssexEngine::Daemons::Sfx::AudioCacheKey;
using EssexEngine::Daemons::Sfx::MusicCacheKey;
using EssexEngine::Daemons::Sfx::Messages::MessageType;

using EssexEngine::Daemons::FileSystem::IFileBuffer;

SfxDaemon::SfxDaemon(WeakPointer<Context> _context):
    BaseProcessDaemon(_context),
    audioCache(_context->GetDaemon<LogDaemon>()),
    musicCache(_context->GetDaemon<LogDaemon>()) {}

SfxDaemon::~SfxDaemon() {}

// non message functions
CachedPointer<AudioCacheKey, IAudio> SfxDaemon::GetAudio(
    CachedPointer<std::string, IFileBuffer> fileContent, int _x, int _y, int _z
) {
    AudioCacheKey key = AudioCacheKey(fileContent->GetFileName());

    if (!audioCache.HasKey(key)) {
        WeakPointer<IAudio> audio = GetDriver()->GetAudio(std::move(fileContent), _x, _y, _z);
        audioCache.Cache(key, audio);
    }

    return audioCache.Get(key);
}

CachedPointer<MusicCacheKey, IMusic> SfxDaemon::GetMusic(
    CachedPointer<std::string, IFileBuffer> fileContent
) {
    MusicCacheKey key = MusicCacheKey(fileContent->GetFileName());

    if (!musicCache.HasKey(key)) {
        WeakPointer<IMusic> music = GetDriver()->GetMusic(std::move(fileContent));
        musicCache.Cache(key, music);
    }

    return musicCache.Get(key);
}

// message generators
void SfxDaemon::PlayAudio(WeakPointer<IAudio> audio) {
    PushMessage(
        WeakPointer<SfxDaemonMessage>(
            new SfxDaemonMessage(
                MessageType::PlayAudio,
                audio.Cast<void>()
            )
        )
    );
}

void SfxDaemon::PlayMusic(WeakPointer<IMusic> music) {
    PushMessage(
        WeakPointer<SfxDaemonMessage>(
            new SfxDaemonMessage(
                MessageType::PlayMusic,
                music.Cast<void>()
            )
        )
    );
}

void SfxDaemon::UpdateAudioPosition(
    WeakPointer<IAudio> audio, int _x, int _y, int _z
) {
    PushMessage(
        WeakPointer<SfxDaemonMessage>(
            new SfxDaemonMessage(
                MessageType::UpdateAudioPosition,
                audio.Cast<void>(),
                Nullable<int>(_x),
                Nullable<int>(_y),
                Nullable<int>(_z)
            )
        )
    );
}

void SfxDaemon::SetAudioListenerLocation(int _x, int _y, int _z) {
    PushMessage(
        WeakPointer<SfxDaemonMessage>(
            new SfxDaemonMessage(
                MessageType::SetAudioListenerLocation,
                WeakPointer<void>(),
                Nullable<int>(_x),
                Nullable<int>(_y),
                Nullable<int>(_z)
            )
        )
    );
}

//message parser
void SfxDaemon::ProcessMessage(WeakPointer<SfxDaemonMessage> message) {
    switch(message->GetType()) {
        case MessageType::SetAudioListenerLocation:
            _SetAudioListenerLocation(message);
            break;
        case MessageType::PlayAudio:
            _PlayAudio(message);
            break;
        case MessageType::PlayMusic:
            _PlayMusic(message);
            break;
        case MessageType::UpdateAudioPosition:
            _UpdateAudioPosition(message);
            break;
    }
}

//message logic
void SfxDaemon::_SetAudioListenerLocation(WeakPointer<SfxDaemonMessage> message) {
    if(HasDriver()) {
        if(
            message->GetX().HasValue() &&
            message->GetY().HasValue() &&
            message->GetZ().HasValue()
        ) {
            GetDriver()->SetAudioListenerLocation(
                message->GetX().Get(),
                message->GetY().Get(),
                message->GetZ().Get()
            );
        }
    }
}

void SfxDaemon::_PlayAudio(WeakPointer<SfxDaemonMessage> message) {
    if(HasDriver()) {
        if(message->GetData().HasValue()) {
            GetDriver()->PlayAudio(message->GetData().Cast<IAudio>());
        }
    }
}

void SfxDaemon::_PlayMusic(WeakPointer<SfxDaemonMessage> message) {
    if(HasDriver()) {
        if(message->GetData().HasValue()) {
            GetDriver()->PlayMusic(message->GetData().Cast<IMusic>());
        }
    }
}

void SfxDaemon::_UpdateAudioPosition(WeakPointer<SfxDaemonMessage> message) {
    if(HasDriver()) {
        if(
            message->GetData().HasValue() &&
            message->GetX().HasValue() &&
            message->GetY().HasValue() &&
            message->GetZ().HasValue()
        ) {
            GetDriver()->UpdateAudioPosition(
                message->GetData().Cast<IAudio>(),
                message->GetX().Get(),
                message->GetY().Get(),
                message->GetZ().Get()
            );
        }
    }
}
