#include <boost/program_options.hpp>
#include <iostream>
#include "server.hpp"

namespace {

struct param {
  unsigned short port{0};
  std::size_t bulk_size{0};
};

using param_t = param;

void get_param(const int argc, const char* const argv[], param_t& param) {
  namespace po = boost::program_options;

  // clang-format off
  po::options_description desc("Options: ");
  desc.add_options()
      ("help,h", "this help.")
      ("port,p", po::value<unsigned short>(), "set server port")
      ("size,s", po::value<std::size_t>(), "set size of bulk");
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help") || vm.count("h")) {
    std::cout << desc << "\n";
    exit(0);
  }

  if (vm.count("port"))
    param.port = vm["port"].as<unsigned short>();
  else if (vm.count("p"))
    param.port = vm["p"].as<unsigned short>();
  else
    throw std::invalid_argument("Port was not set");

  if (vm.count("size"))
    param.bulk_size = vm["size"].as<std::size_t>();
  else if (vm.count("s"))
    param.bulk_size = vm["s"].as<std::size_t>();
  else
    throw std::invalid_argument("Size of bulk was not set.");
}

} /* :: */

int main(int argc, const char* argv[]) {
  param_t params;

  try {
    get_param(argc, argv, params);
  }
  catch (const std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  auto server = std::make_shared<bulk_server::server>(params.port, params.bulk_size);
  server->start();

  return EXIT_SUCCESS;
}
