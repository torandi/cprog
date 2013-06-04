#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdlib>
#include <cstdio>
#include <list>
#include <yaml.h>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <sstream>

#include "config.hpp"
#include "logging.hpp"
#include "string_utils.hpp"

std::default_random_engine ConfigNode::generator(std::chrono::system_clock::now().time_since_epoch().count());

struct error_info_t {
	const std::string * context;
	const yaml_event_t * event;
};

static void __FORMAT__(printf, 2,3) yaml_event_error(const error_info_t &error_info, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	char* message = nullptr;
	if ( vasprintf(&message, fmt, ap) == -1 ){
		fprintf(stderr, "memory allocation failed\n");
		abort();
	}

	Logging::fatal("[Config] Parse error in %s (%lu:%lu - %lu:%lu): %s\n", error_info.context->c_str(),
			error_info.event->start_mark.line, error_info.event->start_mark.column,
			error_info.event->end_mark.line, error_info.event->end_mark.column,
			message);

	free(message);

	va_end(ap);
}

static void add_anchor(const error_info_t &error_info, std::map<std::string, ConfigNode*> &anchor_mapping, ConfigNode * node) {
	if(node->has_anchor()) {
		anchor_mapping[node->anchor()] = node;
	}
}

/*
 * Prints error
 * @param context: filename or something describing the context
 * @param parser: the yaml parser
 */
static int yaml_error(const std::string &context, const yaml_parser_t &parser) {
	Logging::fatal("[ConfigNode] YAML error in %s (%lu:%lu): %s\n", context.c_str(), parser.context_mark.line, parser.context_mark.column, parser.problem);
	return 0;
}

ConfigNode::ConfigNode(const ConfigNode &node)
	: type(node.type)
	, node_has_tag(node.node_has_tag)
	, node_has_anchor(node.node_has_anchor)
	, is_reference(true)
{
	switch(type) {
		case NODE_MAPPING:
			mapping = node.mapping;
			break;
		case NODE_SEQUENCE:
			sequence = node.sequence;
			break;
		case NODE_SCALAR:
			scalar = node.scalar;
			break;
	}
}

ConfigNode &ConfigNode::operator=(const ConfigNode &node) {
	type = node.type;
	node_has_tag = node.node_has_tag;
	node_has_anchor = node.node_has_anchor;
	is_reference = true;
	switch(type) {
		case NODE_MAPPING:
			mapping = node.mapping;
			break;
		case NODE_SEQUENCE:
			sequence = node.sequence;
			break;
		case NODE_SCALAR:
			scalar = node.scalar;
			break;
	}
	return *this;
}


/* Emit constructors */

ConfigNode::ConfigNode(const char * str)
	: type(NODE_SCALAR)
	, scalar(str)
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false) {
}

ConfigNode::ConfigNode(const std::string &str)
	: type(NODE_SCALAR)
	, scalar(str)
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false) {
}
ConfigNode::ConfigNode(int i)
	: type(NODE_SCALAR)
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false) {
		std::ostringstream o;
		o << i;
		scalar = o.str();
}

ConfigNode::ConfigNode(float f)
	: type(NODE_SCALAR)
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false) {
		std::ostringstream o;
		o << f;
		scalar = o.str();
}

ConfigNode::ConfigNode(bool b)
	: type(NODE_SCALAR)
	, scalar(b ? "true" : "false")
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false) {
}

ConfigNode::ConfigNode(const std::map<std::string, ConfigNode *> &mapping)
	: type(NODE_MAPPING)
	, mapping(mapping)
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false) {
}

ConfigNode::ConfigNode(const std::vector<ConfigNode *> &sequence)
	: type(NODE_SEQUENCE)
	, sequence(sequence)
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false) {
}

/* End emit constructors */

ConfigNode::ConfigNode(ConfigNode::type_t type_)
	: type(type_)
	, node_has_tag(false)
	, node_has_anchor(false)
	, is_reference(false)
{};


ConfigNode::~ConfigNode() {
	if(!is_reference) {
		switch(type) {
			case NODE_MAPPING:
				for(auto it: mapping) {
					delete it.second;
			}
				break;
			case NODE_SEQUENCE:
				for(auto it : sequence) {
					delete it;
				}
				break;
			case NODE_SCALAR:
				break;
		}
	}
}



void ConfigNode::mapping_add(const error_info_t &error_info, const std::string &key, ConfigNode * node) {
	if(mapping.find(key) != mapping.end()) yaml_event_error(error_info, "Duplicate map entry %s", key.c_str());
	mapping[key] = node;
}

void ConfigNode::sequence_add(ConfigNode * node) {
	sequence.push_back(node);
}

void ConfigNode::add_child(const error_info_t &error_info, const std::string &key, ConfigNode * node) {
	switch(type) {
		case ConfigNode::NODE_MAPPING:
			mapping_add(error_info, key, node);
			break;
		case ConfigNode::NODE_SEQUENCE:
			sequence_add(node);
			break;
		case ConfigNode::NODE_SCALAR:
			yaml_event_error(error_info, "Can't add child nodes to scalar node");
			break;
	}
}

Config Config::from_filename(const std::string &filename) {
	std::ifstream file(std::string(srcdir) + "/" + filename);
	if(file.is_open()) {
		file.seekg(0, file.end);
		std::ifstream::pos_type size = file.tellg();
		char * data = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(data, size);
		file.close();

		Config config = parse(filename, data, size);

		delete[] data;

		return config;
	} else {
		Logging::fatal("[Config] Failed to open %s\n", filename.c_str());
	}
}

Config Config::from_string(const std::string &str) {
	return Config::parse(str.substr(0, 30) /* include 30 chars as "context" */, str.c_str(), str.size());
}

Config Config::parse(const std::string &context, const char * data, size_t size) {
	yaml_parser_t parser;
	yaml_event_t event;

	yaml_parser_initialize(&parser) || yaml_error(context, parser);

	yaml_parser_set_input_string(&parser, (const unsigned char*)data, size);

	/* Pre parsing */
	yaml_parser_parse(&parser, &event) || yaml_error(context, parser);

	if(event.type != YAML_STREAM_START_EVENT) Logging::fatal("[Config] %s doesn't start with stream start event\n", context.c_str());
	yaml_event_delete(&event);

	yaml_parser_parse(&parser, &event) || yaml_error(context, parser);
	if(event.type != YAML_DOCUMENT_START_EVENT) Logging::fatal("[Config] %s doesn't start with document start event\n", context.c_str());

	/* Begin parsing */

	ConfigNode * root = nullptr;
	ConfigNode * current = nullptr;

	std::list<ConfigNode*> node_stack;

	std::map<std::string, ConfigNode*> anchor_mapping;

	ConfigNode * node = nullptr;

	/* for mapping */
	bool has_key = false;
	std::string key_name;

	error_info_t error_info = {
		&context,
		&event
	};

	do {
		yaml_event_delete(&event);
		yaml_parser_parse(&parser, &event) || yaml_error(context, parser);

		switch(event.type) {
			case YAML_SCALAR_EVENT:
				if(current != nullptr) {
					if(current->type == ConfigNode::NODE_MAPPING && !has_key) {
						key_name = std::string((char*)event.data.scalar.value);
						has_key = true;
					} else {
						node = new ConfigNode(ConfigNode::NODE_SCALAR);

						node->node_has_anchor = event.data.scalar.anchor != nullptr;
						if(node->has_anchor()) node->node_anchor = std::string((char*)event.data.scalar.anchor);

						node->node_has_tag = event.data.scalar.tag != nullptr;
						if(node->has_tag()) node->node_tag = std::string((char*)event.data.scalar.tag);

						node->scalar = std::string((char*)event.data.scalar.value);

						add_anchor(error_info, anchor_mapping,node);

						current->add_child(error_info, key_name, node);
						has_key = false; /* Key is alway consumed */
					}
				} else {
					yaml_event_error(error_info, "Scalar element can't be root element");
				}
				break;
			case YAML_SEQUENCE_START_EVENT:
				node = new ConfigNode(ConfigNode::NODE_SEQUENCE);
				node->node_has_anchor = event.data.sequence_start.anchor != nullptr;
				if(node->has_anchor()) node->node_anchor = std::string((char*)event.data.sequence_start.anchor);

				node->node_has_tag = event.data.sequence_start.tag != nullptr;
				if(node->has_tag()) node->node_tag = std::string((char*)event.data.sequence_start.tag);

				add_anchor(error_info, anchor_mapping,node);

				if(current != nullptr) {
					if(current->type == ConfigNode::NODE_MAPPING && !has_key) yaml_event_error(error_info, "Sequence can't be used as mapping key");
					current->add_child(error_info, key_name, node);
					has_key = false; /* Key is alway consumed */
					node_stack.push_back(current);
					current = node;
				} else {
					if(root == nullptr) root = node;
					else yaml_event_error(error_info, "Multiple root elements, only one document per context supported!");
					current = node;
				}

				break;
			case YAML_MAPPING_START_EVENT:
				node = new ConfigNode(ConfigNode::NODE_MAPPING);
				node->node_has_anchor = event.data.mapping_start.anchor != nullptr;
			if(node->has_anchor()) node->node_anchor = std::string((char*)event.data.mapping_start.anchor);

				node->node_has_tag = event.data.mapping_start.tag != nullptr;
				if(node->has_tag()) node->node_tag = std::string((char*)event.data.mapping_start.tag);

				add_anchor(error_info, anchor_mapping,node);

				if(current != nullptr) {
					if(current->type == ConfigNode::NODE_MAPPING && !has_key) yaml_event_error(error_info, "Mapping can't be used as mapping key");
					current->add_child(error_info, key_name, node);
					has_key = false; /* Key is alway consumed */
					node_stack.push_back(current);
					current = node;
				} else {
					if(root == nullptr) root = node;
					else yaml_event_error(error_info, "Multiple root elements, only one document per context supported!");
					current = node;
				}

				break;
				break;
			case YAML_SEQUENCE_END_EVENT:
			case YAML_MAPPING_END_EVENT:
				if(!node_stack.empty()) {
					current = node_stack.back();
					node_stack.pop_back();
				} else {
					current = nullptr; /* We are back at root, anything other than document end will cause fatal error now */
				}
				break;
			case YAML_ALIAS_EVENT:
				{
					auto it = anchor_mapping.find(std::string((char*)event.data.alias.anchor));
					if(it == anchor_mapping.end()) yaml_event_error(error_info, "Unknown anchor %s", event.data.alias.anchor);
					node = it->second;

					if(current != nullptr) {

						if(current->type == ConfigNode::NODE_MAPPING && !has_key) yaml_event_error(error_info, "Mapping can't be used as mapping key (to append anchored data to map, use <<:*anchor)");

						if(current->type == ConfigNode::NODE_MAPPING && key_name == "<<") {
							if(node->type != ConfigNode::NODE_MAPPING) yaml_event_error(error_info, "Can't append non-mapping entry to mapping");
							/* Append data to map (here we need to do a copy of all elements, otherwise they will be double delete)*/
							for(auto &entry : node->mapping) {
								current->add_child(error_info, entry.first, new ConfigNode(*(entry.second)));
							}
						}
						current->add_child(error_info, key_name, new ConfigNode(*node));
						has_key = false; /* Key is alway consumed */

					} else {
						yaml_event_error(error_info, "You can't use a anchor as root element");
					}
				}
				break;
			/* We only allow one document, and stream events are not really relevant, thus ignore these events: */
			case YAML_STREAM_START_EVENT:
			case YAML_DOCUMENT_START_EVENT:
			case YAML_DOCUMENT_END_EVENT:
			case YAML_NO_EVENT:
				break;
			case YAML_STREAM_END_EVENT:
				yaml_event_error(error_info, "Reached stream end event (this should never be, should terminate at document_end_event)");
		}
	} while (event.type != YAML_DOCUMENT_END_EVENT);

	yaml_event_delete(&event);

	yaml_parser_delete(&parser);

	if(root == nullptr) Logging::fatal("[Config] Error in %s: Reached end of file with no root element found\n", context.c_str());

	return Config(root);
}

void ConfigNode::print(std::string indent) const {
	std::string indent_next = indent + "\t";
	switch(type) {
		case NODE_MAPPING:
			printf("%s{%s\n", indent.c_str(), node_tag.c_str());
			for(auto it : mapping) {
				printf("%s%s = ", indent_next.c_str(), it.first.c_str());
				it.second->print(it.second->type == NODE_SCALAR ? "" : indent_next);
			}
			printf("%s}\n", indent.c_str());
			break;
		case NODE_SEQUENCE:
			printf("%s[%s\n", indent.c_str(), node_tag.c_str());
			for(auto it : sequence) {
				it->print(indent_next);
			}
			printf("%s]\n", indent.c_str());
			break;
		case NODE_SCALAR:
			printf("%s%s%s;\n", indent.c_str(), node_tag.c_str(), scalar.c_str());
			break;
	}
}

const std::string &ConfigNode::tag() const {
	return node_tag;
}

bool ConfigNode::has_tag() const {
	return node_has_tag;
}

const std::string &ConfigNode::anchor() const {
	return node_anchor;
}

bool ConfigNode::has_anchor() const {
	return node_has_anchor;
}

const std::string &ConfigNode::parse_string() const {
	if(type != NODE_SCALAR) {
    print();
    Logging::fatal("[ConfigNode] Trying to read a non-scalar node as string\n");
  }
	return scalar;
}

int ConfigNode::parse_int() const {
	if(tag() == "!rnd" && type == NODE_SCALAR) {

		std::vector<std::string> parts = split(scalar, ",", false);

		if(parts.size() != 2) Logging::fatal("[ConfigNode] !rnd requires the format min , max: %s\n", scalar.c_str());

    int r1, r2;

    r1 = atoi(parts[0].c_str());
    r2 = atoi(parts[1].c_str());
		std::uniform_int_distribution<int> rnd(r1, r2);
    //printf("RAND: %d, %d, %d, %d\n", rnd(generator), rnd(generator), rnd(generator), rnd(generator));
    int r = rnd(generator);

		return r;
	} else {
		if(type != NODE_SCALAR) {
      print();
      Logging::fatal("[ConfigNode] Trying to read a non-scalar node as int\n");
    }
		return atoi(scalar.c_str());
	}
}

bool ConfigNode::parse_bool() const {
	if(type != NODE_SCALAR) {
    print();
    Logging::fatal("[ConfigNode] Trying to read a non-scalar node as bool\n");
  }
	if(scalar == "true") return true;
	else if(scalar == "false") return false;
	else Logging::fatal("Invalid boolean value %s\n", scalar.c_str());
	return false;
}

float ConfigNode::parse_float() const {
	if(type != NODE_SCALAR) {
    print();
    Logging::fatal("[ConfigNode] Trying to read a non-scalar node as float\n");
  }
	return static_cast<float>(atof(scalar.c_str()));
}

const std::vector<const ConfigNode*> &ConfigNode::list() const {
	if(type != NODE_SEQUENCE) {
    print();
		Logging::fatal("[ConfigNode] Trying to read a non-sequence node as list\n");
	}
	/* If first call, reflect sequence list to exposed sequence list */
	if(sequence.size() != exposed_sequence.size()) exposed_sequence = std::vector<const ConfigNode*>(sequence.cbegin(), sequence.cend());
	return exposed_sequence;
}

const std::map<std::string, const ConfigNode*> &ConfigNode::map() const {
	if(type != NODE_MAPPING) {
    print();
		Logging::fatal("[ConfigNode] Trying to read a non-mapping node as map\n");
	}
	/* If first call, reflect map to exposed map */
	if(mapping.size() != exposed_mapping.size()) exposed_mapping = std::map<std::string, const ConfigNode*>(mapping.cbegin(), mapping.cend());
	return exposed_mapping;
}

const ConfigNode &ConfigNode::operator[](const std::string &path) const {
	return *find(path, true);
}

ConfigNode ConfigNode::get(const std::string &path, const std::string &default_value) const {
	const ConfigNode * node = find(path, false);
	if(node) return *node;
	else {
		ConfigNode default_node(NODE_SCALAR);
		default_node.scalar = default_value;
		return default_node;
	}
}

const ConfigNode * ConfigNode::find(const std::string &path, bool fail_on_not_found) const {
	if(path == "/" || path == "") return this; //Always return this if seaching for root
	if(type != NODE_MAPPING) {
    print();
		Logging::fatal("[ConfigNode] Can't search non-map config node\n");
	}

	std::vector<std::string> data = split(path, "/", false);
	const ConfigNode * current = this;
	std::string prev = "/";
	for(std::string &s : data) {
		if(current->type != NODE_MAPPING) {
			Logging::verbose("[ConfigNode] node %s is of non-map type, can't search\n", prev.c_str());
			current = nullptr;
			break;
		}
		auto f = current->mapping.find(s);
		if(f != current->mapping.end()) {
			current = f->second;
		} else {
			current = nullptr;
			break;
		}
		prev = s;
	}
	if(current == nullptr && fail_on_not_found) {
    print();
		Logging::fatal("[ConfigNode] node not found: %s\n", path.c_str());
	}
	return current;
}

const ConfigNode * Config::find(const std::string &path, bool fail_on_not_found) const {
	return root_->find(path, fail_on_not_found);
}
const ConfigNode &Config::operator[](const std::string &path) const {
	return (*root_)[path];
}

ConfigNode Config::get(const std::string &path, const std::string &default_value) const {
	return root_->get(path, default_value);
}

Config::Config(ConfigNode * node) : root_(node) { }

Config::~Config() { }

void Config::print() const {
	root_->print();
}

const ConfigNode &Config::root() const {
	return *root_;
}

void ConfigNode::set_tag(const std::string &tag) {
	node_tag = tag;
	node_has_tag = true;
}

void Config::emit(ConfigNode * root_node, const std::string &filename) {
	char buffer[256];
	sprintf(buffer, "%s/%s", srcdir, filename.c_str());

	yaml_emitter_t emitter;
	yaml_event_t event;

	yaml_emitter_initialize(&emitter);

	FILE * out = fopen(buffer, "wb");

	yaml_emitter_set_output_file(&emitter, out);

	yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
	emit_event(&emitter, &event);


	yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1);
	emit_event(&emitter, &event);

	root_node->emit(&emitter);

	yaml_document_end_event_initialize(&event, 0);
	emit_event(&emitter, &event);

	yaml_stream_end_event_initialize(&event);

	fclose(out);

	yaml_emitter_delete(&emitter);
}

void ConfigNode::emit_scalar(yaml_emitter_t * emitter, yaml_event_t * event, const std::string &scalar, yaml_char_t * tag) {
	yaml_char_t * str = new yaml_char_t[scalar.length()];
	strncpy((char*)(str), scalar.c_str(), scalar.length());
	yaml_scalar_event_initialize(event, NULL,
			tag,
			str, static_cast<int>(scalar.length()),1,1,
			YAML_ANY_SCALAR_STYLE);
	Config::emit_event(emitter, event);
	delete[] str;
}

void ConfigNode::emit(yaml_emitter_t * emitter) const {
	yaml_event_t event;
	yaml_char_t * tag = NULL;
	if(node_has_tag) {
		tag = new yaml_char_t[node_tag.length() + 1];
		strcpy((char*)(tag), node_tag.c_str());
	}
	switch(type) {
		case NODE_SCALAR:
			emit_scalar(emitter, &event, scalar, tag);
			break;
		case NODE_SEQUENCE:
			yaml_sequence_start_event_initialize(&event, NULL,
					tag, 1,
					YAML_ANY_SEQUENCE_STYLE);
			Config::emit_event(emitter, &event);

			std::for_each(sequence.begin(), sequence.end(), std::bind(&ConfigNode::emit, std::placeholders::_1, emitter));

			yaml_sequence_end_event_initialize(&event);
			Config::emit_event(emitter, &event);
			break;
		case NODE_MAPPING:
			yaml_mapping_start_event_initialize(&event, NULL,
					tag, 1,
					YAML_ANY_MAPPING_STYLE);
			Config::emit_event(emitter, &event);

			for(auto entry : mapping) {
				emit_scalar(emitter, &event, entry.first);
				entry.second->emit(emitter);
			}

			yaml_mapping_end_event_initialize(&event);
			Config::emit_event(emitter, &event);
			break;


	}
	if(tag != NULL) {
		delete[] tag;
	}
}

void Config::emit_event(yaml_emitter_t * emitter, yaml_event_t * event) {
	if(!yaml_emitter_emit(emitter, event)) {
		Logging::fatal("Failed to emit: %s\n", emitter->problem);
	}
	//yaml_event_delete(event);
}
