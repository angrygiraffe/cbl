#pragma once

#include <exception>
#include <stdexcept>
#include <string>

#include "fmt.h"

//change macro value to raw stirng in complie phase
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x

//macro used for exception/log trace
#define LINE_STRING STRINGIZE2(__LINE__)
#define LOC_STRING "[" __FILE__ ":" STRINGIZE(__LINE__) "]"

//macro for throw loc_except.
//or can just 
//	throw cbl::loc_except(LOC_STRING, msg);
#define THROW_TRACE(msg)	\
	do{	\
		throw(cbl::trace_error(fmt::format("{}{}", LOC_STRING, msg)));	\
	}while(0)

#define RETHROW_TRACE(msg)	\
	do{ \
		cbl::trace_rethrow(LOC_STRING, msg);	\
	}while(0)


namespace cbl {

#define TRACE_ERROR_DEFAULT_INDENT 2

class trace_error : public std::logic_error{
public:
	explicit trace_error(const std::string& msg, size_t indent = 0)
		:logic_error(msg),
		indent_(indent){}

	explicit trace_error(const char* msg, size_t indent = 0)
		:logic_error(msg),
		indent_(indent){}

	//no neead to override method what
	//const char* what() const noexcept override{}

	size_t indent()const{
		return indent_;
	}

private:
	size_t indent_;
};

template<class T>
[[noreturn]] void trace_rethrow(const char* loc, T&& msg){
	auto eptr = std::current_exception();
	if(eptr == nullptr){
		throw trace_error(fmt::format("{}{}", loc, msg));
	}
	try{
		std::rethrow_exception(eptr);
	}catch(const trace_error& te){
		throw trace_error(fmt::format("{}\n{}{}{}", te.what(), std::string(TRACE_ERROR_DEFAULT_INDENT * (te.indent() + 1), ' '), loc, msg), te.indent() + 1);
	}catch(const std::exception& e){
		throw trace_error(fmt::format("{}\n{}{}{}", e.what(), std::string(TRACE_ERROR_DEFAULT_INDENT, ' '), loc, msg), 1);
	}catch(...){
		throw trace_error(fmt::format("lost exception\n{}{}{}", std::string(TRACE_ERROR_DEFAULT_INDENT, 'n'), loc, msg), 1);
	}
}

}
