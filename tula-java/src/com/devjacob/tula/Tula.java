package com.devjacob.tula;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;

import com.devjacob.tula.cli.TulaCLI;
import com.devjacob.tula.errors.TulaReporter;
import com.devjacob.tula.interpretation.evaluating.Interpreter;
import com.devjacob.tula.interpretation.parsing.Expr;
import com.devjacob.tula.interpretation.parsing.Parser;
import com.devjacob.tula.interpretation.scanning.Scanner;
import com.devjacob.tula.interpretation.tokens.Token;
import com.devjacob.tula.utils.LoggedClass;

public class Tula extends LoggedClass {
    private static final Interpreter interperter = new Interpreter();
    public static TulaCLI CLI;

    public Tula() {
        super(Tula.class);
    }

    public static void main(String[] args) throws IOException, FileNotFoundException {
        CLI = new TulaCLI(args);

        if (!CLI.getOptions().isInteractive() && CLI.getOptions().getFilePath() == null) {
            System.out.println("Usage: tulaj --file <file path>");
            System.exit(64); // EX_USAGE
            return;
        }

        if (CLI.getOptions().getFilePath() != null) {
            LOGGER.verbose("Running in file mode");
            runFile(CLI.getOptions().getFilePath());
            return;
        }

        if (CLI.getOptions().isInteractive()) {
            LOGGER.verbose("Running in interactive mode mode");
            runInteractive();
            return;
        }
    }

    private static void runFile(String path) throws IOException, FileNotFoundException {
        try (BufferedReader reader = new BufferedReader(new FileReader(path))) {
            StringBuilder sourceBld = new StringBuilder();
            String line;

            while ((line = reader.readLine()) != null) {
                sourceBld.append(line).append("\n");
            }
            
            run(sourceBld.toString());

            if (TulaReporter.hadError()) System.exit(65);
            if (TulaReporter.hadRuntimeError()) System.exit(70);
        } catch (FileNotFoundException ex) {
            throw ex;
        } catch (IOException ex) {
            throw ex;
        }
    }

    private static void runInteractive() throws IOException {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(System.in))) {
            for (;;) {
                System.out.print("> ");
                String line = reader.readLine();
                if (line == null) { break; }
                run(line);
                TulaReporter.resetHadError();
            }
        } catch (IOException ex) {
            throw ex;
        }
    }

    private static void run(String source) {
        Scanner scanner = new Scanner(source);
        List<Token> tokens = scanner.scanTokens();
        Parser parser = new Parser(tokens);
        Expr expression = parser.parse();

        // Stop if there was a syntax error.
        if (TulaReporter.hadError()) return;

        // System.out.println(new AstPrinter().print(expression));
        interperter.interpret(expression);
    }
}
