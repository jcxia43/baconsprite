#include "Set.h"

#include <iostream>

namespace BaconSprite
{
    Set::Set(std::string filename)
    {
        // * Try to parse the file
        Json::Value root;
    
        {
            // Try to open the file and read all its contents
            std::fstream inStream;
            
            inStream.open(filename.c_str(),
                          std::ifstream::in);
                          
            if (!inStream.is_open())
                throw Exception("Unable to open sprite set " + filename);
                
            std::string content;
            
            inStream.seekg(0, std::ios::end);
            content.reserve(inStream.tellg());
            inStream.seekg(0, std::ios::beg);

            content.assign((std::istreambuf_iterator<char>(inStream)),
                           std::istreambuf_iterator<char>());
            
            // Try to actually parse the JSON
            Json::Reader reader;
        
            if (!reader.parse(content,
                              root))
                throw Exception("Parsing error when trying to parse " + filename + ": " + reader.getFormatedErrorMessages());
        }
        
        // * Read out CSS rules
        Json::Value cssNode = root["css"];
        
        // Check the availability of the document
        if (cssNode.isNull())
            throw Exception("No rules for CSS output found");
            
        // Check that the output is defined
        Json::Value cssOutput = cssNode["output"];
        if (cssOutput.isNull())
            throw Exception("No output filename for the CSS");
        
        _settingCssOutput = cssOutput.asString();
        
        // Read out the minification
        _settingCssMinify = cssNode.get("minify",
                                        "true").asBool();
        
        // * Read out the sprites
        Json::Value spritesNode = root["sprites"];
        
        // Check that sprites are defined
        if ((!spritesNode.isArray()) ||
            (spritesNode.empty()))
            throw Exception("No sprites defined");
            
        // Iterate all sprites
        Json::Value::ArrayIndex index = 0,
                                count = spritesNode.size();

        while (index < count)
        {
            Json::Value spriteNode = spritesNode[index];
            
            if (!spriteNode.isObject())
                throw Exception("You forgot to define a sprite correctly...");
                
            this->_sprites.push_back(new Sprite(spriteNode));
            
            index++;
        }
    }
    
    Set::~Set()
    {
        // Destroy all sprites
        std::size_t index = this->_sprites.size();
        while (index--)
            delete this->_sprites[index];
    }
    
    void Set::Optimize(OptimizationLevel level)
    {
        // Optimize each of the sprites
        std::size_t index = 0;
        while (index < this->_sprites.size())
        {
            this->_sprites[index]->Optimize(level);
            
            index++;
        }
    }
    
    void Set::Render()
    {
        // * Render all images to their output files and get the CSS
        std::string stylesheet;
        std::size_t index = 0;
        
        while (index < this->_sprites.size())
        {
            this->_sprites[index]->Render();
            
            if ((index > 0) &&
                (!this->_settingCssMinify))
                stylesheet += "\n";
                
            stylesheet += this->_sprites[index]->GetStylesheet(this->_settingCssMinify);
            
            index++;
        }
        
        // * Save the stylesheet
        FILE* stylesheetFile = fopen(this->_settingCssOutput.c_str(),
                                     "w");
        
        if (stylesheetFile == NULL)
            throw Exception("Unable to open stylesheet file " + this->_settingCssOutput + " for writing");
        
        fwrite(stylesheet.c_str(),
               1,
               stylesheet.length(),
               stylesheetFile);
        
       fclose(stylesheetFile);
    }
}