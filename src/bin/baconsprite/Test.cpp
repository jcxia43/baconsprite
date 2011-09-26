#include "Test.h"

namespace BaconSprite
{
    Test::Test(std::vector<Chunk*> chunks,
               TestApproach approach)
    :   _chunks(chunks),
        _approach(approach)
    {
        
    }
    
    Test::~Test()
    {
        
    }
    
    Placement* Test::Run()
    {
        // * Do an initial placement
        Placement* bestPlacement = PlaceInitial();
        
        // * Determine primary boundaries of placement
        int upperPrimary,
            lowerPrimary,
            latestSecondary;
        
        switch (this->_approach)
        {
        case TestLeftFirst:
            lowerPrimary = this->MaxWidth();
            upperPrimary = bestPlacement->Width;
            latestSecondary = bestPlacement->Height;
            break;
            
        case TestTopFirst:
            lowerPrimary = this->MaxHeight();
            upperPrimary = bestPlacement->Height;
            latestSecondary = bestPlacement->Width;
            break;
        }
        
        // * Iterate through the boundary, starting from the upper minus one (upper is the current best placement)
        int currentPrimary = upperPrimary - 1;
        while (currentPrimary >= lowerPrimary)
        {
            // Do placement within the boundary
            Placement* latestPlacement = this->PlaceWithinBoundary(currentPrimary,
                                                                   latestSecondary);
            
            // Determine the secondary
            switch (this->_approach)
            {
            case TestLeftFirst:
                latestSecondary = latestPlacement->Height;
                break;

            case TestTopFirst:
                latestSecondary = latestPlacement->Width;
                break;
            }

            // Check if this is a better solution
            if (latestPlacement->Efficiency() > bestPlacement->Efficiency())
            {
                delete bestPlacement;
                bestPlacement = latestPlacement;
            }
            else
                delete latestPlacement;
            
            // Finally, decrease the boundary
            currentPrimary--;
        }
        
        // * Return the best placement
        return bestPlacement;
    }
    
    inline int Test::SumWidth()
    {
        int result = 0;
        std::size_t index = this->_chunks.size();
        
        while (index--)
            result += this->_chunks[index]->Width();
        
        return result;
    }
    
    inline int Test::SumHeight()
    {
        int result = 0;
        std::size_t index = this->_chunks.size();
        
        while (index--)
            result += this->_chunks[index]->Height();
        
        return result;
    }
    
    inline int Test::MaxWidth()
    {
        int result = 0;
        std::size_t index = this->_chunks.size();
        
        while (index--)
            if (this->_chunks[index]->Width() > result)
                result = this->_chunks[index]->Width();
        
        return result;
    }
    
    inline int Test::MaxHeight()
    {
        int result = 0;
        std::size_t index = this->_chunks.size();
        
        while (index--)
            if (this->_chunks[index]->Height() > result)
                result = this->_chunks[index]->Height();
        
        return result;
    }
    
    Placement* Test::PlaceInitial()
    {
        // Construct the result
        Placement* result = new Placement();
        
        // Determine the width and height
        result->Width = (this->_approach == TestLeftFirst ?
                         this->SumWidth() :
                         this->MaxWidth());
        
        result->Height = (this->_approach == TestTopFirst ?
                          this->SumHeight() :
                          this->MaxHeight());
        
        // Place each of the chunks
        std::size_t index = 0;
        int offset = 0;
        
        while (index < this->_chunks.size())
        {
            Chunk* chunk = this->_chunks[index];
            
            switch (this->_approach)
            {
            case TestLeftFirst:
                result->Chunks.push_back(PlacementChunk(chunk->Identifier(),
                                                        chunk->Width(),
                                                        chunk->Height(),
                                                        offset,
                                                        0));
                
                offset += chunk->Width();
                break;
                
            case TestTopFirst:
                result->Chunks.push_back(PlacementChunk(chunk->Identifier(),
                                                        chunk->Width(),
                                                        chunk->Height(),
                                                        0,
                                                        offset));

                offset += chunk->Height();
                break;
            }
            
            index++;
        }
        
        // Return the final result
        return result;
    }
    
    Placement* Test::PlaceWithinBoundary(int primaryBoundary,
                                         int secondaryBoundary)
    {
        // * Valid assumption:
        //   In optimal conditions, the secondary boundary dimension
        //   should be the same as the last boundary dimension
        
        // * Iterate until we have a result
        Placement* result = NULL;
        bool fit = false;
        
        std::size_t index;
        int left,
            top;
        
        while (!fit)
        {
            // Create the basic placement
            result = new Placement();
            
            result->Width = (this->_approach == TestLeftFirst ?
                             primaryBoundary :
                             secondaryBoundary);
            
            result->Height = (this->_approach == TestTopFirst ?
                              primaryBoundary :
                              secondaryBoundary);
            
            // Construct a cell operator
            CellSet cells(result->Width,
                          result->Height);

            // Go through all rectangles and try to place them within the cells
            index = 0;
            fit = true;

            while (index < this->_chunks.size())
            {
                Chunk* chunk = this->_chunks[index];
                
                if (!cells.Place(chunk->Width(),
                                 chunk->Height(),
                                 this->_approach,
                                 left,
                                 top))
                {
                    delete result;
                    fit = false;
                    break;
                }
                
                result->Chunks.push_back(PlacementChunk(chunk->Identifier(),
                                                        chunk->Width(),
                                                        chunk->Height(),
                                                        left,
                                                        top));
                
                index++;
            }

            // If we couldn't fit the rectangles, increase the size of the secondary boundary dimension and try again
            if (!fit)
                secondaryBoundary++;
        }

        return result;
    }
    
    /*
    private Placement<IdentifierType> PlaceWithinBoundary(ChunkList<IdentifierType> chunks,
    		                                             				 Placement<IdentifierType> previousPlacement,
    		                                             				 int primaryBoundary,
    		                                             				 OptimizationApproach approach)
    		{
    			// * Valid assumption:
    			//   In optimal conditions, the secondary boundary dimension
    			//   should be the same as the last boundary dimension
    			int secondaryBoundary = ((approach == OptimizationApproach.LeftFirst) ?
    			                         previousPlacement.Height :
    			                         previousPlacement.Width);

    			// * Iterate until we have a result
    			Placement<IdentifierType> result = null;
    			bool fit = false;

    			while (!fit)
    			{
    				// Create the basic placement
    				result = new Placement<IdentifierType>
    				{
    					Width = ((approach == OptimizationApproach.LeftFirst) ?
    					         primaryBoundary : 
    					         secondaryBoundary),
    					Height = ((approach == OptimizationApproach.TopFirst) ?
    					         primaryBoundary : 
    					         secondaryBoundary),

    					Chunks = new List<PlacementChunk<IdentifierType>>()
    				};

    				// Construct a cell operator
    				CellSet<IdentifierType> cells = new CellSet<IdentifierType>(result.Width,
    				                       					     				result.Height);

    				// Go through all rectangles and try to place them within the cells
    				int index = 0;
    				fit = true;

    				while (index < chunks.Count)
    				{
    					PlacementChunk<IdentifierType> placementChunk = cells.Place(chunks[index],
    					                                                  		    approach);

    					if (placementChunk == null)
    					{
    						fit = false;
    						break;
    					}

    					result.Chunks.Add(placementChunk);

    					index++;
    				}

    				// If we couldn't fit the rectangles, increase the size of the secondary boundary dimension and try again
    				if (!fit)
    					secondaryBoundary++;
    			}

    			return result;
    		}*/
}