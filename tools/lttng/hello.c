#include <stdio.h>
#include "hello-tp.h"

int main(int argc, char *argv[])
{
    int x;

    puts("Hello, World!\nPress Enter to continue...");

    /*
     * The following getchar() call is only placed here for the purpose
     * of this demonstration, to pause the application in order for
     * you to have time to list its tracepoints. It is not
     * needed otherwise.
     */
    getchar();

    /*
     * A tracepoint() call.
     *
     * Arguments, as defined in hello-tp.h:
     *
     * 1. Tracepoint provider name   (required)
     * 2. Tracepoint name            (required)
     * 3. my_integer_arg             (first user-defined argument)
     * 4. my_string_arg              (second user-defined argument)
     *
     * Notice the tracepoint provider and tracepoint names are
     * NOT strings: they are in fact parts of variables that the
     * macros in hello-tp.h create.
     */
    tracepoint(hello_world, my_first_tracepoint, 23, "hi there!");

    for (x = 0; x < argc; ++x) {
        tracepoint(hello_world, my_first_tracepoint, x, argv[x]);
    }

    puts("Quitting now!");
    tracepoint(hello_world, my_first_tracepoint, x * x, "x^2");

    return 0;
}

