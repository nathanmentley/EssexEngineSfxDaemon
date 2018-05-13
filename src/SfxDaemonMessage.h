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

#include <EssexEngineCore/IMessage.h>
#include <EssexEngineCore/WeakPointer.h>

namespace EssexEngine{
namespace Daemons{
namespace Sfx{
    namespace Messages {
        enum MessageType {
            SetAudioListenerLocation = 1,
            PlayAudio = 2,
            PlayMusic = 3,
            UpdateAudioPosition = 4
        };
    }

    class SfxDaemonMessage: public Core::IMessage
    {
        public:
            SfxDaemonMessage(
                Messages::MessageType _type,
                WeakPointer<void> _data,
                Nullable<int> _x = Nullable<int>(),
                Nullable<int> _y = Nullable<int>(),
                Nullable<int> _z = Nullable<int>()
            ):
            Core::IMessage() {
                type = _type;
                data = _data;

                x = _x;
                y = _y;
                z = _z;
            }

            Messages::MessageType GetType() {
                return type;
            }

            WeakPointer<void> GetData() {
                return data;
            }

            Nullable<int> GetX() {
                return x;
            }

            Nullable<int> GetY() {
                return y;
            }

            Nullable<int> GetZ() {
                return z;
            }
        private:
            Messages::MessageType type;
            WeakPointer<void> data;
            Nullable<int> x;
            Nullable<int> y;
            Nullable<int> z;
    };
}}};
