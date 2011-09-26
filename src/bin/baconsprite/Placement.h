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

#ifndef __BACONSPRITE_PLACEMENT
#define __BACONSPRITE_PLACEMENT
namespace BaconSprite
{
    class PlacementChunk
    {
    public:
        std::size_t Identifier() { return this->_identifier; }
        
        int Width() { return this->_width; }
        int Height() { return this->_height; }
        
        int Left() { return this->_left; }
        int Top() { return this->_top; }
        
        PlacementChunk(std::size_t identifier,
                       int width,
                       int height,
                       int left,
                       int top)
        :   _width(width),
            _height(height),
            _left(left),
            _top(top),
            _identifier(identifier)
        {
            
        }
    private:
        int _width;
        int _height;
        
        int _left;
        int _top;
        
        std::size_t _identifier;
    };
    
    class Placement
    {
    public:
        std::vector<PlacementChunk> Chunks;
        
        int Width;
        int Height;
        
        double Efficiency()
        {
            int surfaceArea = this->Width * this->Height,
                rectanglesArea = 0;
            
            std::size_t index = this->Chunks.size();
            while (index--)
                rectanglesArea += this->Chunks[index].Width() * this->Chunks[index].Height();
                
            return double(rectanglesArea) / double(surfaceArea);
        }
        
        PlacementChunk& GetByIdentifier(std::size_t identifier)
        {
            std::size_t index = this->Chunks.size();
            while (index--)
                if (this->Chunks[index].Identifier() == identifier)
                    return this->Chunks[index];
        }
    };
}
#endif