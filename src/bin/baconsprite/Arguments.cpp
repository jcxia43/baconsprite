#include "Arguments.h"

namespace BaconSprite
{
    ArgumentContainer::ArgumentContainer(bool lastIsFilename)
    :   _lastIsFilename(lastIsFilename)
    {
        
    }
    
    void ArgumentContainer::RegisterFlag(int descriptor,
                                         std::string aliasA,
                                         std::string aliasB,
                                         std::string aliasC)
    {
        Argument argument;

        argument.Type = ArgumentTypeFlag;
        argument.Aliases.push_back(aliasA);
        if (!aliasB.empty())
            argument.Aliases.push_back(aliasB);
        if (!aliasC.empty())
            argument.Aliases.push_back(aliasC);

        argument.IsSet = false;

        this->_map[descriptor] = argument;
    }

    void ArgumentContainer::RegisterParameterWithDefault(int descriptor,
                                             std::string defaultValue,
                                             std::string aliasA,
                                             std::string aliasB,
                                             std::string aliasC)
    {
        Argument argument;

        argument.Type = ArgumentTypeParameterWithDefault;
        argument.Aliases.push_back(aliasA);
        if (!aliasB.empty())
            argument.Aliases.push_back(aliasB);
        if (!aliasC.empty())
            argument.Aliases.push_back(aliasC);

        argument.Value = defaultValue;
        argument.IsSet = false;

        this->_map[descriptor] = argument;
    }

    void ArgumentContainer::RegisterParameter(int descriptor,
                                             std::string aliasA,
                                             std::string aliasB,
                                             std::string aliasC)
    {
        Argument argument;

        argument.Type = ArgumentTypeParameter;
        argument.Aliases.push_back(aliasA);
        if (!aliasB.empty())
            argument.Aliases.push_back(aliasB);
        if (!aliasC.empty())
            argument.Aliases.push_back(aliasC);

        argument.IsSet = false;

        this->_map[descriptor] = argument;
    }

    bool ArgumentContainer::Parse(int count, char* arguments[])
    {
        std::string name, value, current;
        Argument* argument = NULL;
        
        int index = 1;
        while (index < count)
        {
            current = arguments[index];
            index++;

            // Skip empty names
            if (current.empty())
            {
                argument = NULL;
                continue;
            }

            if (current[0] == '-')
            {
                // Make sure that no argument value is expected up until now
                if (argument != NULL)
                {
                    std::cout << "Argument '" << argument->Aliases[0] << "' requires a value" << std::endl;
                    return false;
                }

                // Check if this is the last argument
                if ((index == count) &&
                    (this->_lastIsFilename))
                {
                    this->_filename = current;
                    continue;
                }

                // Get the new argument
                argument = this->GetByAlias(current);

                // Validate that this is an allowed argument
                if (argument == NULL)
                {
                    std::cout << "Unknown argument '" << current << "'" << std::endl;
                    return false;
                }

                // Set the value if this is a flag argument
                if (argument->Type == ArgumentTypeFlag)
                {
                    argument->IsSet = true;

                    argument = NULL;
                }
            }
            else
            {
                // Check if this is the last argument
                if ((index == count) &&
                    (this->_lastIsFilename))
                {
                    this->_filename = current;
                    continue;
                }
                
                if (argument == NULL)
                {
                    std::cout << "Unknown argument '" << current << "'" << std::endl;
                    return false;
                }

                argument->Value = current;
                argument->IsSet = true;

                argument = NULL;
            }
        }

        // Make sure that no argument value is expected up until now
        if (argument != NULL)
        {
            std::cout << "Argument '" << argument->Aliases[0] << "' requires a value" << std::endl;
            return false;
        }
        
        // Make sure that filename is set
        if ((this->_lastIsFilename) &&
            (this->_filename.empty()))
        {
            return false;
        }

        return true;
    }

    inline Argument* ArgumentContainer::GetByAlias(std::string name)
    {
        ArgumentIterator iterator = this->_map.begin();
        std::size_t aliasIndex;
        while (iterator != this->_map.end())
        {
            aliasIndex = iterator->second.Aliases.size();
            while (aliasIndex--)
                if (iterator->second.Aliases[aliasIndex] == name)
                    return &iterator->second;

            iterator++;
        }

        return NULL;
    }
}
