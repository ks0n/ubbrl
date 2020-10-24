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
	size_t len = term_strlen("\x1B[33m");
	cr_assert_eq(0, len, "Expected 0, got %ld", len);
}

Test(term_strlen, escape_color_no_reset)
{
	size_t len = term_strlen("\x1B[33mubsh");
	cr_assert_eq(4, len, "Expected 4, got %ld", len);
}

Test(term_strlen, escape_color_plus_reset)
{
	size_t len = term_strlen("\x1B[33mubsh\x1B[0m");
	cr_assert_eq(4, len, "Expected 4, got %ld", len);
}

Test(term_strlen, escape_color_unfinished)
{
	size_t len = term_strlen("\x1B[33ubsh");
	cr_assert_eq(8, len, "Expected 8, got %ld", len);
}
