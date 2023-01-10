#pragma once

#include <chrono>
#include <string_view>

#ifndef __DATE__
#error "__DATE__ is not defined, which is required for TODO_BEFORE"
#endif

#ifdef STATIC_TODO_GIT_USERNAME
constexpr std::string_view git_username = STATIC_TODO_GIT_USERNAME;
static_assert (! git_username.empty(), "Your git username is empty, check your git config or CMakeLists.txt");
#endif

namespace static_todo
{

consteval bool is_whitespace (char c) { return c == ' ' || c == '\t'; }

consteval bool is_null (char c) { return c == '\0'; }

consteval char to_lowercase (char c) { return c >= 'A' && c <= 'Z' ? char (c + ('a' - 'A')) : c; }

// Match string
consteval bool words_match (const char* a, const char* b)
{
    if (a == nullptr || b == nullptr)
        return false;

    while (! is_null (*a) && ! is_null (*b))
    {
        if (to_lowercase (*a) != to_lowercase (*b))
            return false;

        ++a;
        ++b;
        if (is_whitespace (*a) || is_whitespace (*b))
            return true;
    }
    return is_null (*a) && is_null (*b);
}

// Write a function that returns pointers to the start of each word
// Return the nullptr if there is only one word
consteval const char* get_next_word (const char* str)
{
    while (! is_null (*str) && ! is_whitespace (*str))
        ++str;

    while (! is_null (*str) && is_whitespace (*str))
        ++str;

    return is_null (*str) ? nullptr : str;
}

consteval size_t count_words (std::string_view str)
{
    size_t num_words = 0;
    if (str.empty())
        return num_words;

    for (auto i : str)
    {
        if (is_whitespace (i))
            ++num_words;
    }
    return num_words + 1;
}

// Write a function that iterates over all pairs of words between two strings and returns true if
// any two strings match (case insensitive)
consteval bool any_two_words_match (const char* a, const char* b)
{
    const auto a_num_words = count_words (a);
    const auto b_num_words = count_words (b);

    const auto b_start = b;

    for (size_t i = 0; i < a_num_words; ++i)
    {
        for (size_t j = 0; j < b_num_words; ++j)
        {
            if (words_match (a, b))
                return true;

            b = get_next_word (b);
            if (b == nullptr)
                b = b_start;
        }
        a = get_next_word (a);
        b = b_start;
    }

    return false;
}

constexpr const std::string_view months[12] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

consteval int year_string_to_int (const std::string_view year)
{
    return (year[0] - '0') * 1000 + (year[1] - '0') * 100 + (year[2] - '0') * 10 + (year[3] - '0');
}

consteval int get_year_from_string (const std::string_view date)
{
    const char* year = date.data();

    // Walk over the first two words and point at the start of the year
    // NB There could be multiple spaces between the words!
    int num_words = 0;
    bool was_prev_char_space = false;
    while (num_words < 2)
    {
        if (is_whitespace (*year))
        {
            was_prev_char_space = true;
        }
        else
        {
            if (was_prev_char_space)
            {
                num_words++;
                year--;
            }
            was_prev_char_space = false;
        }
        year++;
    }

    return year_string_to_int (year);
}

consteval bool is_a_month (std::string_view month)
{
    for (auto i : months)
    {
        if (month[0] == i[0] && month[1] == i[1] && month[2] == i[2] && is_null (month[3]))
            return true;
    }
    return false;
}

// Returns the month number (1..12) from an input string
consteval int get_month_from_string (const std::string_view date)
{
    // Find matching month
    for (int i = 0; i < 12; i++)
    {
        const char* m = months[i].data();
        if (m[0] == date[0] && m[1] == date[1] && m[2] == date[2])
            return i + 1;
    }
    return 0;
}

// Returns the month number (1..12) from an input string
consteval int get_day_from_string (const std::string_view date)
{
    const char* day = date.data();

    // Walk over the first word and point at the start of the year
    // NB There could be multiple spaces between the words!
    int num_words = 0;
    bool was_prev_char_space = false;
    while (num_words < 1)
    {
        if (is_whitespace (*day))
        {
            was_prev_char_space = true;
        }
        else
        {
            if (was_prev_char_space)
            {
                num_words++;
                day--;
            }
            was_prev_char_space = false;
        }
        day++;
    }

    if (is_whitespace (day[1]))
        return day[0] - '0';

    return (day[0] - '0') * 10 + (day[1] - '0');
}

using Date = std::chrono::year_month_day;

consteval Date toDate (int year, int month, int day = 1)
{
    return { std::chrono::year { year },
             std::chrono::month { static_cast<unsigned int> (month) },
             std::chrono::day { static_cast<unsigned int> (day) } };
}

consteval Date toDate (std::string_view date_str)
{
    return toDate (get_year_from_string (date_str), get_month_from_string (date_str), get_day_from_string (date_str));
}

consteval bool check_git_user (std::string_view current_git_username, std::string_view query_username)
{
    if (query_username == "*")
        return true;
    return any_two_words_match (current_git_username.data(), query_username.data());
}

consteval bool should_break_compilation (std::string_view system_date,
                                         int query_year,
                                         int query_month,
                                         std::string_view current_git_username,
                                         std::string_view query_username)
{
    const auto is_current_git_user = check_git_user (current_git_username, query_username);
    const auto is_after_deadline = toDate (system_date) >= toDate (query_year, query_month);
    return is_current_git_user && is_after_deadline;
}

consteval bool should_warn_about_upcoming_deadline (std::string_view system_date,
                                                    int query_year,
                                                    int query_month,
                                                    std::string_view current_git_username,
                                                    std::string_view query_username)
{
    const auto is_current_git_user = check_git_user (current_git_username, query_username);
    const auto days_until_deadline = (std::chrono::sys_days { toDate (query_year, query_month) } - std::chrono::sys_days { toDate (system_date) }).count();
    const auto is_approaching_deadline = days_until_deadline > 0 && days_until_deadline < 10;
    return is_current_git_user && is_approaching_deadline;
}

#if __has_attribute(diagnose_if) && STATIC_TODO_ENABLE_DEADLINE_APPROACHING_WARNINGS
consteval int static_warn (bool test) __attribute__ ((diagnose_if (! (test), "TODO_BEFORE deadline coming soon!", "warning")))
{
    return 0;
}
#else
consteval int static_warn (bool)
{
    return 0;
}
#endif
} // namespace static_todo

#if STATIC_TODO_DISABLE
#define TODO_BEFORE(month, year, user_query, msg)
#else

#define STATIC_TODO_CONCAT_INTERNAL(x, y) x##y
#define STATIC_TODO_CONCAT(x, y) STATIC_TODO_CONCAT_INTERNAL (x, y)
#define STATIC_TODO_UNIQUE_NAME(base) STATIC_TODO_CONCAT (base, __COUNTER__)

/**
 * TODO_BEFORE() inserts a compilation "time bomb" that will trigger a "TODO" build error a soon as
 * the given date is reached and the person assigned the TODO is the one trying to compile the code.
 *
 * This is useful to force attention on a specific piece of code that should not been forgotten
 * among a growing list of many other "TODO" comments...
 *
 * Example:
 *   TODO_BEFORE(Jan, 2019, "refactor to use std::optional<> once we compile in C++17 mode");
 */
#define TODO_BEFORE(month, year, user_query, msg)                                                                                                                              \
    static_assert (static_todo::is_a_month (#month), "Enter a month in the format Jan, Feb, Dec, etc.");                                                                       \
    static_assert (! static_todo::should_break_compilation (__DATE__, year, static_todo::get_month_from_string (#month), STATIC_TODO_GIT_USERNAME, user_query), "TODO: " msg); \
    auto STATIC_TODO_UNIQUE_NAME (static_todo_warner_) = static_todo::static_warn (! static_todo::should_warn_about_upcoming_deadline (__DATE__, year, static_todo::get_month_from_string (#month), STATIC_TODO_GIT_USERNAME, user_query))
#endif
