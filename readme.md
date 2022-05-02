# options
one c++ header for handling program options

### installation
drop [options.hpp](https://github.com/tadashibashi/options/blob/main/options.hpp) into your project

### quick-start example:

```cpp
#include "options.hpp"

int main (int argc, char *argv[])
{
    const options opts(argc, argv);
    
    const char *filepath = "default-value.txt";
    opts.get_arg('o', &filepath);

    // do something with filepath...
    ...
}
```

please check out [test.cpp](https://github.com/tadashibashi/options/blob/main/test.cpp) for further usage
