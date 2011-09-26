# baconsprite - the crisp CSS spriter

CSS sprites are a vastly efficient way of optimizing the number of HTTP requests necessary to present a graphically rich user interface. However, maintaining and optimizing CSS sprites by hand is downright a bitch, and a task that no actual person in their right mind can be bothered with.

The solution? A simple command line tool, that takes a definition list and turns a shitload of images into a series of sprites, which are optimized, and spits out a CSS file for background positioning of the sprited images. Beautiful, right?

## Building and installing

Before building and installing baconstrip, you need to make sure that the following dependencies are installed on your system:

*   `cmake`
*   `gdlib`

Once dependencies are met, building and installing is as easy as the following:

    ./configure
    make
    make install

## Input format

baconstrip relies on a definition list, usually named `sprites.bs` or something like that. `.bs` files are simply JSON files following the schema laid out below:

    {
        "css":
        {
            /* Output filename of the positioning CSS file */
            "output": "Sprites.css",
            
            /* Defines whether CSS minification should be applied or not, default is true */
            "minify": true
        }, 
        "sprites": [
            {
                /* Output filename of the sprite file */
                "output": "SpriteAbsolute.png",
                
                /* Behavior of images. Possible values are no-repeat, repeat-x or repeat-y */
                "behavior": "no-repeat",
                
                "images": [
                    {
                        /* Input filename of the image */
                        "input": "MyImage.png",
                        
                        /* Output the size in the CSS file, default is false */
                        "outputSize": false,
                        
                        /* CSS selectors, can be either a string or an array of strings */
                        "selector": "div.someclass"
                    },
                    ..
                ]
            },
            ..
        ]
    }
