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
  {
    doc.parse(text);
  }

  CSelection tags{doc.find("li.ffwd a[href^=\"/chart/channels/subscribers/all?page=\"]")};
  const auto path{tags.nodeAt(1).attribute("href")};
  const auto equal_sign{path.rfind('=')};
  const auto num_str{path.substr(equal_sign + 1)};
  const auto num{std::stoul(num_str)};

  return num;
}

inline static std::vector<std::string> page(const unsigned long idx) noexcept {
  std::string url{"https://dbase.tube/chart/channels/subscribers/all"};

  if (idx > 1) {
    url += "?page=" + std::to_string(idx);
  }

  const auto text{Curl::get(url)};
  CDocument doc{};
  {
    doc.parse(text);
  }

  CSelection tags{doc.find("a[href^=\"/c/\"]")};
  std::vector<std::string> chans{};
  {
    for (auto i{0u}; i < tags.nodeNum(); ++i) {
      const auto href_raw{tags.nodeAt(i).attribute("href")};

      std::string href{};
      {
        for (auto j{0u}; j < 24; ++j) {
          href += href_raw[3 + j];
        }
      }

      chans.push_back(href);
    }
  }

  return chans;
}

int main(int argc, char* argv[]) noexcept {
  const auto pages{max_pages()};

  std::vector<unsigned long> vec{pages};
  {
    for (auto i{1u}; i <= pages; ++i) {
      vec.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g{rd()};
    std::shuffle(vec.begin(), vec.end(), g);
  }

  for (const auto& v : vec) {
    pqxx::connection C{"dbname=youtube user=postgres host=192.168.1.63 port=30339"};
    pqxx::work W{C};

    const auto chans{page(v)};
    for (const auto& c : chans) {
      std::cout << c << std::endl;

      std::string insert{"INSERT INTO youtube.entities.channels (serial) VALUES ('"};
      insert += c;
      insert += "') ON CONFLICT (serial) DO NOTHING;";

      W.exec(insert);
    }

    W.commit();
    C.disconnect();
    C.deactivate();
  }



  return 0;
}