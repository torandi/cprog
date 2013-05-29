#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <random>

class Config;

struct error_info_t;

class ConfigNode {
	public:
		enum type_t {
			NODE_SCALAR,
			NODE_SEQUENCE,
			NODE_MAPPING
		} type;

		const std::string &parse_string() const;
		int parse_int() const;
		float parse_float() const;

		bool parse_bool() const;

		const std::vector<const ConfigNode*> &list() const;
		const std::map<std::string, const ConfigNode*> &map() const;

		/*
		 * Returns the tag for the node
		 * If the node does not have a tag this returns the empty string
		 */
		const std::string &tag() const;

		/*
		 * Returns true if the node has a tag
		 */
		bool has_tag() const;

		const std::string &anchor() const;

		bool has_anchor() const;

		/*
		 * These may only be called on NODE_MAPPING
		 */
		const ConfigNode * find(const std::string &path, bool fail_on_not_found = false) const;
		const ConfigNode &operator[](const std::string &path) const;
		ConfigNode get(const std::string &path, const std::string &default_value) const;

		void print(std::string indent = "") const;


		~ConfigNode();
	private:
		ConfigNode(type_t type_);

		/**
		 * Implement these to make sure is_reference is set to true
		 * (to prevent extra deletion
		 *
		 * But they are still private, since you shouldn't use these!
		 */
		ConfigNode(const ConfigNode &node);
		ConfigNode &operator=(const ConfigNode &node);


		/*
		 * Calls {mapping,sequence}_add depending on type
		 * key is irrelevant if type is sequence
		 * @param error_info: for printing errors
		 */
		void add_child(const error_info_t &error_info, const std::string &key, ConfigNode * node);

		/*
		 * Add to mapping and sequence nodes
		 * These assume that you already have verified correct node type
		 * @param error_info: for printing errors
		 */
		void mapping_add(const error_info_t &error_info, const std::string &key, ConfigNode * node);
		void sequence_add(ConfigNode * node);

		std::map<std::string, ConfigNode*> mapping;
		std::vector<ConfigNode*> sequence;
		mutable std::vector<const ConfigNode*> exposed_sequence;
		mutable std::map<std::string, const ConfigNode*> exposed_mapping;
		std::string scalar;

		std::string node_tag;
		bool node_has_tag;


		std::string node_anchor;
		bool node_has_anchor;

		bool is_reference;

    static std::default_random_engine generator;

		friend class Config;
};

class Config {
	Config(ConfigNode * node);
	std::shared_ptr<ConfigNode> root_;
	public:
		static Config from_filename(const std::string &file);
		static Config from_string(const std::string &str);
		~Config();

		/*
		 * Find returns nullptr if not found
		 */
		const ConfigNode * find(const std::string &path, bool fail_on_not_found = false) const;
		const ConfigNode &operator[](const std::string &path) const;
		ConfigNode get(const std::string &path, const std::string &default_value) const;
		void print() const;

		const ConfigNode &root() const;

		/*
		 * Helper for parsers:
		 *
		 * Split on any of the chars in search
		 * set keep to true to keep splited char, false removes it
		 */
		static std::vector<std::string> split(const std::string &str, const std::string &search, bool keep=false);
	private:
		static Config parse(const std::string &context, const char * data, size_t size);
};


#endif
