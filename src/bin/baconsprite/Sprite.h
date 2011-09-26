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
#include <algorithm>
#include <jsoncpp/json.h>

#include "Exception.h"
#include "Chunk.h"
#include "OptimizationLevel.h"
#include "Placement.h"
#include "Test.h"

#ifndef __BACONSPRITE_SPRITE
#define __BACONSPRITE_SPRITE
namespace BaconSprite
{
    enum SpriteBehavior
    {
        SpriteNoRepeat,
        SpriteRepeatX,
        SpriteRepeatY
    };

    class Sprite
    {
    public:
        // Constructor and destructor
        Sprite(Json::Value& spriteNode);
        ~Sprite();
        
        // Optimization
        void Optimize(OptimizationLevel level);
        
        // Rendering
        void Render();
        std::string GetStylesheet(bool minifiy);
    private:
        // Output file
        std::string _outputFilename;
    
        // Behavior
        SpriteBehavior _behavior;
    
        // Images
        std::vector<Chunk*> _chunks;
        
        // Optimization
        void OptimizeNoRepeat(OptimizationLevel level);
        void OptimizeRepeatX();
        void OptimizeRepeatY();
        
        // Test construction
        void ConstructTestTree(std::vector<Test*>& tests,
                               std::vector<Chunk*> used,
                               std::vector<Chunk*> available);
        
        // Sorting
        std::vector<Chunk*> SortChunksByApproach(TestApproach approach);
        
        // Placement
        Placement* _placement;
    };
}
#endif