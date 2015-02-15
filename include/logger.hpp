#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_LOGGER_H_
#define COM_LETV_BIGDATA_MYSQL2HDFS_LOGGER_H_

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/log/expressions/formatters/named_scope.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/exception.hpp>
#include <boost/exception/all.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <fstream>

namespace src = boost::log::sources;
namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
//namespace phoenix = boost::phoenix;

enum tracker_severity_level {
//  trace,
//  debug,
//  info,
//  warning,
//  error,
//  fatal
  mytrace,
  mydebug,
  myinfo,
  mywarning,
  myerror,
  myfatal
};

void LogInit();

//lvl: trace,debug,info,warning, error, fatal
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(mylogger, src::severity_logger_mt<tracker_severity_level>)

#define LOG(lvl) BOOST_LOG_SEV(mylogger::get(), lvl)
//#define LOG_INFO LOG(info)
//#define LOG_TRACE LOG(trace)
//#define LOG_DEBUG LOG(debug)
//#define LOG_WARN LOG(warning)
//#define LOG_ERR LOG(error)
////#define LOG_ERR LOG(tracker_severity_level(4))
//#define LOG_FATAL LOG(fatal)
#define LOG_INFO LOG(myinfo)
#define LOG_TRACE LOG(mytrace)
#define LOG_DEBUG LOG(mydebug)
#define LOG_WARN LOG(mywarning)
#define LOG_ERR LOG(myerror) << " [file:" << __FILE__ << ",line:" << __LINE__ << "] "
#define LOG_FATAL LOG(myfatal)

#endif
