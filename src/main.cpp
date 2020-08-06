/**
 * @file main.cpp
 * @brief Main file in this project.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

/* See the license in the file "LICENSE.txt" in the root directory. */

#include <boost/program_options.hpp>
#include <iostream>

#include "common/taskpool.hpp"
#include "common/tsqueue.hpp"
#include "core/server.hpp"
#include "proto/parser.hpp"

namespace {

void get_param(const int argc, const char* const argv[], unsigned short& port) {
  namespace po = boost::program_options;

  // clang-format off
  po::options_description desc("Options: ");
  desc.add_options()
      ("help,h", "this help.")
      ("port,p", po::value<unsigned short>(), "set server port");
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help") || vm.count("h")) {
    std::cout << desc << "\n";
    exit(0);
  }

  if (vm.count("port"))
    port = vm["port"].as<unsigned short>();
  else if (vm.count("p"))
    port = vm["p"].as<unsigned short>();
  else
    throw std::invalid_argument("Port was not set");
}

} /* :: */

/** @brief Main entry point */
int main(int argc, const char* argv[]) {
  using namespace join_server;
  unsigned short port{0};

  try {
    get_param(argc, argv, port);
  }
  catch (const std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  join_server::common::taskpool_ptr_t pool = std::make_shared<join_server::common::taskpool_t>();

  auto server = std::make_shared<core::server>(port, pool);
  server->exec();

  return EXIT_SUCCESS;
}
