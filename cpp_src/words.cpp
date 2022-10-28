#include "words.h"

#include <algorithm>
#include <stdexcept>
#include <cstdlib>

namespace low_index {

/// Parse a word like x1X2
Relator
_parse_numeric_word(
    const RankType rank,
    const std::string &word)
{
    Relator result;

    const char * p = word.c_str();
    while (*p) {
        const char sign = *p;
        if (sign != 'x' && sign != 'X') {
            throw std::domain_error(
                std::string("Bad word, expected 'x' or 'X', got '") +
                sign + "'");
        }
        ++p;
        if (*p < '0' || *p > '9') {
            throw std::domain_error(
                std::string("Expected number after 'x', got '") +
                *p + "'");
        }
        const long value = std::strtol(p, const_cast<char**>(&p), 10);
        if (value > rank || value < 1) {
            throw std::domain_error(
                std::string("Invalid generator ") +
                sign + std::to_string(value) +
                " for group of rank " +
                std::to_string(static_cast<int>(rank)));
        }
        if (sign == 'x') {
            result.push_back( value);
        } else {
            result.push_back(-value);
        }
    }

    return result;
}

// Parse a word like aB
Relator
_parse_alpha_word(
    const RankType rank,
    const std::string &word)
{
    Relator result;
    result.reserve(word.size());

    for (const char letter : word) {
        if (letter >= 'a' && letter <= 'z') {
            const RankType l = letter - 'a' + 1;
            if (l > rank) {
                throw std::domain_error(
                    std::string("Invalid generator '") + letter +
                    "' for group of rank " +
                    std::to_string(static_cast<int>(rank)));
            }
            result.push_back(l);
            continue;
        }
        if (letter >= 'A' && letter <= 'Z') {
            const RankType l = letter - 'A' + 1;
            if (l > rank) {
                throw std::domain_error(
                    std::string("Invalid generator '") + letter +
                    "' for group of rank " +
                    std::to_string(static_cast<int>(rank)));
            }
            result.push_back(-l);
            continue;
        }
        throw std::domain_error(
            std::string("Expected letter got '") + letter + "'");
    }

    return result;
}

Relator
parse_word(
    const RankType rank,
    const std::string &word)
{
    if (rank > 26) {
        return _parse_numeric_word(rank, word);
    } else {
        return _parse_alpha_word(rank, word);
    }
}

// Cyclically shift relator by i
static
Relator
_spin_relator(
    const Relator &r,
    const size_t i)
{
    Relator result;
    result.insert(result.end(), r.begin() + i, r.end());
    result.insert(result.end(), r.begin(), r.begin() + i);
    return result;
}

// Add all cyclic shifts of relator to result
static
void
_spin_relator(
    const Relator &r,
    std::vector<Relator> * const result)
{
    for (size_t i = 0; i < r.size(); i++) {
        result->push_back(_spin_relator(r, i));
    }
}

std::vector<Relator>
spin_short(const std::vector<Relator> &relators,
           const DegreeType max_degree)
{
    std::vector<Relator> result;

    if (relators.empty()) {
        return result;
    }
    
    size_t total_length = 0;
    for (const Relator &relator : relators) {
        total_length += relator.size();
    }
    const size_t n = relators.size();
    // Average length
    const size_t avg = (total_length + n - 1) / n;
    const size_t max_len = std::max<size_t>(avg, max_degree);

    for (const Relator &relator : relators) {
        if (relator.size() <= max_len) {
            _spin_relator(relator, &result);
        } else {
            result.push_back(relator);
        }
    }

    // Remove duplicates from the result.
    // E.g. a periodic relator such as abab will have
    // several identical cyclic shifts.
    std::sort(result.begin(), result.end());
    auto l = std::unique(result.begin(), result.end());
    result.erase(l, result.end());

    return result;
}

}
