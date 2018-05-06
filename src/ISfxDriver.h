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
#pragma once

#include <string>

#include <EssexEngineCore/IDriver.h>
#include <EssexEngineCore/WeakPointer.h>
#include <EssexEngineCore/CachedPointer.h>

#include <EssexEngineFileSystemDaemon/IFileBuffer.h>

#include <EssexEngineSfxDaemon/IMusic.h>
#include <EssexEngineSfxDaemon/IAudio.h>

namespace EssexEngine{
namespace Daemons{
namespace Sfx{
    class ISfxDriver: public Core::IDriver
    {
        public:
            virtual void PlayAudio(WeakPointer<IAudio> audio) = 0;
            virtual void PlayMusic(WeakPointer<IMusic> music) = 0;
        
            virtual void SetAudioListenerLocation(int _x, int _y, int _z) = 0;
            virtual void UpdateAudioPosition(WeakPointer<IAudio> audio, int _x, int _y, int _z) = 0;
            virtual WeakPointer<IAudio> GetAudio(CachedPointer<std::string, FileSystem::IFileBuffer> fileContent, int _x, int _y, int _z) = 0;
            virtual WeakPointer<IMusic> GetMusic(CachedPointer<std::string, FileSystem::IFileBuffer> fileContent) = 0;
        private:
    };
}}};
