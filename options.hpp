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
#ifndef aishi_options_hpp
#define aishi_options_hpp
#include <vector>
#include <cstdio>
#include <utility>

class option {
public:
    // default dummy option
    option() : m_index(-1), m_flag(), m_param() { }
    // populated option
    option(int index, char flag, const char *param) : m_index(index), m_flag(flag), m_param(param) { }
    
    void log() const;
    
    // Getters / Setters
    
    const char *param() const { return m_param; }
    char flag() const { return m_flag; }
    
    /// The index in argv of the param or flag, whichever came first.
    int index() const { return m_index; }
    /// Has a parameter
    bool has_param() const { return m_param; }
    /// Has a flag
    bool has_flag() const { return m_flag; }
    /// Has only a parameter and no flag
    bool is_param() const { return m_param && !m_flag; }
    /// Has only a flag and no parameter
    bool is_flag() const { return m_flag && !m_param; }
    /// Has both a flag and a paramter
    bool is_flagged_param() const { return m_flag && m_param; }
private:
    int m_index;
    char m_flag;
    const char *m_param;
};

/// Class wrapping a vector of option objects. Manages the parsing of
class options {
public:
    typedef option *iterator;
    typedef const option *const_iterator;
    
    /// Pass the arg info from main into this constructor to populate the options object.
    /// @param argc argument count
    /// @param argv array of c-string args
    options(int argc, const char *argv[]);
    options() : m_opts() {}
    
    // Info log
    void log() const;
    
    // Iteration and indexing
    
    /// Finds an option with a particular flag
    /// @param flag the flag to check
    /// @param opt [out] the option to receive
    /// @returns true if one was found, false if there was none
    bool find_flag(char flag, option *opt) const;
    
    /// Returns a list of every option with a flag. Each may or may not have a parameter attached.
    options flags() const;
    
    /// Returns a list of params without flags.
    options params() const;
    
    bool empty() const { return m_opts.empty(); }
    const_iterator cbegin() const { return &(*m_opts.cbegin()); }
    const_iterator cend() const   { return &(*m_opts.cend()); }
    iterator begin() { return &(*m_opts.begin()); }
    iterator end() { return &(*m_opts.end()); }
    size_t size() const { return m_opts.size(); }
    const option &operator[](int index) const { return m_opts[index]; }
    const option &at(int index) const { return m_opts.at(index); }
private:
    options(std::vector<option> opts) : m_opts(std::move(opts)) {}
    std::vector<option> m_opts;
};

inline void
option::log() const
{
    printf("[%i]", index());
    if (has_flag())
        printf(" -%c", flag());
    if (has_param())
        printf(" %s", param());
    printf("\n");
}

inline
options::options(int argc, const char *argv[]) : m_opts()
{
    for (int i = 0; i < argc; ++i)
    {
        char flag = '\0';
        const char *param = nullptr;
        int ind = -1;
        
        size_t param_length = strlen(argv[i]);
        
        if (param_length >= 2 && argv[i][0] == '-') // is flag
        {
            if (i < argc - 1 && argv[i + 1][0] != '-') // has flagged param
            {
                param = argv[i + 1];
                ind = i;
                flag = argv[i][1];
                ++i; // we consumed the next param, so increase counter by one
            }
            else                                       // has only a flag
            {
                ind = i;
                flag = argv[i][1];
            }
        }
        else                                           // is a lone param
        {
            param = argv[i];
            ind = i;
        }
        
        // commit option
        m_opts.emplace_back(ind, flag, param);
    }
}

inline void
options::log() const
{
    if (empty())
    {
        printf("No options available.\n");
        return;
    }
    
    for (const option &o : m_opts)
    {
        o.log();
    }
}

inline bool
options::find_flag(char flag, option *opt) const
{
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
options::params() const
{
    std::vector<option> ret;
    for (const option &o : m_opts)
    {
        if (o.is_param())
            ret.emplace_back(o);
    }
    
    return options(std::move(ret));
}


#endif /* ai_options_hpp */
