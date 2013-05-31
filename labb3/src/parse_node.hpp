#ifndef PARSE_NODE_HPP
#define PARSE_NODE_HPP

#include <string>
#include <vector>
#include <functional>

namespace game {

  struct ParseData {
    std::string line;
		void * user_data;
  };

  class ParseNode {
    public:
			ParseNode(const std::string &cmd, std::function<void(ParseData &data)> func, const std::vector<ParseNode> &children);
      void add_child(const ParseNode &node);

      /* Matches string for things in this node,
       * removes the match from the string if found
       */
      bool match(std::string &str) const;

      virtual bool exec(ParseData &data) const;

			void set_func(std::function<void(ParseData &data)> func);

      static bool parse(const ParseNode &parse_tree_root, const std::string &line, void * user_data = nullptr);

			const std::vector<ParseNode> &children() const;
			const std::string &cmd() const;

      virtual ~ParseNode();

    protected:
      std::string m_cmd;
      std::vector<ParseNode> m_children;
      std::function<void(ParseData &data)> m_func;
  };
};

#endif
