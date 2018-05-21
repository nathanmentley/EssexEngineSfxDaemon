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

#include <EssexEngineCore/BaseMessage.h>
#include <EssexEngineCore/IMessageResponse.h>
#include <EssexEngineCore/WeakPointer.h>

namespace EssexEngine{
namespace Daemons{
namespace Sfx{
    namespace Messages {
        enum MessageType {
            SetupSfx = 1,
            SetAudioListenerLocation = 2,
            PlayAudio = 3,
            PlayMusic = 4,
            UpdateAudioPosition = 5
        };
    }

    class SfxDaemonMessageResponse: public Core::Models::IMessageResponse
    {

    };

    class SfxDaemonMessage: public Core::Models::BaseMessage<SfxDaemonMessageResponse>
    {
        public:
            SfxDaemonMessage(
                Messages::MessageType _type,
                WeakPointer<void> _data,
                Nullable<int> _x = Nullable<int>(),
                Nullable<int> _y = Nullable<int>(),
                Nullable<int> _z = Nullable<int>()
            ):
            Core::Models::BaseMessage<SfxDaemonMessageResponse>() {
                type = _type;
                data = _data;

                x = _x;
                y = _y;
                z = _z;
            }

            int GetType() {
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
