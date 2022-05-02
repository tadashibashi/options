#include "options.hpp"
#include <iostream>
#include <sstream>

int test_main(int argc, char *argv[]);
template <typename T>
void assert_equal(T actual, T expected, const char *test_name);
template <> 
void assert_equal<const char *>(const char *actual, const char *expected, const char *test_name);
template <>
void assert_equal<bool>(bool actual, bool expected, const char *test_name);

static int tests_passed;
static int tests_ran;

static std::stringstream errors;

int test_main(int argc, char *argv[])
{
    const options opts(argc, argv);

    std::cout << "========== Options Tests ==========\n";
    // Option count is correct
    {
        assert_equal((int)opts.size(), 13, "Size is correct");
        assert_equal(opts.empty(), false, "Not empty");

        options empty_opts;
        assert_equal(empty_opts.empty(), true, "Empty opts");
    }

    // Has flag
    {
        assert_equal(opts.has_flag('l'), false, "Does not have flag 'l'");
        assert_equal(opts.has_flag('n'), true, "Has flag 'n'");
    }

    // Options match
    //    const char *argv[argc] {
    //            "program",
    //            "-o", "test_file.txt",
    //            "-f",
    //            "-n", "10",
    //            "-q", "99999999999999999999999999999999999999999999999999999999999999999999999",
    //            "-r", "-99999999999999999999999999999999999999999999999999999999999999999999999",
    //            "-b", "yes",
    //            "-c", "false",
    //            "-d", "true",
    //            "-e", "no",
    //            "-g", "1",
    //            "-h", "0",
    //            "-h", "20"
    //    };

    {
        // option 0
        assert_equal(opts[0].has_flag(), false, "option 0 has no flag");
        assert_equal(opts[0].has_param(), true, "option 0 has a param");
        assert_equal(opts[0].param(), "program", "option 0's param is \"program\"");
        assert_equal(opts[0].flag(), '\0', "option 0's param is a null terminator");
        assert_equal(opts[0].is_param_only(), true, "option 0 is a param-only option");
        assert_equal(opts[0].is_flag_only(), false, "option 0 is not a flag-only option");
        assert_equal(opts[0].is_flagged_param(), false, "option 0 is not a flagged param option");
        // option 1
        assert_equal(opts[1].has_flag(), true, "option 1 has a flag");
        assert_equal(opts[1].has_param(), true, "option 1 has a param");
        assert_equal(opts[1].param(), "test_file.txt", "option 1's param is \"test_file.txt\"");
        assert_equal(opts[1].flag(), 'o', "option 1's flag is \"o\"");
        assert_equal(opts[1].is_param_only(), false, "option 1 is not a param-only option");
        assert_equal(opts[1].is_flag_only(), false, "option 1 is not a flag-only option");
        assert_equal(opts[1].is_flagged_param(), true, "option 1 is a flagged param option");
        // option 2
        assert_equal(opts[2].has_flag(), true, "option 2 has a flag");
        assert_equal(opts[2].has_param(), false, "option 2 has a param");
        assert_equal(opts[2].param(), (const char *)nullptr, "option 2's param is empty string");
        assert_equal(opts[2].flag(), 'f', "option 2's flag is \"f\"");
        assert_equal(opts[2].is_param_only(), false, "option 2 is not a param-only option");
        assert_equal(opts[2].is_flag_only(), true, "option 2 is a flag-only option");
        assert_equal(opts[2].is_flagged_param(), false, "option 2 is not a flagged param option");
        // option 3
        assert_equal(opts[3].has_flag(), true, "option 3 has a flag");
        assert_equal(opts[3].has_param(), true, "option 3 has a param");
        assert_equal(opts[3].param(), "10", "option 3's param is \"10\"");
        assert_equal(opts[3].flag(), 'n', "option 3's flag is \"n\"");
        assert_equal(opts[3].is_param_only(), false, "option 3 is not a param-only option");
        assert_equal(opts[3].is_flag_only(), false, "option 3 is not a flag-only option");
        assert_equal(opts[3].is_flagged_param(), true, "option 3 is a flagged param option");
        // option 4
        assert_equal(opts[4].has_flag(), true, "option 4 has a flag");
        assert_equal(opts[4].has_param(), true, "option 4 has a param");
        assert_equal(opts[4].param(), "99999999999999999999999999999999999999999999999999999999999999999999999",
                     "option 4's param is as expected");
        assert_equal(opts[4].flag(), 'q', "option 4's flag is \"q\"");
        assert_equal(opts[4].is_param_only(), false, "option 4 is not a param-only option");
        assert_equal(opts[4].is_flag_only(), false, "option 4 is not a flag-only option");
        assert_equal(opts[4].is_flagged_param(), true, "option 4 is a flagged param option");
        // option 5
        assert_equal(opts[5].has_flag(), true, "option 5 has a flag");
        assert_equal(opts[5].has_param(), true, "option 5 has a param");
        assert_equal(opts[5].param(), "-99999999999999999999999999999999999999999999999999999999999999999999999",
                     "option 5's param is as expected");
        assert_equal(opts[5].flag(), 'r', "option 5's flag is \"r\"");
        assert_equal(opts[5].is_param_only(), false, "option 5 is not a param-only option");
        assert_equal(opts[5].is_flag_only(), false, "option 5 is not a flag-only option");
        assert_equal(opts[5].is_flagged_param(), true, "option 5 is a flagged param option");

    }

    // Iterate through each option
    {
        char flags[] = {'\0', 'o', 'f', 'n', 'q', 'r', 'b', 'c', 'd', 'e', 'g', 'h', 'h'};
        size_t count = 0;
        bool flags_correct = true;
        for (const option &o : opts)
        {
            if (flags_correct && o.flag() != flags[count])
                flags_correct = false;
            ++count;
        }
        assert_equal(count, opts.size(), "Iterator count is equal to size");
        assert_equal(flags_correct, true, "Iterated through options with correct flags");
    }

    // begin and end are accurate
    {
        assert_equal(opts.begin(), &opts[0], "begin gets ptr to opt[0]");
        assert_equal(opts.end(), &opts[0] + opts.size(), "end gets ptr after last opt");
    }

    // Get options with flag
    {
        // two options: 'h' flag
        options h_options;
        bool result = opts.get_options('h', &h_options);
        assert_equal(result, true, "get_options received valid option(s) with 'h' flag");
        assert_equal(h_options.size(), (size_t)2, "get_options received two options");
        bool all_are_h_options = true;
        for (const option &o : h_options)
        {
            if (o.flag() != 'h')
            {
                all_are_h_options = false;
                break;
            }
        }
        assert_equal(all_are_h_options, true, "get_options received all 'h' flagged options");

        // one option: 'n' flag
        options n_options;
        result = opts.get_options('n', &n_options);
        assert_equal(result, true, "get_options received valid option(s) with 'n' flag");
        assert_equal(n_options.size(), (size_t)1, "get_options received one 'n' flag option");

        // zero options: 'p' flag
        options p_options;
        result = opts.get_options('p', &p_options);
        assert_equal(result, false, "get_options did not receive valid options with 'p' flag");
        assert_equal(p_options.empty(), true, "get_options received 0 options on false return");
    }

    // Find a specific flag's parameter: found
    {
        const char *filepath = "default_file.txt";
        opts.get_param('o', &filepath);
        assert_equal(filepath, "test_file.txt", "Find parameter string");
    }

    // Find a specific flag's parameter: not found
    {
        const char *filepath = "default_file.txt";
        opts.get_param('z', &filepath);
        assert_equal(filepath, "default_file.txt", "Out-string is un-mutated when flag missing");
    }

    // Search for numeric flag
    {
        int number = -1;
        long long_num = -1;
        bool result = opts.get_param('n', &number);
        assert_equal(number, 10, "Integer parsed from param");
        assert_equal(result, true, "get_param int: returns true on successful parse");
        result = opts.get_param('n', &long_num);
        assert_equal(long_num, 10L, "Long integer parsed from param");
        assert_equal(result, true, "get_param long: returns true on successful parse");
    }

    // find_param int overload returns false with non-numeric param
    {
        int number = -1;
        long long_num = -1L;
        bool result = opts.get_param('o', &number);
        assert_equal(result, false, "get_param returns false non-int");
        assert_equal(number, -1, "get_param int un-mutated on false");
        result = opts.get_param('o', &long_num);
        assert_equal(result, false, "get_param returns false non-long");
        assert_equal(long_num, -1L, "get_param long un-mutated on false");
    }

    // find_param int overload returns false when out of range > max val
    {
        int number = -1;
        long long_num = -1;
        bool result = opts.get_param('q', &number);
        assert_equal(result, false, "get_param int: returns false on out of range > max val");
        assert_equal(number, -1, "get_param int: un-mutated on false");
        assert_equal(errno, ERANGE, "get_param int: errno set to ERANGE");
        result = opts.get_param('q', &long_num);
        assert_equal(result, false, "get_param long: returns false on out of range > max val");
        assert_equal(long_num, -1L, "get_param long: un-mutated on false");
        assert_equal(errno, ERANGE, "get_param long: errno set to ERANGE");
    }

    // find_param int overload returns false when out of range < min val
    {
        int number = -1;
        long long_num = -1;
        bool result = opts.get_param('r', &number);
        assert_equal(result, false, "get_param int: returns false on out of range < min val");
        assert_equal(errno, ERANGE, "get_param int: errno set to ERANGE");
        result = opts.get_param('r', &long_num);
        assert_equal(result, false, "get_param long: returns false on out of range < min val");
        assert_equal(errno, ERANGE, "get_param long: errno set to ERANGE");
    }

    // find_param bool
    {
        bool check;
        bool result;

        result = opts.get_param('b', &check);
        assert_equal(result, true, "get_param bool: \"yes\" returns true");
        assert_equal(check, true, "get_param bool: \"yes\" str gets true");
        result = opts.get_param('c', &check);
        assert_equal(result, true, "get_param bool: \"false\" returns true");
        assert_equal(check, false, "get_param bool: \"false\" str gets false");
        result = opts.get_param('d', &check);
        assert_equal(result, true, "get_param bool: \"true\" returns true");
        assert_equal(check, true, "get_param bool: \"true\" str gets true");
        result = opts.get_param('e', &check);
        assert_equal(result, true, "get_param bool: \"no\" returns true");
        assert_equal(check, false, "get_param bool: \"no\" str results in false");
        result = opts.get_param('g', &check);
        assert_equal(result, true, "get_param bool: \"1\" returns true");
        assert_equal(check, true, "get_param bool: \"1\" str gets true");
        result = opts.get_param('h', &check);
        assert_equal(result, true, "get_param bool: \"0\" returns true");
        assert_equal(check, false, "get_param bool: \"0\" str gets false");

        result = opts.get_param('q', &check);
        assert_equal(result, false, "get_param bool: \"999....\" returns false");
        result = opts.get_param('f', &check);
        assert_equal(result, false, "get_param bool: nullptr returns false");
        result = opts.get_param('n', &check);
        assert_equal(result, false, "get_param bool: \"10\" returns false");
    }

    // Log
    {
        opts.log();
        assert_equal(std::cout.good(), true, "std::cout is good");
    }

    printf("\nTotal %i/%i tests passed.\n", tests_passed, tests_ran);

    if (tests_passed < tests_ran)
        printf("Tests failed:\n%s", errors.str().c_str());
    else
        printf("All tests passed!\n");

    return 0;
}


int main ()
{
    // Create test flags
    const int argc = 24;
    const char *argv[argc] {
        "program",
        "-o", "test_file.txt",
        "-f",
        "-n", "10",
            "-q", "99999999999999999999999999999999999999999999999999999999999999999999999",
            "-r", "-99999999999999999999999999999999999999999999999999999999999999999999999",
            "-b", "yes",
            "-c", "false",
            "-d", "true",
            "-e", "no",
            "-g", "1",
            "-h", "0",
            "-h", "20"
    };
    
    return test_main(argc, (char **)argv);
}


template <typename T>
void assert_equal(T actual, T expected, const char *test_name)
{
    if (actual == expected)
    {
        ++tests_passed;
        std::cout << " #" << tests_ran << ' ' <<
            "[" << test_name << "] passed.\n";
    }
    else
    {
        errors << " #" << tests_ran << ' ' << "=====> [" << test_name <<
            "] FAILED: Expected " << expected << ", but got " << actual <<'\n';
    }

    ++tests_ran;
}

template <> 
void assert_equal<const char *>(const char *actual, const char *expected, const char *test_name)
{
    if (!actual || !expected)
    {
        if (!actual && !expected)
        {
            ++tests_passed;
            std::cout << " #" << tests_ran << ' ' <<
                "[" << test_name << "] passed.\n";
        }
        else
        {
            errors << " #" << tests_ran << ' '
                << "=====> [" << test_name << "] FAILED\n";
        }

    }
    else if ((actual[0] == '\0' && expected[0] == '\0') || strcmp(actual, expected) == 0)
    {
        ++tests_passed;
        std::cout << " #" << tests_ran << ' '
            << "[" << test_name << "] passed.\n";
    }
    else
    {
        errors << " #" << tests_ran << ' '
            << "=====> [" << test_name << "] FAILED: Expected " << expected <<
            ", but got " << actual << '\n';
    }

    ++tests_ran;
}

template <>
void assert_equal<bool>(bool actual, bool expected, const char *test_name)
{
    if (actual == expected)
    {
        ++tests_passed;
        std::cout << " #" << tests_ran << ' ' << "[" << test_name <<
            "] passed\n";
    }
    else
    {
        errors << " #" << tests_ran << ' ' << "=====> [" << test_name <<
            "] FAILED: Expected " << (expected ? "TRUE" : "FALSE") <<
            ", but got " << (actual ? "TRUE" : "FALSE") << '\n';
    }

    ++tests_ran;
}