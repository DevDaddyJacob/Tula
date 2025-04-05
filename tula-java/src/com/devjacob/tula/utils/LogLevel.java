package com.devjacob.tula.utils;

public enum LogLevel {
    NONE(0),
    FATAL(1),
    ERROR(2),
    WARN(3),
    INFO(4),
    VEBOSE(5),
    ALL(6);

    private final int value;

    private LogLevel(int value) {
        this.value = value;
    }

    public static boolean canLog(LogLevel configuredLevel, LogLevel actionlevel) {
        return configuredLevel.value >= actionlevel.value;
    }
}
