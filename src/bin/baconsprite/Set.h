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
#include <fstream>
#include <jsoncpp/json.h>

#include "Sprite.h"
#include "Exception.h"

#ifndef __BACONSPRITE_SET
#define __BACONSPRITE_SET
namespace BaconSprite
{
    class Set
    {
    public:
        // Constructor
        Set(std::string filename);
        ~Set();
        
        // Processing
        void Optimize(OptimizationLevel level);
        
        // Output
        void Render();
    private:
        // CSS settings
        std::string _settingCssOutput;
        bool _settingCssMinify;
    
        // Sprites
        std::vector<Sprite*> _sprites;
    };
}
#endif