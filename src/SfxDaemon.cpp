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

using EssexEngine::Core::Models::IMessage;
using EssexEngine::Core::Models::IMessageResponse;
using EssexEngine::Core::Logging::LogDaemon;

using EssexEngine::Daemons::Sfx::SfxDaemon;
using EssexEngine::Daemons::Sfx::IAudio;
using EssexEngine::Daemons::Sfx::IMusic;
using EssexEngine::Daemons::Sfx::AudioCacheKey;
using EssexEngine::Daemons::Sfx::MusicCacheKey;
using EssexEngine::Daemons::Sfx::SfxDaemonMessageResponse;
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
void SfxDaemon::SetupSfx() {
    PushMessage(
        WeakPointer<IMessage>(
            new SfxDaemonMessage(
                MessageType::SetupSfx,
                WeakPointer<void>()
            )
        )
    );
}

void SfxDaemon::PlayAudio(WeakPointer<IAudio> audio) {
    PushMessage(
        WeakPointer<IMessage>(
            new SfxDaemonMessage(
                MessageType::PlayAudio,
                audio.Cast<void>()
            )
        )
    );
}

void SfxDaemon::PlayMusic(WeakPointer<IMusic> music) {
    PushMessage(
        WeakPointer<IMessage>(
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
        WeakPointer<IMessage>(
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
        WeakPointer<IMessage>(
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
WeakPointer<IMessageResponse> SfxDaemon::ProcessMessage(WeakPointer<IMessage> message) {
    switch(message->GetType()) {
        case MessageType::SetupSfx:
            return _SetupSfx(message.Cast<SfxDaemonMessage>()).Cast<IMessageResponse>();
        case MessageType::SetAudioListenerLocation:
            return _SetAudioListenerLocation(message.Cast<SfxDaemonMessage>()).Cast<IMessageResponse>();
        case MessageType::PlayAudio:
            return _PlayAudio(message.Cast<SfxDaemonMessage>()).Cast<IMessageResponse>();
        case MessageType::PlayMusic:
            return _PlayMusic(message.Cast<SfxDaemonMessage>()).Cast<IMessageResponse>();
        case MessageType::UpdateAudioPosition:
            return _UpdateAudioPosition(message.Cast<SfxDaemonMessage>()).Cast<IMessageResponse>();
    }

    return WeakPointer<IMessageResponse>(
        new IMessageResponse()
    );
}

//message logic
WeakPointer<SfxDaemonMessageResponse> SfxDaemon::_SetupSfx(WeakPointer<SfxDaemonMessage> message) {
    if(HasDriver()) {
        GetDriver()->SetupSfx();
    }

    return WeakPointer<SfxDaemonMessageResponse>(
        new SfxDaemonMessageResponse()
    );
}

WeakPointer<SfxDaemonMessageResponse> SfxDaemon::_SetAudioListenerLocation(WeakPointer<SfxDaemonMessage> message) {
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

    return WeakPointer<SfxDaemonMessageResponse>(
        new SfxDaemonMessageResponse()
    );
}

WeakPointer<SfxDaemonMessageResponse> SfxDaemon::_PlayAudio(WeakPointer<SfxDaemonMessage> message) {
    if(HasDriver()) {
        if(message->GetData().HasValue()) {
            GetDriver()->PlayAudio(message->GetData().Cast<IAudio>());
        }
    }

    return WeakPointer<SfxDaemonMessageResponse>(
        new SfxDaemonMessageResponse()
    );
}

WeakPointer<SfxDaemonMessageResponse> SfxDaemon::_PlayMusic(WeakPointer<SfxDaemonMessage> message) {
    if(HasDriver()) {
        if(message->GetData().HasValue()) {
            GetDriver()->PlayMusic(message->GetData().Cast<IMusic>());
        }
    }

    return WeakPointer<SfxDaemonMessageResponse>(
        new SfxDaemonMessageResponse()
    );
}

WeakPointer<SfxDaemonMessageResponse> SfxDaemon::_UpdateAudioPosition(WeakPointer<SfxDaemonMessage> message) {
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

    return WeakPointer<SfxDaemonMessageResponse>(
        new SfxDaemonMessageResponse()
    );
}
