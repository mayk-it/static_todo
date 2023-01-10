#include <static_todo/static_todo.hpp>

// Tests for the static_todo.hpp header
// These are all static assertions, so if they fail to compile, the test has failed!
namespace static_todo
{
static_assert (is_whitespace (' '));
static_assert (is_whitespace ('\t'));
static_assert (! is_whitespace ('a'));

static_assert (year_string_to_int ("2018") == 2018);
static_assert (year_string_to_int ("2345") == 2345);

static_assert (get_year_from_string ("Jan  1 2018") == 2018);
static_assert (get_year_from_string ("Dec 1 2022") == 2022);
static_assert (get_year_from_string ("December \t 1st 2022") == 2022);
static_assert (get_year_from_string ("December 1st 2022") == 2022);
static_assert (get_year_from_string ("1st December  2022") == 2022);
static_assert (get_year_from_string ("1s t December 2022") != 2022); // breaks! four total spaces.

static_assert (is_a_month ("Jan"));
static_assert (is_a_month ("Feb"));
static_assert (is_a_month ("Mar"));
static_assert (! is_a_month ("April")); // Must be 3 chars
static_assert (is_a_month ("May"));
static_assert (! is_a_month ("jun")); // Must be title case

static_assert (get_month_from_string ("Jan  1 2018") == 1);
static_assert (get_month_from_string ("Feb 1 2022") == 2);
static_assert (get_month_from_string ("Mar \t 1st 2022") == 3);
static_assert (get_month_from_string ("Apr 1st 2022") == 4);
static_assert (get_month_from_string ("Nov 1st 2022") == 11);
static_assert (get_month_from_string ("Dec 1st 2022") == 12);

static_assert (get_day_from_string ("Jan  1 2018") == 1);
static_assert (get_day_from_string ("Feb 12 2022") == 12);
static_assert (get_day_from_string ("Mar \t 31st 2022") == 31);
static_assert (get_day_from_string ("Apr 08 2022") == 8);
static_assert (get_day_from_string ("Nov 19th 2022") == 19);
static_assert (get_day_from_string ("Dec 21st 2022") == 21);

static_assert (words_match ("hello", "hello"));
static_assert (words_match ("hello", "hello "));

static_assert (get_next_word ("hello world") != nullptr);
static_assert (get_next_word ("world") == nullptr);

static_assert (count_words ("hello world") == 2);
static_assert (count_words ("hello") == 1);
static_assert (count_words ("") == 0);

static_assert (any_two_words_match ("Billy Joel", "billy"));
static_assert (any_two_words_match ("Billy Joel", "joel jatin bernie"));
static_assert (any_two_words_match ("Stevie Wonder", "stevie"));
static_assert (any_two_words_match ("Richard Wright", "richard"));
static_assert (! any_two_words_match ("Richard Wright", "billy stevie"));

static_assert (! should_break_compilation ("Dec 12 2023", 2022, 11, "Billy Joel", "stevie"));
static_assert (should_break_compilation ("Dec 12 2023", 2022, 11, "Billy Joel", "billy"));
static_assert (! should_break_compilation ("Oct 12 2022", 2022, 11, "Billy Joel", "billy"));

static_assert (! should_warn_about_upcoming_deadline ("Dec 12 2023", 2022, 11, "Billy Joel", "stevie"));
static_assert (! should_warn_about_upcoming_deadline ("Dec 12 2023", 2022, 11, "Billy Joel", "billy"));
static_assert (should_warn_about_upcoming_deadline ("Oct 23 2022", 2022, 11, "Billy Joel", "billy"));
static_assert (! should_warn_about_upcoming_deadline ("Aug 22 2022", 2022, 11, "Billy Joel", "billy"));
} // namespace static_todo

int main() {}
