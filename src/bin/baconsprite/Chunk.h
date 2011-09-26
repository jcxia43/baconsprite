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
#include <vector>
#include <jsoncpp/json.h>
#include <cstdio>
#include <gd.h>

#include "Exception.h"

#ifndef __BACONSPRITE_CHUNK
#define __BACONSPRITE_CHUNK
namespace BaconSprite
{
    class Chunk
    {
    public:
        // Constructor
        Chunk(Json::Value& imageNode,
              std::size_t identifier);
        ~Chunk();
        
        // Accessors
        int Width() { return this->_width; }
        int Height() { return this->_height; }
        
        std::size_t Identifier() { return this->_identifier; }
        bool OutputSize() { return this->_outputSize; }
        
        gdImagePtr Image() { return this->_inputImage; }
        
        std::vector<std::string> Selectors() { return this->_selectors; }
    private:
        // Input file
        std::string _inputFilename;
        gdImagePtr _inputImage;
        int _width;
        int _height;
        
        std::size_t _identifier;
    
        // Output size in CSS
        bool _outputSize;
    
        // CSS selectors
        std::vector<std::string> _selectors;
    };
}
#endif