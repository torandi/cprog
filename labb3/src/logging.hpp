#ifndef LOGGING_H
#define LOGGING_H

#include <cstdio>
#include <cstdarg>
#include <memory>
#include <functional>

#ifdef _WIN32
#define SIZET_FMT "Id"
#else
#define SIZET_FMT "zd"
#endif

namespace Logging {

	enum Severity {
		FATAL,
		ERROR,
		WARNING,
		INFO,
		VERBOSE,
		DEBUG,
	};

	class Destination {
	public:
		Destination(Severity severity);
		virtual ~Destination();

		virtual void write(Severity severity, const char* message) = 0;

	protected:
		/**
		 * Returns true if the severity isnt severe enough.
		 */
		bool filtered(Severity severity) const;

		Severity severity;
	};

	/**
	 * Initialize logging subsystem.
	 */
	void init();

	/**
	 * Closes all open logs.
	 */
	void cleanup();

	/**
	 * Add a logging destination.
	 *
	 * @param severity Anything with severity and higher will be included in this
	 *                 output.
	 * @param dst File pointer. Must be open for writing but will not closed during
	 *            cleanup.
	 */
	void add_destination(Severity severity, FILE* dst);

	/**
	 * Add a logging desination. Same add FILE* but automatically opens and closes
	 * file stream.
	 */
	void add_destination(Severity severity, const char* filename);

	/**
	 * Add logging destination to custom interface.
	 */
	void add_destination(std::unique_ptr<Destination> dst);

	/**
	 * Add logging destination using callback.
	 */
	void add_destination(Severity severity, std::function<void(const char* message)> callback);

	void __FORMAT__(printf, 2,3) message(Severity severity, const char* fmt, ...);
	void vmessage(Severity severity, const char* fmt, va_list ap);

	/**
	 * Short for `message(FATAL, fmt, ...);`
	 * @note It terminates the process using SIGTRAP or similar, e.g. execution
	 *       may continue only using a debugger.
	 */
	void __FORMAT__(printf, 1,2) __NORETURN__ fatal(const char* fmt, ...);

	/**
	 * Short for `message(Severity, fmt, ...);`.
	 */
	void __FORMAT__(printf, 1,2) error(const char* fmt, ...);
	void __FORMAT__(printf, 1,2) warning(const char* fmt, ...);
	void __FORMAT__(printf, 1,2) info(const char* fmt, ...);
	void __FORMAT__(printf, 1,2) verbose(const char* fmt, ...);
	void __FORMAT__(printf, 1,2) debug(const char* fmt, ...);


	/**
	 * Callback for fatal errors.
	 * After a call to this the engines state is undefined and should be assumed to not be recoverable.
	 */
	typedef void __NORETURN_PTR__ (*fatal_callback)();
	extern fatal_callback fatal_func;
}

#endif /* LOGGING_H */
