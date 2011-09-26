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
#include <stdexcept>
#include <vector>
#include <iostream>

#include "Chunk.h"
#include "Placement.h"
#include "TestApproach.h"
#include "CellSet.h"

#ifndef __BACONSPRITE_TEST
#define __BACONSPRITE_TEST
namespace BaconSprite
{
    class Test
    {
    public:
        // Constructor and destructor
        Test(std::vector<Chunk*> chunks,
             TestApproach approach);
        ~Test();
        
        // Optimization
        Placement* Run();
    private:
        std::vector<Chunk*> _chunks;
        TestApproach _approach;
        
        Placement* PlaceInitial();
        Placement* PlaceWithinBoundary(int primaryBoundary,
                                       int secondaryBoundary);
        
        inline int SumWidth();
        inline int SumHeight();
        inline int MaxWidth();
        inline int MaxHeight();
    };
}
#endif