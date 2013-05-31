#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "parse_node.hpp"
#include "game.hpp"

#include <algorithm>

static std::string trim(std::string s) {
	size_t begin_str = s.find_first_not_of(" \t\n\r");
	if(begin_str != std::string::npos) {
		size_t last = s.find_last_not_of(" \t\n\r");
		if(last >= begin_str) {
			std::string trimmed = s.substr(begin_str, last - begin_str + 1);
			return trimmed;
		} else {
			return "";
		}
	} else {
		return "";
	}
}

static void erase_word(std::string &str, size_t pos, size_t len) {
	str.erase(pos, str.find(" ", pos + len) - pos);
	str = trim(str);
}

namespace game {

	ParseNode::ParseNode(const std::string &cmd, std::function<void(ParseData &data)> func, const std::vector<ParseNode> &children)
    : m_cmd(Game::lowercase(cmd))
		, m_children(children)
    , m_func(func) {
  }

  void ParseNode::add_child(const ParseNode &node) {
    m_children.push_back(node);
  }

  bool ParseNode::exec(ParseData &data) const {
    for(const ParseNode &node : m_children) {
      if(node.match(data.line)) {
         return node.exec(data);
      }
    }
		if(m_func != nullptr) m_func(data);

    return m_children.size() == 0; /* leaf or not */
  }

  bool ParseNode::match(std::string &str)  const {
    size_t pos = str.find(m_cmd);
    if(pos != str.npos) {
			erase_word(str, pos, m_cmd.length());
      return true;
    } else {
      return false;
    }
  }

	void ParseNode::set_func(std::function<void(ParseData &data)> func) {
		m_func = func;
	}

  bool ParseNode::parse(const ParseNode &parse_tree_root, const std::string &line, void * user_data) {
    ParseData data;
    data.line = Game::lowercase(line);
		data.user_data = user_data;
    return parse_tree_root.exec(data);
  }

  ParseNode::~ParseNode() { }

	const std::string &ParseNode::cmd() const {
		return m_cmd;
	}

	const std::vector<ParseNode> &ParseNode::children() const {
		return m_children;
	}
};
