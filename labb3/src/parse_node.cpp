#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "parse_node.hpp"
#include "game.hpp"

#include <algorithm>

namespace game {

	ParseNode::ParseNode(const std::string &cmd, std::function<void(ParseData data)> func, const std::vector<ParseNode> &children)
    : m_cmd(Game::lowercase(cmd))
		, m_children(children)
    , m_func(func) {
  }

  void ParseNode::add_child(const ParseNode &node) {
    m_children.push_back(node);
  }

  bool ParseNode::exec(ParseData data) const {
    for(const ParseNode &node : m_children) {
      if(node.match(data.line)) {
         return node.exec(data);
      }
    }
		if(m_func != nullptr) m_func(data);

    return m_children.size() > 0; /* leaf or not */
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

	void ParseNode::set_func(std::function<void(ParseData data)> func) {
		m_func = func;
	}

  void ParseNode::parse(const ParseNode &parse_tree_root, const std::string &line, void * user_data) {
    ParseData data;
    data.line = Game::lowercase(line);
		data.user_data = user_data;
    parse_tree_root.exec(data);
  }

  ParseNode::~ParseNode() {
    //std::for_each(m_children.begin(), m_children.end(), [](ParseNode * n) { delete n; });
  }
};
