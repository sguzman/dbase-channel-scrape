#include <algorithm>
#include <string>
#include <random>
#include <vector>
#include <iostream>

#include <pqxx/pqxx>
#include <gq/Document.h>
#include <gq/Node.h>


#include "curl.hxx"

inline static unsigned long max_pages() noexcept {
  const auto text{Curl::get("https://dbase.tube/chart/channels/subscribers/all")};

  CDocument doc{};
  doc.parse(text);

  CSelection tags = doc.find("li.ffwd a[href^=\"/chart/channels/subscribers/all?page=\"]");
  const auto path{tags.nodeAt(1).attribute("href")};
  const auto equal_sign{path.rfind('=')};
  const auto num_str{path.substr(equal_sign + 1)};
  const auto num{std::stoul(num_str)};

  return num;
}

int main(int argc, char* argv[]) noexcept {
  const auto pages{max_pages()};

  std::vector vec{pages};
  {
    for (auto i{1u}; i <= pages; ++i) {
      vec.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g{rd()};
    std::shuffle(vec.begin(), vec.end(), g);
  }

  for (const auto& v : vec) {

  }

  return 0;
}