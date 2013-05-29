#ifndef PARSE_NODE_HPP
#define PARSE_NODE_HPP

#include <string>
#include <vector>
#include <functional>

namespace game {

  struct ParseData {
    std::string line;
    std::vector<void*> data;
  };

  class ParseNode {
    public:
      ParseNode(const std::string &cmd);
      ParseNode(const std::string &cmd, std::function<void(ParseData data)>);
      void add_child(ParseNode * node);

      /* Matches string for things in this node,
       * removes the match from the string if found
       */
      bool match(std::string &str) const;

      virtual bool exec(ParseData data) const;

      static void parse(const ParseNode &parse_tree_root, const std::string &line);

      virtual ~ParseNode();
    protected:
      std::string m_cmd;
      std::vector<ParseNode*> m_children;
      std::function<void(ParseData data)> m_func;
  };

  class ParseEndNode : public ParseNode {
    public:
      ParseEndNode(const std::string &cmd, std::function<void(ParseData data)>);
      virtual bool exec(ParseData data) const;
      virtual ~ParseEndNode() {};
  };
};

#endif
