#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "parse_node.hpp"
#include "game.hpp"

#include <algorithm>

namespace game {

  ParseNode::ParseNode(const std::string &cmd)
    : m_cmd(Game::lowercase(cmd))
    , m_func(nullptr) {
  }

  ParseNode::ParseNode(const std::string &cmd, std::function<void(ParseData data)> func)
    : m_cmd(Game::lowercase(cmd))
    , m_func(func) {
  }

  ParseEndNode::ParseEndNode(const std::string &cmd, std::function<void(ParseData data)> func)
    : ParseNode(cmd, func) {
  }

  void ParseNode::add_child(ParseNode * node) {
    m_children.push_back(node);
  }

  bool ParseNode::exec(ParseData data) const {
    if(m_func != nullptr) m_func(data);

    for(ParseNode * node : m_children) {
      if(node->match(data.line)) {
         return node->exec(data);
      }
    }
    return false;
  }

  bool ParseEndNode::exec(ParseData data) const {
    m_func(data);
    return true;
  }

  bool ParseNode::match(std::string &str)  const {
    size_t pos = str.find(m_cmd);
    if(pos != str.npos) {
      str.erase(pos, m_cmd.length());
      return true;
    } else {
      return false;
    }
  }

  void ParseNode::parse(const ParseNode &parse_tree_root, const std::string &line) {
    ParseData data;
    data.line = Game::lowercase(line);
    parse_tree_root.exec(data);
  }

  ParseNode::~ParseNode() {
    std::for_each(m_children.begin(), m_children.end(), [](ParseNode * n) { delete n; });
  }
};
