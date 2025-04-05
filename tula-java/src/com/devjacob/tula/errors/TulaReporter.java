package com.devjacob.tula.errors;

import com.devjacob.tula.interpretation.tokens.Token;
import com.devjacob.tula.interpretation.tokens.TokenType;
import com.devjacob.tula.utils.LogLevel;
import com.devjacob.tula.utils.LoggedClass;
import com.devjacob.tula.utils.Logger;

public class TulaReporter extends LoggedClass {
    private static final Logger ERR_LOG = Logger.get(TulaReporter.class, LogLevel.ERROR, "%message");
    private static boolean hadError = false;
    private static boolean hadRuntimeError = false;
    private static TulaReporter instance;

    private TulaReporter() {
        super(TulaReporter.class);
    }

    public static boolean hadError() {
        return hadError;
    }

    public static void resetHadError() {
        hadError = false;
    }

    public static boolean hadRuntimeError() {
        return hadRuntimeError;
    }

    public static void resetHadRuntimeError() {
        hadRuntimeError = false;
    }
    
    public static void error(int line, String message) {
        report(line, "", message);
    }

    public static void error(Token token, String message) {
        if (token.type == TokenType.EOF) {
            report(token.line, " at end", message);
        } else {
            report(token.line, " at '" + token.lexeme + "'", message);
        }
    }

    public static void runtimeError(RuntimeError error) {
        System.err.println(error.getMessage() + "\n[line " + error.token.line + "]");
        hadRuntimeError = true;
    }

    private static void report(int line, String where, String message) {
        ERR_LOG.error("[line " + line + "] Error" + where + ": " + message);
        hadError = true;
    }

    public TulaReporter instance() {
        if (instance == null) { instance = new TulaReporter(); }
        return instance;
    }
}
