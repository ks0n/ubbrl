#include <criterion/criterion.h>
#include <sys/types.h>

ssize_t term_strlen(const char *str);

Test(term_strlen, no_escape)
{
    cr_assert_eq(0, term_strlen(""));
    cr_assert_eq(4, term_strlen("ubsh"));
    cr_assert_eq(7, term_strlen("ubsh > "));
}

Test(term_strlen, escape_color_empty)
{
    cr_assert_eq(0, term_strlen("\x1B[33m"));
}

Test(term_strlen, escape_color_no_reset)
{
    cr_assert_eq(4, term_strlen("\x1B[33mubsh"));
}

Test(term_strlen, escape_color_plus_reset)
{
    cr_assert_eq(4, term_strlen("\x1B[33mubsh\x1B[0m"));
}
