#include "logger.hpp"
void LogInit() {

  typedef sinks::asynchronous_sink<sinks::text_file_backend> TextSink;
  
  logging::formatter formatter =
    expr::stream
<< expr::format_named_scope("Scopes", boost::log::keywords::format = "%n (%f : %l)") 
      << "["
    << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
    << "] ["
    << expr::attr<attrs::current_thread_id::value_type >("ThreadID")
    << "] ["
    << expr::attr<tracker_severity_level>("Severity")
    << "]    "
    << expr::smessage;

  // init file_sink
  boost::shared_ptr<sinks::text_file_backend> backend1 = boost::make_shared<sinks::text_file_backend>(
                              keywords::file_name = "mysql2hive_%Y-%m-%d_%H-%M-%S.%N.log",
                              keywords::rotation_size = 50 * 1024 * 1024,
                              keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                              keywords::min_free_space = 30 * 1024 * 1024);
  backend1->auto_flush(true);
  boost::shared_ptr<TextSink> file_sink(new TextSink(backend1));
  file_sink->set_formatter(formatter);
  //file_sink->set_filter(expr::attr<tracker_severity_level>("Severity") >= trace);
  file_sink->set_filter(expr::attr<tracker_severity_level>("Severity") >= mytrace);
  logging::core::get()->add_sink(file_sink);
    
  ///init console log
  auto console_sink = logging::add_console_log();
  console_sink->set_formatter(formatter);
  //console_sink->set_filter(expr::attr<tracker_severity_level>("Severity") >= trace);
  console_sink->set_filter(expr::attr<tracker_severity_level>("Severity") >= mytrace);
  logging::core::get()->add_sink(console_sink);
    logging::core::get()->add_global_attribute("Scope", attrs::named_scope()); 
  logging::add_common_attributes();
}

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
    std::basic_ostream< CharT, TraitsT >& strm, tracker_severity_level lvl)
{
    static const char* const str[] = {"trace", "debug", "info", "warning", "error", "fatal"};
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
        strm << str[lvl];
    else
        strm << static_cast< int >(lvl);
    return strm;
};
