#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include "Arguments.h"
#include "Set.h"

BaconSprite::ArgumentContainer arguments(true);

void ShowHelp(bool graceful = true)
{
    if (graceful)
        std::cout << "baconsprite, the crisp CSS spriter v0.1.0" << std::endl;

    std::cout <<
//      "---------.---------.---------.---------.---------.---------.---------.---------."
        "Usage: baconsprite [options] <definition list>\n" <<
        "\n" <<
        "Available options:\n" <<
        "-h  [--help | -?]             Show this bit of information\n" <<
        "-o  [--optimization] <path>   Level of placement optimization in the range of\n" <<
        "                              1 (lowest) - 3 (highest). Default is 2.\n" <<
        std::endl;
}

namespace AvailableArguments
{
    enum Specific
    {
        Help = 1,
        PlacementOptimization
    };
}

int main(int argc,
         char* argv[])
{
    // * Handle application arguments

    // Register the supported arguments
    arguments.RegisterFlag(AvailableArguments::Help,
                           "--help",
                           "-h",
                           "-?");
    arguments.RegisterParameterWithDefault(AvailableArguments::PlacementOptimization,
                                           "2",
                                           "--optimization",
                                           "-o");

    // Parse out any available arguments
    bool argumentsStatus = arguments.Parse(argc,
                                           argv);

    // If arguments were succesfully parsed, check a few things
    if (argumentsStatus)
    {
        // Help message
        if (arguments.IsSet(AvailableArguments::Help))
        {
            ShowHelp(true);
            return EXIT_SUCCESS;
        }
    }

    // Throw some help text if the user is too inert to use this...
    if (!argumentsStatus)
    {
        ShowHelp(false);
        return EXIT_FAILURE;
    }
    
    try
    {
        // * Parse the definitions
        BaconSprite::Set set(arguments.GetFilename());
        
        // * Optimize each of the sprites
        set.Optimize(BaconSprite::OptimizationHigh);
        
        // * Render the final set
        set.Render();
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    // * And, we're done!
    return EXIT_SUCCESS;
}
