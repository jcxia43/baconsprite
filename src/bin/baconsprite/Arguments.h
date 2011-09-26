/*
 * Copyright (c) 2011 - Nick Bruun and baconized ApS <nick@baconized.net>
 * Released as part of the "baconsprite" project.
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 * 
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original software.
 * 
 *   3. If you meet (any of) the author(s), you're encouraged to buy them a beer,
 *   a drink or whatever is suited to the situation, given that you like the
 *   software.
 * 
 *   4. This notice may not be removed or altered from any source
 *   distribution.
*/

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>
#include <cstdlib>
#include <cerrno>
#include <limits>
#include <climits>

#ifndef __BACONSPRITE_ARGUMENTS
#define __BACONSPRITE_ARGUMENTS
namespace BaconSprite
{
    enum ArgumentType
    {
        ArgumentTypeFlag,
        ArgumentTypeParameter,
        ArgumentTypeParameterWithDefault
    };

    struct Argument
    {
        ArgumentType Type;
        std::vector<std::string> Aliases;

        bool IsSet;
        std::string Value;
    };

    typedef std::map<int, Argument> ArgumentMap;
    typedef ArgumentMap::iterator ArgumentIterator;
    typedef std::pair<int, Argument> ArgumentPair;

    class ArgumentContainer
    {
    public:
        ArgumentContainer(bool lastIsFilename);
        
        void RegisterFlag(int descriptor,
                          std::string aliasA,
                          std::string aliasB = "",
                          std::string aliasC = "");
        void RegisterParameterWithDefault(int descriptor,
                                          std::string defaultValue,
                                          std::string aliasA,
                                          std::string aliasB = "",
                                          std::string aliasC = "");
        void RegisterParameter(int descriptor,
                               std::string aliasA,
                               std::string aliasB = "",
                               std::string aliasC = "");
        
        bool Parse(int count, char* arguments[]);

        bool IsSet(int descriptor)
        {
            return this->_map[descriptor].IsSet;
        }
        
        std::string GetFilename()
        {
            return this->_filename;
        }

        std::string GetAsString(int descriptor)
        {
            return this->_map[descriptor].Value;
        }
        
        bool TryGetAsUInt32(int descriptor, uint32_t& out)
        {
            errno = 0;
            out = strtoul(this->_map[descriptor].Value.c_str(), NULL, 0);
            return !((errno != 0) &&
                     (out == std::numeric_limits<uint32_t>::max()));
        }
    private:
        // Filename
        bool _lastIsFilename;
        std::string _filename;
        
        ArgumentMap _map;

        inline Argument* GetByAlias(std::string name);
    };
}
#endif
