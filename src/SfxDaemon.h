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

#include <EssexEngineSfxDaemon/ISfxDriver.h>
#include <EssexEngineCore/BaseDaemon.h>
#include <EssexEngineCore/LogDaemon.h>

namespace EssexEngine{
namespace Daemons{
namespace Sfx{
	class SfxDaemon:public BaseDaemon<ISfxDriver>
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
		private:
	};
}}};