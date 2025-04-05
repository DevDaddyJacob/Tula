package com.devjacob.tula.cli;

import com.devjacob.tula.utils.LogLevel;

public class CliOptions {
    private LogLevel logLevel = LogLevel.INFO;
    private String filePath = null;
    private boolean interactive = false;

    /**
     * <p>Controlled with the <code>--logging={level}</code> option.</p>
     * <ul>
     *      <li>None - <code>{0|n|none}</code></li>
     *      <li>Fatal - <code>{1|f|fatal}</code></li>
     *      <li>Error - <code>{2|e|error}</code></li>
     *      <li>Warn - <code>{3|w|warn}</code></li>
     *      <li>Info - <code>{4|i|info}</code></li>
     *      <li>Verbose - <code>{5|v|verbose}</code></li>
     *      <li>All - <code>{6|a|all}</code></li>
     * </ul>
     * <p>Default: <code>info</code></p>
     * @return The logging level of the program
     */
    public LogLevel getLogLevel() {
        return logLevel;
    }

    /**
     * <p>Controlled with the <code>--file=FILE_PATH</code> option.</p>
     * <p>Note: Optional if <code>-i</code> is used, otherwise required.</p>
     * @return The path to the file to parse
     */
    public String getFilePath() {
        return filePath;
    }

    /**
     * <p>Controlled with the <code>-i</code> flag, or <code>--interactive={on|off}</code> option.</p>
     * <p>Default: <code>false</code></p>
     * @return If the program is running in interactive mode
     */
    public boolean isInteractive() {
        return interactive;
    }

    public static class Builder {
        private String filePath = null;
        private boolean interactive = false;
        private LogLevel logLevel = LogLevel.INFO;

        public CliOptions build() {
            CliOptions options = new CliOptions();
            options.filePath = filePath;
            options.interactive = interactive;
            options.logLevel = logLevel;

            return options;
        }

        public Builder setFilePath(String filePath) {
            this.filePath = filePath;
            return this;
        }

        public Builder setInteractive(boolean interactive) {
            this.interactive = interactive;
            return this;
        }

        public Builder setLogLevel(LogLevel level) {
            this.logLevel = level;
            return this;
        }
    }
}
