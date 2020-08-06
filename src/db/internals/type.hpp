/**
 * @file type.hpp
 * @brief Definition of internal's type.
 *
 * @details
 * Description of internal database types.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef INTERNALS_TYPE_HPP_
#define INTERNALS_TYPE_HPP_

#include <map>
#include <string>
#include <vector>

namespace join_server {
namespace db {
namespace internals {

using row_t = std::vector<std::string>;
using storage_t = std::map<int, row_t>;

} /* internals:: */

using row_t = internals::row_t;
using storage_t = internals::storage_t;

} /* db:: */
} /* join_server:: */

#endif /* INTERNALS_TYPE_HPP_ */
