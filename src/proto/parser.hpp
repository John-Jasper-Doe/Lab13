/**
 * @file parser.hpp
 * @brief Discription of the "Parser" class.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <memory>
#include <string>
#include <vector>
#include "iparser.hpp"

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {

namespace core {
class client;
} /* core:: */

/** @brief The namespace of the "Proto" */
namespace proto {

/** @brief The parser class */
class parser final : public join_server::proto::iparser {
  using tokens_t = std::vector<std::string>;

  std::string data_string_;
  std::shared_ptr<core::client> client_;

public:
  /**
   * @brief Constructor with params
   * @param [in] data_string - string with data.
   * @param [in] client - pointer on client session.
   */
  explicit parser(std::string&& data_string, std::shared_ptr<core::client> client) noexcept;

  /** @brief The default distructor. */
  virtual ~parser() noexcept override;

  /** @brief Run of handler */
  virtual void exec() noexcept override;

protected:
  void proc_insert(tokens_t& tokens);
  void proc_truncate(tokens_t& tokens);
  void proc_intersect(tokens_t& tokens);
  void proc_symdiff(tokens_t& tokens);
};

} /* proto:: */
} /* join_server:: */

#endif /* PARSER_HPP_ */
