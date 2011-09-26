#include "Chunk.h"

#include <iostream>

namespace BaconSprite
{
    Chunk::Chunk(Json::Value& imageNode,
                 std::size_t identifier)
    :   _identifier(identifier)
    {
        // * Parse out the input filename
        Json::Value inputNode = imageNode["input"];
        if (!inputNode.isString())
            throw Exception("No input file defined for an image. Fix that!");
            
        this->_inputFilename = inputNode.asString();
        
        // * Parse out the output behavior
        this->_outputSize = imageNode.get("outputSize", false).asBool();
        
        // * Parse out the selectors
        Json::Value selectorsNode = (imageNode.isMember("selectors") ?
                                     imageNode["selectors"] :
                                     imageNode["selector"]);
                                     
        // Check if it is an array
        if (selectorsNode.isArray())
        {
            // Check that it isn't empty
            if (selectorsNode.empty())
                throw Exception("No selectors are defined for this image");
                
            // Iterate all selectors
            Json::Value::ArrayIndex index = 0,
                                    count = selectorsNode.size();
                                    
            while (index < count)
            {
                Json::Value selectorNode = selectorsNode[index];

                if (!selectorNode.isString())
                    throw Exception("You forgot to define a selector correctly...");

                this->_selectors.push_back(selectorNode.asString());

                index++;
            }
        }
        else if (selectorsNode.isString())
            this->_selectors.push_back(selectorsNode.asString());
        else
            throw Exception("No selectors are defined for this image");
            
        // * Load the image
        FILE* file = fopen(this->_inputFilename.c_str(),
                           "r");
        
        if (file == NULL)
            throw Exception("Unable to load image from " + this->_inputFilename);
        
        // Create a PNG image from the file
        this->_inputImage = gdImageCreateFromPng(file);
        
        // Close the image file
        fclose(file);
        
        // Check the image
        if (this->_inputImage == NULL)
            throw Exception("Unable to load image from " + this->_inputFilename);
        
        // * Parse image dimensions
        this->_width = gdImageSX(this->_inputImage);
        this->_height = gdImageSY(this->_inputImage);
    }
    
    Chunk::~Chunk()
    {
        // * Release the image
        gdImageDestroy(this->_inputImage);
    }
}