#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>

DOCTEST_SYMBOL_IMPORT void from_northstar();

int main(int argc, char** argv) {
    // force the use of a symbol from the driver so tests from it get registered
    from_northstar();

    trompeloeil::set_reporter([](
        trompeloeil::severity s,
        const char* file,
        unsigned long line,
        std::string const& msg)
        {
            auto f = line ? file : "[file/line unavailable]";
            if (s == trompeloeil::severity::fatal)
            {
                ADD_FAIL_AT(f, line, msg);
            }
            else
            {
                ADD_FAIL_CHECK_AT(f, line, msg);
            }
        }
    );

    doctest::Context context(argc, argv);
    int res = context.run();

    if (context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
