package com.devjacob.tula.utils;

import com.devjacob.tula.cli.CliOptions;

public abstract class LoggedClass {
    protected static Logger LOGGER;

    public LoggedClass(Class<?> clazz, LogLevel level, String logPattern) {
        LOGGER = Logger.get(clazz, level, logPattern);
    }
    
    public LoggedClass(Class<?> clazz, LogLevel level) {
        LOGGER = Logger.get(clazz, level);
    }
    
    public LoggedClass(Class<?> clazz, String logPattern) {
        LOGGER = Logger.get(clazz, logPattern);
    }

    public LoggedClass(Class<?> clazz) {
        LOGGER = Logger.get(clazz);
    }

    public static void initLogger(CliOptions options) {
        LOGGER.setLevel(options.getLogLevel());
    }

}
