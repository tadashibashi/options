# options
simple c++ header for handling command-line args

### supports 
- single-character flags
- argument strings

### installation
drop [options.hpp](https://github.com/tadashibashi/options/blob/main/options.hpp) into your project

### examples
command line: `program -o my/path.txt`
```cpp
#include "options.hpp"

int main (int argc, char *argv[])
{
    // load args
    const options opts(argc, argv);
    ...
}
```
read c-string args
```cpp
// get option "-o"
const char *outpath;
if (opts.get_arg('o', &outpath))
{
    // do something with outpath
    ...
}
```
parse numeric args
```cpp
int i;
long l;
bool b;

if (opts.get_arg('i', &i))
{
    // returns true if parse was successful
    // it's safe to use variable in here
    ...
}

if (opts.get_arg('l', &l))
{
    ...
}

if (opts.get_arg('b', &b))
{
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

log all options for debugging
```cpp
opts.log();

// log format: argv-index, flag (if any), arg (if any)
/*
    [0] program
    [1] -o my/path.txt
    [3] -f
*/
```

check out [test.cpp](https://github.com/tadashibashi/options/blob/main/test.cpp) for further usage
