/**
 * @file split.hpp
 * @brief Implementation of various split functions.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef SPLIT_HPP_
#define SPLIT_HPP_

#include <regex>
#include <string>

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the Common */
namespace common {

/**
 * @brief The function slices a string into substrings.
 *
 * @details
 * This function slices a given string into substrings and writes them to a
 * string vector. The line feed character ("<space>") is used as a separator.
 *
 * @note If the delimiter is at the beginning of the line, then the first
 * substring will be empty. That is, the line is empty before the separator.
 *
 * @param [in] str - input of data string.
 * @return Vector of substrings.
 */
inline std::vector<std::string> split(const std::string& str) {
  std::regex ws_re("\\s+");
  return std::vector<std::string>{std::sregex_token_iterator(str.begin(), str.end(), ws_re, -1),
                                  std::sregex_token_iterator()};
}

} /* common:: */
} /* join_server:: */

#endif /* SPLIT_HPP_ */
