/*
 * Copyright 2003,2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOG4CXX_LOGGER_H
#define _LOG4CXX_LOGGER_H

#include <log4cxx/portability.h>
#include <log4cxx/helpers/appenderattachableimpl.h>
#include <log4cxx/helpers/objectimpl.h>
#include <log4cxx/level.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/helpers/mutex.h>
#include <log4cxx/spi/location/locationinfo.h>
#include <log4cxx/helpers/resourcebundle.h>
#include <log4cxx/spi/loggerrepository.h>


namespace log4cxx
{

    namespace helpers {
            class synchronized;
    }

    class Logger;
    /** smart pointer to a Logger class */
    typedef helpers::ObjectPtrT<Logger> LoggerPtr;


    /**
    This is the central class in the log4cxx package. Most logging
    operations, except configuration, are done through this class.
    */
    class LOG4CXX_EXPORT Logger :
                public virtual spi::AppenderAttachable,
                public virtual helpers::ObjectImpl
    {
    public:
        DECLARE_ABSTRACT_LOG4CXX_OBJECT(Logger)
        BEGIN_LOG4CXX_CAST_MAP()
                LOG4CXX_CAST_ENTRY(Logger)
                LOG4CXX_CAST_ENTRY(spi::AppenderAttachable)
        END_LOG4CXX_CAST_MAP()

    protected:
        /**
        The name of this logger.
        */
        LogString name;

        /**
        The assigned level of this logger.  The
        <code>level</code> variable need not be assigned a value in
        which case it is inherited form the hierarchy.  */
        LevelPtr level;

        /**
        The parent of this logger. All loggers have at least one
        ancestor which is the root logger. */
        LoggerPtr parent;

        /** The resourceBundle for localized messages.

        @see setResourceBundle, getResourceBundle
        */
        helpers::ResourceBundlePtr resourceBundle;


        // Loggers need to know what Hierarchy they are in
        spi::LoggerRepository * repository;

        helpers::AppenderAttachableImplPtr aai;

                /** Additivity is set to true by default, that is children inherit
                        the appenders of their ancestors by default. If this variable is
                        set to <code>false</code> then the appenders found in the
                        ancestors of this logger are not used. However, the children
                        of this logger will inherit its appenders, unless the children
                        have their additivity flag set to <code>false</code> too. See
                        the user manual for more details. */
                bool additive;

    protected:
        friend class DefaultCategoryFactory;

        /**
        This constructor created a new <code>logger</code> instance and
        sets its name.

        <p>It is intended to be used by sub-classes only. You should not
        create categories directly.

        @param name The name of the logger.
        */
        Logger(const LogString& name);

    public:
        ~Logger();

        /**
        Add <code>newAppender</code> to the list of appenders of this
        Logger instance.

        <p>If <code>newAppender</code> is already in the list of
        appenders, then it won't be added again.
        */
        virtual void addAppender(const AppenderPtr& newAppender);


         /**
        Call the appenders in the hierrachy starting at
        <code>this</code>.  If no appenders could be found, emit a
        warning.

        <p>This method calls all the appenders inherited from the
        hierarchy circumventing any evaluation of whether to log or not
        to log the particular log request.

        @param event the event to log.  */
        void callAppenders(const spi::LoggingEventPtr& event, log4cxx::helpers::Pool& p);

        /**
        Close all attached appenders implementing the AppenderAttachable
        interface.
        */
        void closeNestedAppenders();

        /**
        Log a message string with the {@link Level#DEBUG DEBUG} level.

        <p>This method first checks if this logger is <code>DEBUG</code>
        enabled by comparing the level of this logger with the {@link
        Level#DEBUG DEBUG} level. If this logger is
        <code>DEBUG</code> enabled, it proceeds to call all the
        registered appenders in this logger and also higher in the
        hierarchy depending on the value of the additivity flag.

        @param message the message string to log.
        @param file the file where the log statement was written.
        @param line the line where the log statement was written.
        */
        void debug(const std::string& msg, const log4cxx::spi::LocationInfo& location);
        void debug(const std::wstring& msg, const log4cxx::spi::LocationInfo& location);
        void debug(const std::string& msg);
        void debug(const std::wstring& msg);


        /**
        Log a message string with the {@link Level#ERROR ERROR} level.

        <p>This method first checks if this logger is <code>ERROR</code>
        enabled by comparing the level of this logger with the {@link
        Level#ERROR ERROR} level. If this logger is
        <code>ERROR</code> enabled, it proceeds to call all the
        registered appenders in this logger and also higher in the
        hierarchy depending on the value of the additivity flag.

        @param message the message string to log.
        @param file the file where the log statement was written.
        @param line the line where the log statement was written.
        */
        void error(const std::wstring& msg, const log4cxx::spi::LocationInfo& location);
        void error(const std::string& msg, const log4cxx::spi::LocationInfo& location);
        void error(const std::wstring& msg);
        void error(const std::string& msg);


        /**
        Log a message string with the {@link Level#FATAL FATAL} level.

        <p>This method first checks if this logger is <code>FATAL</code>
        enabled by comparing the level of this logger with the {@link
        Level#FATAL FATAL} level. If this logger is
        <code>FATAL</code> enabled, it proceeds to call all the
        registered appenders in this logger and also higher in the
        hierarchy depending on the value of the additivity flag.

        @param message the message string to log.
        @param file the file where the log statement was written.
        @param line the line where the log statement was written.
        */
        void fatal(const std::wstring& msg, const log4cxx::spi::LocationInfo& location);
        void fatal(const std::string& msg, const log4cxx::spi::LocationInfo& location);
        void fatal(const std::wstring& msg);
        void fatal(const std::string& msg);

        /**
        This method creates a new logging event and logs the event
        without further checks.
        @param level the level to log.
        @param message the message string to log.
        @param locaion location of the logging statement.
        */
        void forcedLog(const LevelPtr& level, const std::string& message,
                        const log4cxx::spi::LocationInfo& location);
        void forcedLog(const LevelPtr& level, const std::wstring& message,
                        const log4cxx::spi::LocationInfo& location);


        void forcedLog(const LevelPtr& level, const std::string& message);
        void forcedLog(const LevelPtr& level, const std::wstring& message);


        /**
        Get the additivity flag for this Logger instance.
        */
    public:
        bool getAdditivity() const;

        /**
        Get the appenders contained in this logger as an AppenderList.
        If no appenders can be found, then an empty AppenderList
        is returned.
        @return AppenderList An collection of the appenders in this logger.*/
        AppenderList getAllAppenders() const;

        /**
        Look for the appender named as <code>name</code>.
        <p>Return the appender with that name if in the list. Return
        <code>NULL</code> otherwise.  */
        AppenderPtr getAppender(const LogString& name) const;

        /**
        Starting from this logger, search the logger hierarchy for a
        non-null level and return it.

        <p>The Logger class is designed so that this method executes as
        quickly as possible.

        @throws RuntimeException if all levels are null in the hierarchy
        */
        virtual const LevelPtr& getEffectiveLevel() const;

        /**
        Return the the LoggerRepository where this
        <code>Logger</code> is attached.
        */
        spi::LoggerRepositoryPtr getLoggerRepository() const;


        /**
        Return the logger name.  */
        inline const LogString& getName() const
                        { return name; }
        void getName(std::string& name) const;
        void getName(std::wstring& name) const;


        /**
        Returns the parent of this logger. Note that the parent of a
        given logger may change during the lifetime of the logger.

        <p>The root logger will return <code>0</code>.
        */
        const LoggerPtr& getParent() const;


        /**
        Returns the assigned Level, if any, for this Logger.

        @return Level - the assigned Level, can be null.
        */
        const LevelPtr& getLevel() const;

        /**
        Retrieve a logger by name.
        */
        static LoggerPtr getLogger(const std::string& name);
        static LoggerPtr getLogger(const std::wstring& name);
        static LoggerPtr getLogger(const char* const name);
        static LoggerPtr getLogger(const wchar_t* const name);

        /**
        Retrieve the root logger.
        */
        static LoggerPtr getRootLogger();

        /**
        Like #getLogger except that the type of logger
        instantiated depends on the type returned by the
        LoggerFactory#makeNewLoggerInstance method of the
        <code>factory</code> parameter.

        <p>This method is intended to be used by sub-classes.

        @param name The name of the logger to retrieve.

        @param factory A LoggerFactory implementation that will
        actually create a new Instance.
                */
        static LoggerPtr getLogger(const LogString& name,
                        const spi::LoggerFactoryPtr& factory);


        /**
        Return the <em>inherited</em> ResourceBundle for this logger.


        This method walks the hierarchy to find the appropriate resource bundle.
        It will return the resource bundle attached to the closest ancestor of
        this logger, much like the way priorities are searched. In case there
        is no bundle in the hierarchy then <code>NULL</code> is returned.
        */
        helpers::ResourceBundlePtr getResourceBundle() const;

        protected:
        /**
        Returns the string resource coresponding to <code>key</code> in this
        category's inherited resource bundle.

        If the resource cannot be found, then an {@link #error error} message
        will be logged complaining about the missing resource.

        @see #getResourceBundle.
        */
        LogString getResourceBundleString(const LogString& key) const;

        public:
        /**
        Log a message string with the {@link Level#INFO INFO} level.

        <p>This method first checks if this logger is <code>INFO</code>
        enabled by comparing the level of this logger with the {@link
        Level#INFO INFO} level. If this logger is
        <code>INFO</code> enabled, it proceeds to call all the
        registered appenders in this logger and also higher in the
        hierarchy depending on the value of the additivity flag.

        @param message the message string to log.
                @param file the file where the log statement was written.
                @param line the line where the log statement was written.
                */
       void info(const std::wstring& msg, const log4cxx::spi::LocationInfo& location);
       void info(const std::string& msg, const log4cxx::spi::LocationInfo& location);
       void info(const std::wstring& msg);
       void info(const std::string& msg);

        /**
        Is the appender passed as parameter attached to this category?
        */
        bool isAttached(const AppenderPtr& appender) const;

       /**
        *  Check whether this logger is enabled for the <code>DEBUG</code>
        *  Level.
        *
        *  <p> This function is intended to lessen the computational cost of
        *  disabled log debug statements.
        *
        *  <p> For some <code>logger</code> Logger object, when you write,
        *  <pre>
        *      logger->debug("debug message");
        *  </pre>
        *
        *  <p>You incur the cost constructing the message, concatenation in
        *  this case, regardless of whether the message is logged or not.
        *
        *  <p>If you are worried about speed, then you should write
        *  <pre>
        * 	 if(logger->isDebugEnabled()) {
        * 	   logger->debug("debug message");
        * 	 }
        *  </pre>
        *
        *  <p>This way you will not incur the cost of parameter
        *  construction if debugging is disabled for <code>logger</code>. On
        *  the other hand, if the <code>logger</code> is debug enabled, you
        *  will incur the cost of evaluating whether the logger is debug
        *  enabled twice. Once in <code>isDebugEnabled</code> and once in
        *  the <code>debug</code>.  This is an insignificant overhead
        *  since evaluating a logger takes about 1%% of the time it
        *  takes to actually log.
        *
        *  @return bool - <code>true</code> if this logger is debug
        *  enabled, <code>false</code> otherwise.
        *   */
        bool isDebugEnabled() const;

        /**
        Check whether this logger is enabled for a given
        Level passed as parameter.

        See also #isDebugEnabled.

        @return bool True if this logger is enabled for <code>level</code>.
        */
        bool isEnabledFor(const LevelPtr& level) const;


        /**
        Check whether this logger is enabled for the info Level.
        See also #isDebugEnabled.

        @return bool - <code>true</code> if this logger is enabled
        for level info, <code>false</code> otherwise.
        */
        bool isInfoEnabled() const;

         /**
        Check whether this logger is enabled for the warn Level.
        See also #isDebugEnabled.

        @return bool - <code>true</code> if this logger is enabled
        for level warn, <code>false</code> otherwise.
        */
        bool isWarnEnabled() const;

         /**
        Check whether this logger is enabled for the error Level.
        See also #isDebugEnabled.

        @return bool - <code>true</code> if this logger is enabled
        for level error, <code>false</code> otherwise.
        */
        bool isErrorEnabled() const;

         /**
        Check whether this logger is enabled for the fatal Level.
        See also #isDebugEnabled.

        @return bool - <code>true</code> if this logger is enabled
        for level fatal, <code>false</code> otherwise.
        */
        bool isFatalEnabled() const;

        /**
        Log a localized and parameterized message.

        First, the user supplied
        <code>key</code> is searched in the resource bundle. Next, the resulting
        pattern is formatted using helpers::StringHelper::format method with the user
        supplied object array <code>params</code>.

        @param level The level of the logging request.
        @param key The key to be searched in the #resourceBundle.
        @param file The source file of the logging request, may be null.
        @param line The number line of the logging request.

        @see #setResourceBundle
        */
        void l7dlog(const LevelPtr& level, const std::wstring& key,
                                const log4cxx::spi::LocationInfo& locationInfo,
                                ...);
        void l7dlog(const LevelPtr& level, const std::string& key,
                                const log4cxx::spi::LocationInfo& locationInfo,
                                ...);

          /**
        This is the most generic printing method. It is intended to be
        invoked by <b>wrapper</b> classes.

        @param level The level of the logging request.
        @param message The message of the logging request.
        @param file The source file of the logging request, may be null.
        @param line The number line of the logging request.  */
        void log(const LevelPtr& level, const std::wstring& message,
            const log4cxx::spi::LocationInfo& location);
        void log(const LevelPtr& level, const std::string& message,
            const log4cxx::spi::LocationInfo& location);
        void log(const LevelPtr& level, const std::wstring& message);
        void log(const LevelPtr& level, const std::string& message);



        /**
        Remove all previously added appenders from this logger
        instance.
        <p>This is useful when re-reading configuration information.
        */
        void removeAllAppenders();

        /**
        Remove the appender passed as parameter form the list of appenders.
        */
        void removeAppender(const AppenderPtr& appender);

        /**
        Remove the appender with the name passed as parameter form the
        list of appenders.
         */
        void removeAppender(const LogString& name);

       /**
        Set the additivity flag for this Logger instance.
         */
        void setAdditivity(bool additive);

    protected:
        friend class Hierarchy;
        /**
        Only the Hierarchy class can set the hierarchy of a logger.*/
        void setHierarchy(spi::LoggerRepository * repository);

        public:
        /**
        Set the level of this Logger. If you are passing any of
        <code>Level#DEBUG</code>, <code>Level#INFO</code>,
        <code>Level#WARN</code>, <code>Level#ERROR</code>,
        <code>Level#FATAL</code> as a parameter, you need to case them as
        Level.

        <p>As in <pre> &nbsp;&nbsp;&nbsp;logger->setLevel(Level::getDebug()); </pre>


        <p>Null values are admitted.  */
        virtual void setLevel(const LevelPtr& level);

        /**
        Set the resource bundle to be used with localized logging method #l7dlog.
        */
        inline void setResourceBundle(const helpers::ResourceBundlePtr& bundle)
                { resourceBundle = bundle; }

        /**
        Log a message string with the {@link Level#WARN WARN} level.

        <p>This method first checks if this logger is <code>WARN</code>
        enabled by comparing the level of this logger with the {@link
        Level#WARN WARN} level. If this logger is
        <code>WARN</code> enabled, it proceeds to call all the
        registered appenders in this logger and also higher in the
        hierarchy depending on the value of the additivity flag.

        @param message the message string to log.
        @param file the file where the log statement was written.
        @param line the line where the log statement was written.
        */
        void warn(const std::wstring& msg, const log4cxx::spi::LocationInfo& location);
        void warn(const std::string& msg, const log4cxx::spi::LocationInfo& location);
        void warn(const std::wstring& msg);
        void warn(const std::string& msg);



        inline const log4cxx::helpers::Mutex& getMutex() const { return mutex; }

        private:
                //
        //  prevent copy and assignment
        Logger(const Logger&);
        Logger& operator=(const Logger&);
        log4cxx::helpers::Mutex mutex;
        friend class log4cxx::helpers::synchronized;
   };
}

/** @addtogroup LoggingMacros Logging macros
@{
*/

#if !defined(LOG4CXX_UNLIKELY)
#if __GNUC__ >= 3
/**
Provides optimization hint to the compiler
to optimize for the expression being false.
@param expr boolean expression.
@returns value of expression.
*/
#define LOG4CXX_UNLIKELY(expr) __builtin_expect(expr, 0)
#else
/**
Provides optimization hint to the compiler
to optimize for the expression being false.
@param expr boolean expression.
@returns value of expression.
**/
#define LOG4CXX_UNLIKELY(expr) expr
#endif
#endif


/**
Logs a message to a specified logger with a specified level.

@param logger the logger to be used.
@param level the level to log.
@param message the message string to log.
*/
#define LOG4CXX_LOG(logger, level, message) { \
        if (logger->isEnabledFor(level)) {\
           logger->forcedLog(level, message, LOG4CXX_LOCATION); } }

/**
Logs a message to a specified logger with the DEBUG level.

@param logger the logger to be used.
@param message the message string to log.
*/
#define LOG4CXX_DEBUG(logger, message) { \
        if (LOG4CXX_UNLIKELY(logger->isDebugEnabled())) {\
           logger->forcedLog(::log4cxx::Level::getDebug(), message, LOG4CXX_LOCATION); }}

/**
Logs a message to a specified logger with the INFO level.

@param logger the logger to be used.
@param message the message string to log.
*/
#define LOG4CXX_INFO(logger, message) { \
        if (logger->isInfoEnabled()) {\
            logger->forcedLog(::log4cxx::Level::INFO, message, LOG4CXX_LOCATION); }}

/**
Logs a message to a specified logger with the WARN level.

@param logger the logger to be used.
@param message the message string to log.
*/
#define LOG4CXX_WARN(logger, message) { \
        if (logger->isWarnEnabled()) {\
            logger->forcedLog(::log4cxx::Level::WARN, message, LOG4CXX_LOCATION); }}

/**
Logs a message to a specified logger with the ERROR level.

@param logger the logger to be used.
@param message the message string to log.
*/
#define LOG4CXX_ERROR(logger, message) { \
        if (logger->isErrorEnabled()) {\
             logger->forcedLog(::log4cxx::Level::getError(), message, LOG4CXX_LOCATION); }}

/**
Logs a error if the condition is not true.

@param logger the logger to be used.
@param condition condition
@param message the message string to log.
*/
#define LOG4CXX_ASSERT(logger, condition, message) { \
        if (!condition && logger->isErrorEnabled()) {\
             logger->forcedLog(::log4cxx::Level::getError(), message, LOG4CXX_LOCATION); }}


/**
Logs a message to a specified logger with the FATAL level.

@param logger the logger to be used.
@param message the message string to log.
*/
#define LOG4CXX_FATAL(logger, message) { \
        if (logger->isFatalEnabled()) {\
            logger->forcedLog(::log4cxx::Level::FATAL, message, LOG4CXX_LOCATION); }}

/**
Logs a localized message with no parameter.

@param logger the logger to be used.
@param level the level to log.
@param key the key to be searched in the resourceBundle of the logger.
*/
#define LOG4CXX_L7DLOG(logger, level, key) { \
        if (logger->isEnabledFor(level)) {\
        logger->l7dlog(level, key, LOG4CXX_LOCATION); }}

/**
Logs a localized message with one parameter.

@param logger the logger to be used.
@param level the level to log.
@param key the key to be searched in the resourceBundle of the logger.
@param p1 the unique parameter. Must be of type (TCHAR *).
*/
#define LOG4CXX_L7DLOG1(logger, level, key, p1) { \
        if (logger->isEnabledFor(level)) {\
        logger->l7dlog(level, key, LOG4CXX_LOCATION, p1); }}

/**
Logs a localized message with two parameters.

@param logger the logger to be used.
@param level the level to log.
@param key the key to be searched in the resourceBundle of the logger.
@param p1 the first parameter. Must be of type (TCHAR *).
@param p2 the second parameter. Must be of type (TCHAR *).
*/
#define LOG4CXX_L7DLOG2(logger, level, key, p1, p2) { \
        if (logger->isEnabledFor(level)) {\
        logger->l7dlog(level, key, LOG4CXX_LOCATION, p1, p2); }}

/**
Logs a localized message with three parameters.

@param logger the logger to be used.
@param level the level to log.
@param key the key to be searched in the resourceBundle of the logger.
@param p1 the first parameter. Must be of type (TCHAR *).
@param p2 the second parameter. Must be of type (TCHAR *).
@param p3 the third parameter. Must be of type (TCHAR *).
*/
#define LOG4CXX_L7DLOG3(logger, level, key, p1, p2, p3) { \
        if (logger->isEnabledFor(level)) {\
        logger->l7dlog(level, key, LOG4CXX_LOCATION, p1, p2, p3); }}

/**@}*/

#endif //_LOG4CXX_LOGGER_H
