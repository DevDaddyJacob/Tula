package com.devjacob.tula.cli;

import com.devjacob.tula.Tula;
import com.devjacob.tula.errors.TulaReporter;
import com.devjacob.tula.utils.LogLevel;
import com.devjacob.tula.utils.LoggedClass;

public class TulaCLI extends LoggedClass {
    private static final String FLAG_REGEX = "^-([A-Z]|[a-z])$";
    private static final String OPTION_REGEX = "^--([A-Z]|[a-z])+(=.+)?$";
    private static final String NO_VAL_OPTION_REGEX = "^--([A-Z]|[a-z])+$";
    private final String[] rawArgs;
    private CliOptions options;

    public TulaCLI(String[] args) {
        super(TulaCLI.class);
        rawArgs = args;
        parseArgs();
        initLoggers();
    }

    public CliOptions getOptions() {
        return options;
    }

    private void initLoggers() {
        Tula.initLogger(options);
        TulaReporter.initLogger(options);
    }

    private void parseArgs() {
        CliOptions.Builder optionsBld = new CliOptions.Builder();
        if (rawArgs == null) { return; }

        for (int i = 0; i < rawArgs.length; i++) {
            String arg = rawArgs[i];
            if (arg == null) { continue; }
            
            if (isFlag(arg)) {
                parseFlag(optionsBld, arg);
                continue;
            }
            
            if (isOption(arg)) {
                parseOptions(optionsBld, arg);
                continue;
            }
        }

        options = optionsBld.build();
        LOGGER.verbose("Finished parsing x%d arguments", rawArgs.length);
    }

    private void parseFlag(CliOptions.Builder builder, String arg) {
        switch (arg) {
            case "-i": 
                builder.setInteractive(false);
                break;
        }
    }

    private void parseOptions(CliOptions.Builder builder, String arg) {
        String key = getOptionKey(arg);
        String value = getOptionValue(arg);

        switch (key) {
            case "--file":
                builder.setFilePath(value.replaceAll("\"", ""));
                break;

            case "--interactive":
                if (value.toLowerCase().equals("on")) {
                    builder.setInteractive(true);
                } else if (value.toLowerCase().equals("off")) {
                    builder.setInteractive(false);
                }
                break;

            case "--logging": 
                LogLevel level;
                switch ((value == null ? "4" : value).toLowerCase()) {
                    case "0":
                    case "n":
                    case "none":
                        level = LogLevel.NONE;
                        break;
                        
                    case "1":
                    case "f":
                    case "fatal":
                        level = LogLevel.FATAL;
                        break;
                        
                    case "2":
                    case "e":
                    case "error":
                        level = LogLevel.ERROR;
                        break;
                        
                    case "3":
                    case "w":
                    case "warn":
                        level = LogLevel.WARN;
                        break;
                        
                    case "4":
                    case "i":
                    case "info":
                        level = LogLevel.INFO;
                        break;
                        
                    case "5":
                    case "v":
                    case "verbose":
                        level = LogLevel.VEBOSE;
                        break;
                        
                    case "6":
                    case "a":
                    case "all":
                        level = LogLevel.ALL;
                        break;

                    default:
                        level = LogLevel.INFO;
                        break;
                }

                builder.setLogLevel(level);
                LOGGER.setLevel(level);
                break;

            default: break;
        }
    }

    private boolean isFlag(String arg) {
        return arg.matches(FLAG_REGEX);
    }

    private boolean isOption(String arg) {
        return arg.matches(OPTION_REGEX);
    }

    private boolean isNoValOption(String arg) {
        return arg.matches(NO_VAL_OPTION_REGEX);
    }

    private String getOptionKey(String arg) {
        if (!isOption(arg)) { return null; }
        return arg.split("=")[0];
    }

    private String getOptionValue(String arg) {
        if (!isOption(arg) || isNoValOption(arg)) { return null; }
        return arg.split("=")[1];
    }

}
