Header-only classes for handling program options.

Example:

int main (int argc, char *argv[])
{
    options opts(argc, argv);
    
    // Find 'o' parameter, or use a default value
    const char *filepath = "default/value.txt";
    opts.get_param('o', &filepath);

    // do something with filepath...
    ...
}

