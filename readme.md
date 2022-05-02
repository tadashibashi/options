# options
one c++ header for handling command-line args

### installation
drop [options.hpp](https://github.com/tadashibashi/options/blob/main/options.hpp) into your project

### quick-start examples:

command line: `program -o my/path.txt`
```cpp
#include "options.hpp"

int main (int argc, char *argv[])
{
    // load args
    const options opts(argc, argv);
    
    const char *filepath = "default/file.txt";
    
    // get option "-o"
    opts.get_arg('o', &filepath);

    // do something with filepath
    ...
}
```

find multiple options with the same flag
```cpp

options plugins;

if (opts.get_options('p', &plugins))
{
    // for every option with 'p' flag
    for (const option &o : plugins)
    {
        // load plugins
        some_loading_function(o.arg());
        ...
    }
}

```

check out [test.cpp](https://github.com/tadashibashi/options/blob/main/test.cpp) for further usage
