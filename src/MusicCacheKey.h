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

namespace EssexEngine::Daemons::Sfx {
    class MusicCacheKey {
        public:
            MusicCacheKey(std::string _filename) {
                filename = _filename;
            }
            MusicCacheKey() {
                filename = "";
            }

            bool operator==(const MusicCacheKey &other) const {
                return filename == other.filename;
            }
            friend bool operator<(const MusicCacheKey &l, const MusicCacheKey &r) {
                if(l.filename < r.filename) {
                    return true;
                }

                return false;
            }
            friend bool operator>(const MusicCacheKey &l, const MusicCacheKey &r) {
                return !(r <= l);
            }
            friend bool operator<=(const MusicCacheKey &l, const MusicCacheKey &r) {
                return r == l || r < l;
            }
            friend bool operator>=(const MusicCacheKey &l, const MusicCacheKey &r) {
                return r == l || r > l;
            }

        private:
            std::string filename;
    };
};
