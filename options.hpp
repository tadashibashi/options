/* ============================================================================
    options.hpp

    Header-only file, containing classes for retrieving program options.

    MIT License
    Copyright © 2022 Aaron Ishibashi

    Permission is hereby granted, free of charge, to any person obtaining a 
    copy of this software and associated documentation files (the “Software”),
    to deal in the Software without restriction, including without limitation 
    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in 
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
 * ========================================================================= */
#pragma once
#ifndef __options_hpp__
#define __options_hpp__
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <string>

/// class representing a command line option.
/// It can hold both a flag and an argument, and stores its index in the argv
/// array.
class option {
public:
    option() : m_index(-1), m_flag(), m_arg() { }
    option(int index, char flag, const char *param) : m_index(index), m_flag(flag), m_arg(param) { }

    /// Logs info to the output FILE * specified, default: stdout
    void log(FILE *output = stdout) const;
    
    // ========== Getters / Setters ==========

    /// @returns argument c-string, or a nullptr if has_arg() is false
    [[nodiscard]] const char *arg() const { return m_arg; }


    /// @returns the flag or '\0' if has_flag() is false
    [[nodiscard]] char flag() const { return m_flag; }


    /// The index in argv of the arg or flag, whichever came first.
    [[nodiscard]] int index() const { return m_index; }


    /// Has a parameter
    [[nodiscard]] bool has_arg() const { return m_arg; }


    /// Has a flag
    [[nodiscard]] bool has_flag() const { return m_flag; }


    /// Has only a parameter and no flag
    [[nodiscard]] bool is_arg_only() const { return m_arg && !m_flag; }


    /// Has only a flag and no parameter
    [[nodiscard]] bool is_flag_only() const { return m_flag && !m_arg; }


    /// Has both a flag and a paramter
    [[nodiscard]] bool is_flagged_arg() const { return m_flag && m_arg; }


private:
    /// index in argv array
    int m_index;

    /// flag or '\0', if none
    char m_flag;

    /// argument or nullptr, if none
    const char *m_arg;
};

/// Class wrapping a vector of option objects.
/// Manages the parsing of command line args.
class options {
public:
    typedef const option *const_iterator;

    /// @param argc argument count
    /// @param argv array of c-string args
    options(int argc, char *argv[]);
    options() : m_opts() {}

    /// Swaps the guts of this options container with another.
    void swap(options &other);


    /// Logs info to the output FILE * specified, default: stdout
    void log(FILE *output = stdout) const;
    

    // ========== Getters & Querying ==========
    
    /// Finds the first option with a particular flag
    /// @param flag the flag to check
    /// @param opt [out] the option to receive
    /// @returns true if one was found, false if there was none
    bool get_option(char flag, option *opt) const;


    /// Finds multiple options with the same flag
    /// @param flag the flag to check
    /// @param opt [out] the option to receive
    /// @returns true if at least one option with flag was found, false if
    /// there were none
    bool get_options(char flag, options *opts) const;


    /// Finds the arg of the first option with a specified flag
    /// @param flag the flag to check
    /// @param param [out] the parameter to receive
    /// @returns true if parameter was found, false if there was either a missing option,
    /// or the option found did not have an arg.
    bool get_arg(char flag, const char **param) const;


    /// Finds the arg of the first option with a specified flag
    /// @param flag the flag to check
    /// @param val [out] the value to get
    /// @returns true if parameter was found and parsed correctly, false if no such
    /// option exists, the option found did not have a readable long value,
    /// or the option found did not have an arg at all.
    /// Check errno == EINVAL for invalid number, or errno == ERANGE for out of range
    bool get_arg(char flag, long *val) const;


    /// Finds the arg of the first option with a specified flag.
    /// @param flag the flag to check
    /// @param val [out] the value to get
    /// @returns true if parameter was found and parsed correctly, false if no such
    /// option exists, the option found did not have a parsable int value,
    /// or the option found did not have an arg at all.
    /// Check errno == EINVAL for invalid number, or errno == ERANGE for out of range
    bool get_arg(char flag, int *val) const;


    /// Finds the boolean arg of the first option with a specified flag
    /// Accepted arg values are "true", "false", "yes", "no", "1", "0"
    /// @param flag the flag to check.
    /// @param val [out] the value to get.
    /// @returns true if parameter was found and parsed correctly, false if
    /// no such option exists, or the option found did not have a
    /// readable bool value, or the option found did not have an arg at
    /// all. Please remember to not confuse the return value with the bool
    /// out parameter.
    bool get_arg(char flag, bool *val) const;

    /// TODO: Implement get_arg(char flag, double *val) const and float versions.
    bool get_arg(char flag, long double *val) const;
    bool get_arg(char flag, double *val) const;
    bool get_arg(char flag, float *val) const;

    /// Checks if this container has an option with an indicated flag.
    [[nodiscard]] bool has_flag(char flag) const;


    /// Returns a container of options filled with each option that has a flag.
    /// Each may or may not have an argument attached.
    [[nodiscard]] options flags() const;


    /// Returns a list of arg-only options (each has no flags).
    [[nodiscard]] options args() const;


    // ========== Iteration and indexing ==========
    /// Iterator impl
    [[nodiscard]] const_iterator begin() const { return &(*m_opts.begin()); }
    [[nodiscard]] const_iterator end() const { return &(*m_opts.end()); }

    /// Checks if this container is empty.
    [[nodiscard]] bool empty() const { return m_opts.empty(); }


    /// @returns the number of options stored in this container
    [[nodiscard]] size_t size() const { return m_opts.size(); }


    /// Indexer for this container
    [[nodiscard]] const option &operator[](int index) const { return m_opts[index]; }


    /// At-indexer, which throws an exception if out-of-bounds
    [[nodiscard]] const option &at(int index) const { return m_opts.at(index); }


private:
    explicit options(std::vector<option> opts) : m_opts(std::move(opts)) {}
    std::vector<option> m_opts;
};

inline void
option::log(FILE *output) const
{
    assert(output);

    fprintf(output, "[%i]", index());
    if (has_flag())
        fprintf(output, " -%c", flag());
    if (has_arg())
        fprintf(output, " %s", arg());
    fprintf(output, "\n");
}

inline
options::options(int argc, char *argv[]) : m_opts()
{
    // visit each argument
    for (int i = 0; i < argc; ++i)
    {
        char flag = '\0';
        char *arg = nullptr; // ensure it's never nullptr for str length checks
        int ind = -1;

        size_t arg_len = (argv[i] == nullptr || argv[i][0] == '\0') ?
                         0 : strlen(argv[i]);
        
        if (arg_len >= 2 && argv[i][0] == '-' &&
            isalpha(argv[i][1]))                    // is flag
        {
            size_t next_arg_len =
                    (argv[i + 1] == nullptr || argv[i + 1][0] == '\0') ?
                    0 : strlen(argv[i + 1]);

            if (i < argc - 1 && !(next_arg_len >= 2 && argv[i + 1][0] == '-' &&
                isalpha(argv[i + 1][1])))           // flag paired with arg
            {
                arg = argv[i + 1];
                ind = i;
                flag = argv[i][1];
                ++i; // we consumed the next arg, so increase counter by one
            }
            else                                       // flag has no arg
            {
                ind = i;
                flag = argv[i][1];
            }
        }
        else                                           // arg has no flag
        {
            arg = argv[i];
            ind = i;
        }
        
        // commit option
        m_opts.emplace_back(ind, flag, arg);
    }
}


inline void
options::swap(options &other)
{
    other.m_opts.swap(m_opts);
}


inline void
options::log(FILE *output) const
{
    assert(output);

    if (empty())
    {
        fprintf(output, "No options available.\n");
        return;
    }
    
    for (const option &o : m_opts)
    {
        o.log(output);
    }
}


inline bool
options::get_option(char flag, option *opt) const
{
    assert(opt);

    for (const option &o : m_opts)
    {
        if (o.flag() == flag)
        {
            *opt = o;
            return true;
        }
    }
    
    return false;
}


inline bool
options::get_options(char flag, options *opts) const
{
    assert(opts);

    std::vector<option> collection;
    for (const option &o : m_opts)
    {
        if (o.flag() == flag)
        {
            collection.push_back(o);
        }
    }


    if (collection.empty())
    {
        return false;
    }
    else
    {
        options new_opts(std::move(collection));
        opts->swap(new_opts);
        return true;
    }

}


inline bool
options::get_arg(char flag, const char **param) const
{
    assert(param);

    option opt;
    if (get_option(flag, &opt) && opt.has_arg())
    {
        *param = opt.arg();
        return true;
    }

    return false;
}

inline bool options::get_arg(char flag, long double *val) const
{
    assert(val);

    const char *param;
    if (get_arg(flag, &param))
    {
        if (!param)
            return false;

        errno = 0;
        try {
            long double temp = std::stold(param, nullptr);
            *val = temp;
            return true;
        }
        catch(const std::invalid_argument &e)
        {
            errno = EINVAL;
        }
        catch(const std::out_of_range &e)
        {
            errno = ERANGE;
        }

        // For some reason, std::stold on Mac sets errno to ENOENT after the catch block
        // when an invalid arg exception is thrown.
        // We'll set it here manually to match expected errno behavior for this function.
        if (errno == ENOENT)
        {
            errno = EINVAL;
        }
    }

    return false;
}

inline bool options::get_arg(char flag, double *val) const
{
    assert(val);

    const char *param;
    if (get_arg(flag, &param))
    {
        if (!param)
            return false;

        errno = 0;
        try {
            double temp = std::stod(param, nullptr);
            *val = temp;
            return true;
        }
        catch(const std::invalid_argument &e)
        {
            errno = EINVAL;
        }
        catch(const std::out_of_range &e)
        {
            errno = ERANGE;
        }

        // For some reason, std::stold on Mac sets errno to ENOENT after the catch block
        // when an invalid arg exception is thrown.
        // We'll set it here manually to match expected errno behavior for this function.
        if (errno == ENOENT)
        {
            errno = EINVAL;
        }
    }

    return false;
}

inline bool options::get_arg(char flag, float *val) const
{
    assert(val);

    const char *param;
    if (get_arg(flag, &param))
    {
        if (!param)
            return false;

        errno = 0;
        try {
            float temp = std::stof(param, nullptr);
            *val = temp;
            return true;
        }
        catch(const std::invalid_argument &e)
        {
            errno = EINVAL;
        }
        catch(const std::out_of_range &e)
        {
            errno = ERANGE;
        }

        // For some reason, std::stold on Mac sets errno to ENOENT after the catch block
        // when an invalid arg exception is thrown.
        // We'll set it here manually to match expected errno behavior for this function.
        if (errno == ENOENT)
        {
            errno = EINVAL;
        }
    }

    return false;
}

inline bool
options::get_arg(char flag, long *val) const
{
    assert(val);

    const char *param;
    if (get_arg(flag, &param))
    {
        if (!param)
            return false;

        errno = 0;
        long temp = strtol(param, nullptr, 10);
        if (errno != EINVAL && errno != ERANGE)
        {
            *val = temp;
            return true;
        }
    }

    return false;
}


inline bool
options::get_arg(char flag, int *val) const
{
    assert(val);

    long temp;
    if (get_arg(flag, &temp))
    {
        if (temp <= INT_MAX && temp >= INT_MIN)
        {
            *val = (int)temp;
            return true;
        }
        else
        {
            // Might want to return an error code instead...
            errno = ERANGE;
        }
    }

    return false;
}


inline bool
options::get_arg(char flag, bool *val) const
{
    assert(val);

    long temp;
    const char *str = nullptr;

    errno = 0;
    if (get_arg(flag, &temp))
    {
        if (temp == 0 || temp == 1)
        {
            *val = temp;
            return true;
        }
        errno = ERANGE;
        return false;
    }
    else if (get_arg(flag, &str)) // check string for true, false, yes, no
    {
        if (str && str[0])
        {
            if (strcmp(str, "true") == 0 || strcmp(str, "yes") == 0)
            {
                *val = true;
                return true;
            }
            else if (strcmp(str, "false") == 0 || strcmp(str, "no") == 0)
            {
                *val = false;
                return true;
            }
            else
            {
                errno = EINVAL;
                return false;
            }
        }
        else
        {
            errno = EINVAL;
            return false;
        }
    }
    else
    {
        errno = EINVAL;
        return false;
    }

}


inline bool
options::has_flag(char flag) const
{
    for (const option &o: m_opts)
    {
        if (o.flag() == flag)
            return true;
    }

    return false;
}


inline options
options::flags() const
{
    std::vector<option> ret;
    for (const option &o : m_opts)
    {
        if (o.has_flag())
            ret.emplace_back(o);
    }
    return options(std::move(ret));
}


inline options
options::args() const
{
    std::vector<option> ret;
    for (const option &o : m_opts)
    {
        if (o.is_arg_only())
            ret.emplace_back(o);
    }
    
    return options(std::move(ret));
}




#endif /* __options_hpp__ */
