#include "Sprite.h"

namespace BaconSprite
{
    Sprite::Sprite(Json::Value& spriteNode)
    :   _placement(NULL)
    {
        // * Parse out the output filename
        Json::Value outputNode = spriteNode["output"];
        if (!outputNode.isString())
            throw Exception("No output file defined for a sprite. Fix that!");
            
        this->_outputFilename = outputNode.asString();
        
        // * Parse out the behavior
        Json::Value behaviorNode = spriteNode["behavior"];
        if (!behaviorNode.isString())
            throw Exception("No behavior defined for a sprite. Fix that!");
            
        std::string behavior = behaviorNode.asString();
        
        if (behavior == "no-repeat")
            this->_behavior = SpriteNoRepeat;
        else if (behavior == "repeat-x")
            this->_behavior = SpriteRepeatX;
        else if (behavior == "repeat-y")
            this->_behavior = SpriteRepeatY;
        else
            throw Exception("Sprite behavior '" + behavior + "' is not recognized.");
            
        // * Parse out the images
        Json::Value imagesNode = spriteNode["images"];
        
        // Check that images are defined
        if ((!imagesNode.isArray()) ||
            (imagesNode.empty()))
            throw Exception("No images defined in a sprite defined. That's bad.");
            
        // Iterate all sprites
        Json::Value::ArrayIndex index = 0,
                                count = imagesNode.size();
        std::size_t identifier = 0;

        while (index < count)
        {
            Json::Value imageNode = imagesNode[index];
            
            if (!imageNode.isObject())
                throw Exception("You forgot to define an image correctly...");
                
            this->_chunks.push_back(new Chunk(imageNode,
                                              identifier));
            
            index++;
            identifier++;
        }
    }
    
    Sprite::~Sprite()
    {
        std::size_t index = this->_chunks.size();
        while (index--)
            delete this->_chunks[index];
        
        if (this->_placement != NULL)
            delete this->_placement;
    }
    
    void Sprite::Optimize(OptimizationLevel level)
    {
        switch (this->_behavior)
        {
        case SpriteNoRepeat:
            this->OptimizeNoRepeat(level);
            break;
            
        case SpriteRepeatX:
            this->OptimizeRepeatX();
            break;
        
        case SpriteRepeatY:
            this->OptimizeRepeatY();
            break;
        }
    }
    
    bool ChunkSortLeftFirst(Chunk* a,
                            Chunk* b)
    {
        return (a->Height() > b->Height());
    }
    
    bool ChunkSortTopFirst(Chunk* a,
                           Chunk* b)
    {
        return (a->Width() > b->Width());
    }
    
    std::vector<Chunk*> Sprite::SortChunksByApproach(TestApproach approach)
    {
        std::vector<Chunk*> result = this->_chunks;
        
        switch (approach)
        {
        case TestLeftFirst:
            std::sort(result.begin(),
                      result.end(),
                      ChunkSortLeftFirst);
            break;
            
        case TestTopFirst:
            std::sort(result.begin(),
                      result.end(),
                      ChunkSortTopFirst);
            break;
        }
        
        return result;
    }
    
    void Sprite::ConstructTestTree(std::vector<Test*>& tests,
                                   std::vector<Chunk*> used,
                                   std::vector<Chunk*> available)
    {
        // Check if all are used
        if (available.empty())
        {
            tests.push_back(new Test(used,
                                     TestLeftFirst));
            tests.push_back(new Test(used,
                                     TestTopFirst));
        }
        else
        {
            // Loop all and pick one out
            std::size_t index = 0;
            while (index < available.size())
            {
                std::vector<Chunk*> newUsed = used,
                                    newAvailable = available;
                
                newUsed.push_back(available[index]);
                newAvailable.erase(newAvailable.begin() + index);
                
                this->ConstructTestTree(tests,
                                        newUsed,
                                        newAvailable);
                
                index++;
            }
        }
    }
    
    void Sprite::OptimizeNoRepeat(OptimizationLevel level)
    {
        // * Check that all have an equal height
        int height = 0,
            width = 0;
        std::size_t index = this->_chunks.size();
        while (index--)
        {
            if (height < this->_chunks[index]->Height())
                height = this->_chunks[index]->Height();
            width += this->_chunks[index]->Width();
        }
        
        // * Construct all the tests
        std::vector<Test*> tests;
        
        switch (level)
        {
        case OptimizationLow:
            tests.push_back(new Test(this->SortChunksByApproach(TestLeftFirst),
                                     TestLeftFirst));
            
            break;
            
        case OptimizationMedium:
        case OptimizationHigh:
            tests.push_back(new Test(this->SortChunksByApproach(TestLeftFirst),
                                     TestLeftFirst));
            tests.push_back(new Test(this->SortChunksByApproach(TestTopFirst),
                                     TestTopFirst));
            
            break;
        
        // Currently not used as the number of tests grows to 2 * this->_chunks.size()!
        /*
        case OptimizationHigh:
            this->ConstructTestTree(tests,
                                    std::vector<Chunk*>(),
                                    this->_chunks);

            break;
        */
        }
        
        // * Run the tests and delete them as we go
        Placement* bestPlacement = NULL;
        
        std::size_t testIndex = 0;
        while (testIndex < tests.size())
        {
            // Get the placement
            Placement* testPlacement = tests[testIndex]->Run();
            
            // Replace the best placement if possible
            if (bestPlacement == NULL)
                bestPlacement = testPlacement;
            else if (testPlacement->Efficiency() > bestPlacement->Efficiency())
            {
                delete bestPlacement;
                bestPlacement = testPlacement;
            }
            
            // Delete the placement test
            delete tests[testIndex];
            
            testIndex++;
        }
        
        // * Assign the best placement
        this->_placement = bestPlacement;
    }
    
    void Sprite::OptimizeRepeatX()
    {
        // * Check that all have an equal width
        int width = -1,
            height = 0;
        std::size_t index = this->_chunks.size();
        while (index--)
        {
            if (width == -1)
                width = this->_chunks[index]->Width();
            else if (width != this->_chunks[index]->Width())
                throw Exception("Cannot create a repeat-x sprite because all images are not of the same width");
            
            height += this->_chunks[index]->Height();
        }
        
        // * Construct a simplistic placement
        this->_placement = new Placement();
        this->_placement->Width = width;
        this->_placement->Height = height;
        
        index = 0;
        int top = 0;
        while (index < this->_chunks.size())
        {
            this->_placement->Chunks.push_back(PlacementChunk(index,
                                                              this->_chunks[index]->Width(),
                                                              this->_chunks[index]->Height(),
                                                              0,
                                                              top));
            
            top += this->_chunks[index]->Height();
            index++;
        }
    }
    
    void Sprite::OptimizeRepeatY()
    {
        // * Check that all have an equal height
        int height = -1,
            width = 0;
        std::size_t index = this->_chunks.size();
        while (index--)
        {
            if (height == -1)
                height = this->_chunks[index]->Height();
            else if (height != this->_chunks[index]->Height())
                throw Exception("Cannot create a repeat-y sprite because all images are not of the same height");
            
            width += this->_chunks[index]->Width();
        }
        
        // * Construct a simplistic placement
        this->_placement = new Placement();
        this->_placement->Width = width;
        this->_placement->Height = height;
        
        index = 0;
        int left = 0;
        while (index < this->_chunks.size())
        {
            this->_placement->Chunks.push_back(PlacementChunk(index,
                                                              this->_chunks[index]->Width(),
                                                              this->_chunks[index]->Height(),
                                                              left,
                                                              0));
            
            left += this->_chunks[index]->Width();
            index++;
        }
    }
    
    void Sprite::Render()
    {
        // * Check that a placement has been generated
        if (this->_placement == NULL)
            throw Exception("No sprite placement has been generated.");
        
        // * Create the output image from the dimensions
        gdImagePtr outputImage = gdImageCreateTrueColor(this->_placement->Width,
                                                        this->_placement->Height);
        
        // Set the image to store alpha values
        gdImageSaveAlpha(outputImage, true);
        gdImageAlphaBlending(outputImage, 0);
        
        // Fill the image with a completely transparent background
        int alphaColor = gdImageColorAllocateAlpha(outputImage,
                                                   0,
                                                   0,
                                                   0,
                                                   127);
        
        gdImageFilledRectangle(outputImage,
                               0,
                               0,
                               this->_placement->Width,
                               this->_placement->Height,
                               alphaColor);
        
        // * Place all the images
        std::size_t index = this->_chunks.size();
        while (index--)
        {
            Chunk* chunk = this->_chunks[index];
            PlacementChunk& placementChunk = this->_placement->GetByIdentifier(index);
            
            gdImageCopy(outputImage, // Destination image
                        chunk->Image(), // Source image
                        placementChunk.Left(), // Destination X
                        placementChunk.Top(), // Destination Y
                        0, // Source X
                        0, // Source Y
                        chunk->Width(), // Width
                        chunk->Height()); // Height
        }
        
        // * Store the output image
        FILE* file = fopen(this->_outputFilename.c_str(),
                           "wb");
        
        gdImagePng(outputImage,
                   file);
        
        fclose(file);
        
        // * Destroy the output image
        gdImageDestroy(outputImage);
    }
    
    std::string Sprite::GetStylesheet(bool minify)
    {
        // * Initialzie a result construct
        std::string result;
        
        char buffer[1024];
        int bufferUsed;
        
        // * Sum up all selectors
        std::size_t index = 0;
        bool first = true;
        while (index < this->_chunks.size())
        {
            std::vector<std::string> selectors = this->_chunks[index]->Selectors();
            
            std::size_t selectorIndex = 0;
            while (selectorIndex < selectors.size())
            {
                if (first)
                    first = false;
                else
                {
                    if (minify)
                        result += ",";
                    else
                        result += ",\n";
                }
                
                result += selectors[selectorIndex];
                
                selectorIndex++;
            }
            
            index++;
        }
        
        // Write the selector rules
        if (minify)
            result += "{background-image:url('" + this->_outputFilename + "');background-repeat:no-repeat;}\n";
        else
            result += "\n{\n\
\tbackground-image: url('" + this->_outputFilename + "');\n\
\tbackground-repeat: no-repeat;\n\
}\n";
        
        // * Write the specific image rules
        index = 0;
        while (index < this->_chunks.size())
        {
            std::vector<std::string> selectors = this->_chunks[index]->Selectors();
            
            first = true;
            std::size_t selectorIndex = 0;
            while (selectorIndex < selectors.size())
            {
                if (first)
                {
                    first = false;
                    
                    if (!minify)
                        result += "\n";
                }
                else
                {
                    if (minify)
                        result += ",";
                    else
                        result += ",\n";
                }
                
                result += selectors[selectorIndex];
                
                selectorIndex++;
            }
            
            PlacementChunk& placement = this->_placement->GetByIdentifier(index);
            
            result += (minify ? std::string("{") : std::string("\n{\n"));
            
            bufferUsed = sprintf(buffer,
                                 "-%dpx -%dpx",
                                 placement.Left(),
                                 placement.Top());
            
            if (minify)
                result += "background-position:" + std::string(buffer, bufferUsed) + ";";
            else
                result += "\tbackground-position: " + std::string(buffer, bufferUsed) + ";\n";
                
            if (this->_chunks[index]->OutputSize())
            {
                if (minify)
                    bufferUsed = sprintf(buffer,
                                         "width:%dpx;height:%dpx;",
                                         this->_chunks[index]->Width(),
                                         this->_chunks[index]->Height());
                else
                    bufferUsed = sprintf(buffer,
                                         "\twidth: %dpx;\n\theight: %dpx;\n",
                                         this->_chunks[index]->Width(),
                                         this->_chunks[index]->Height());
                
                result += std::string(buffer, bufferUsed);
            }

            result += "}\n";
            
            index++;
        }
        
        return result;
    }
}