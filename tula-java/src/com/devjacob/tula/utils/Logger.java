package com.devjacob.tula.utils;

public class Logger {
    private final String classFQN;
    private final String logPattern;
    private LogLevel level;

    private Logger(LogLevel level, String classFQN, String logPattern) {
        this.level = level;
        this.classFQN = classFQN;
        this.logPattern = logPattern;
    }

    public static Logger get(Class<?> clazz, LogLevel level, String logPattern) {
        return new Logger(level, clazz.getCanonicalName(), logPattern);
    }
    
    public static Logger get(Class<?> clazz, LogLevel level) {
        return new Logger(level, clazz.getCanonicalName(), "[%class] %level: %message");
    }
    
    public static Logger get(Class<?> clazz, String logPattern) {
        return new Logger(LogLevel.INFO, clazz.getCanonicalName(), logPattern);
    }

    public static Logger get(Class<?> clazz) {
        return new Logger(LogLevel.INFO, clazz.getCanonicalName(), "[%class] %level: %message");
    }

    public void setLevel(LogLevel level) {
        this.level = level;
    }

    public LogLevel getLevel() {
        return level;
    }

    public void verbose(final String message) {
        logIfAble(LogLevel.VEBOSE, message);
    }

    public void verbose(final Object message) {
        logIfAble(LogLevel.VEBOSE, message.toString());
    }

    public void verbose(final String format, Object... params) {
        logIfAble(LogLevel.VEBOSE, String.format(format, params));
    }

    public void verbose(final Throwable throwable) {
        logIfAble(LogLevel.VEBOSE, throwable);
    }

    public void info(final String message) {
        logIfAble(LogLevel.INFO, message);
    }

    public void info(final Object message) {
        logIfAble(LogLevel.INFO, message.toString());
    }

    public void info(final String format, Object... params) {
        logIfAble(LogLevel.INFO, String.format(format, params));
    }

    public void info(final Throwable throwable) {
        logIfAble(LogLevel.INFO, throwable);
    }

    public void warn(final String message) {
        logIfAble(LogLevel.WARN, message);
    }

    public void warn(final Object message) {
        logIfAble(LogLevel.WARN, message.toString());
    }

    public void warn(final String format, Object... params) {
        logIfAble(LogLevel.WARN, String.format(format, params));
    }

    public void warn(final Throwable throwable) {
        logIfAble(LogLevel.WARN, throwable);
    }

    public void error(final String message) {
        logIfAble(LogLevel.ERROR, message);
    }

    public void error(final Object message) {
        logIfAble(LogLevel.ERROR, message.toString());
    }

    public void error(final String format, Object... params) {
        logIfAble(LogLevel.ERROR, String.format(format, params));
    }

    public void error(final Throwable throwable) {
        logIfAble(LogLevel.ERROR, throwable);
    }

    public void fatal(final String message) {
        logIfAble(LogLevel.FATAL, message);
    }

    public void fatal(final Object message) {
        logIfAble(LogLevel.FATAL, message.toString());
    }

    public void fatal(final String format, Object... params) {
        logIfAble(LogLevel.FATAL, String.format(format, params));
    }

    public void fatal(final Throwable throwable) {
        logIfAble(LogLevel.FATAL, throwable);
    }

    private String fillPattern(final LogLevel level, final String classFQN, final String message) {
        String template = logPattern;
        return template
            .replaceAll("%level", level.toString())
            .replaceAll("%class", classFQN)
            .replaceAll("%message", message);
    }

    private void logIfAble(final LogLevel level, final String message) {
        if (!LogLevel.canLog(this.level, level)) { return; }
        System.out.printf("%s%n", fillPattern(level, classFQN, message));
    }

    private void logIfAble(final LogLevel level, final Throwable throwable) {
        if (!LogLevel.canLog(this.level, level)) { return; }

        StringBuilder throwableBld = new StringBuilder()
            .append("Error: ")
            .append(throwable.getMessage())
            .append("\n\tCaused by:\n");
            
        for (StackTraceElement elem : throwable.getStackTrace()) {
            throwableBld.append("\t").append(elem.toString()).append("\n");
        }

        System.out.printf("%s%n", fillPattern(level, classFQN, throwableBld.toString()));
    }
}
