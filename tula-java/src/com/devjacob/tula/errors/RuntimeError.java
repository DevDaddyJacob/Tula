package com.devjacob.tula.errors;

import com.devjacob.tula.interpretation.tokens.Token;

public class RuntimeError extends RuntimeException {
    public final Token token;
  
    public RuntimeError(Token token, String message) {
        super(message);
        this.token = token;
    }
}
