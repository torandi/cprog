#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "logging.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <csignal>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

namespace Logging {

	static bool initialized = false;
	fatal_callback fatal_func = nullptr;

	Destination::Destination(Severity severity)
	: severity(severity) {

	}

	Destination::~Destination(){

	}

	bool Destination::filtered(Severity severity) const {
		return severity > this->severity;
	}

	class FileDestination: public Destination {
	public:
		FileDestination(Severity severity, FILE* dst, bool autoclose)
			: Destination(severity)
			, dst(dst)
			, autoclose(autoclose) {

		}

		~FileDestination(){
			if ( autoclose ){
				fclose(dst);
			}
		}

		virtual void write(Severity severity, const char* message){
			if ( filtered(severity) ) return;
			fputs(message, dst);
			fflush(dst);
		}

		FILE* dst;
		bool autoclose;
	};

	class CallbackDestination: public Destination {
	public:
		CallbackDestination(Severity severity, std::function<void(const char* message)> callback)
		: Destination(severity)
		, callback(callback) {

		}

		virtual void write(Severity severity, const char* message){
			if ( filtered(severity) ) return;
			callback(message);
		}

		std::function<void(const char* message)> callback;
	};

	static std::vector<Destination*> output;

	void init(){
		fatal_func = [](){
#ifdef _MSC_VER
			__debugbreak();
#else
			raise(SIGTRAP);
#endif
			abort(); /* force it to abort, function must be fatal */
		};

		initialized = true;
	}

	void cleanup(){
		for ( auto dst : output ){
			delete dst;
		}
		output.clear();
		initialized = false;
	}

	static void add_destination(Severity severity, FILE* dst, bool autoclose){
		Destination* o = new FileDestination(severity, dst, false);
		output.push_back(o);

		if ( dst == stdout || dst == stderr ){
			return;
		}

#ifdef HAVE_GETTIMEOFDAY
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		struct tm* local = localtime(&tv.tv_sec);
#else
		__time64_t tv;
		struct tm tm, *local = &tm;
        _time64(&tv);
        _localtime64_s(local, &tv);
#endif

		char buf[64] = {0,};
		strftime(buf, sizeof(buf), "\nLogging started at %Y-%m-%d %H.%M.%S\n", local);
		o->write(INFO, buf);
	}

	void add_destination(Severity severity, FILE* dst){
		add_destination(severity, dst, false);
	}

	void add_destination(Severity severity, const char* filename){
		FILE* fp = fopen(filename, "a");
		if ( !fp ){
			error("failed to open logging destination `%s'.", filename);
			return;
		}
		add_destination(severity, fp, true);
	}

	void add_destination(std::unique_ptr<Destination> dst){
		output.push_back(dst.release());
	}

	void add_destination(Severity severity, std::function<void(const char* message)> callback){
		output.push_back(new CallbackDestination(severity, callback));
	}

	void message(Severity severity, const char* fmt, ...){
		va_list ap;
		va_start(ap, fmt);
		vmessage(severity, fmt, ap);
		va_end(ap);
	}

	void vmessage(Severity severity, const char* fmt, va_list ap){
		if ( !initialized ){
			fprintf(stderr, "logging not initialized, no messages may be written.\n");
			abort();
		}

		char* message = nullptr;
		if ( vasprintf(&message, fmt, ap) == -1 ){
			fprintf(stderr, "memory allocation failed\n");
			abort();
		}
		for ( auto dst : output ){
			dst->write(severity, message);
		}
		free(message);
	}

	void fatal(const char* fmt, ...){

		/*
		 * Make sure to get some output:
		 */
		if(output.empty()) {
			add_destination(FATAL, stderr, false);
		}

		va_list ap;
		va_start(ap, fmt);
		vmessage(FATAL, fmt, ap);
		va_end(ap);

		fatal_func();
	}

	void error(const char* fmt, ...){
		va_list ap;
		va_start(ap, fmt);
		vmessage(ERROR, fmt, ap);
		va_end(ap);
	}

	void warning(const char* fmt, ...){
		va_list ap;
		va_start(ap, fmt);
		vmessage(WARNING, fmt, ap);
		va_end(ap);
	}

	void info(const char* fmt, ...){
		va_list ap;
		va_start(ap, fmt);
		vmessage(INFO, fmt, ap);
		va_end(ap);
	}

	void verbose(const char* fmt, ...){
		va_list ap;
		va_start(ap, fmt);
		vmessage(VERBOSE, fmt, ap);
		va_end(ap);
	}

	void debug(const char* fmt, ...){
		va_list ap;
		va_start(ap, fmt);
		vmessage(DEBUG, fmt, ap);
		va_end(ap);
	}
}
