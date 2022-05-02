#options

###A header-only file for handling program options.

##Installation
Drop options.hpp into your project.

(The CMakeLists.txt is to build test.cpp)

##Quick start example:

#include "options.hpp"

```cpp
int main (int argc, char *argv[])
{
    const options opts(argc, argv);
    
    // Find 'o' parameter, or use a default value
    const char *filepath = "default-value.txt";
    opts.get_param('o', &filepath);

    // do something with filepath...
    ...
}
```

Please check out test.cpp for further usage.

