/**
 * @file iparser.hpp
 * @brief Description of the interface class "IParser".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef IPARSER_HPP_
#define IPARSER_HPP_

/** @brief The namespace of the JOIN SERVER project */
namespace join_server {
/** @brief The namespace of the "Proto" */
namespace proto {

/** @brief The "IParser" class */
class iparser {
public:
  /** @brief Constructor by default */
  iparser() = default;

  /** @brief Disstructor by default */
  virtual ~iparser() = default;

  /** @brief Run of handler */
  virtual void exec() noexcept = 0;
};

} /* proto:: */
} /* join_server:: */

#endif /* IPARSER_HPP_ */
