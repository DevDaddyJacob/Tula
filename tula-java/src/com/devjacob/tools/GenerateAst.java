package com.devjacob.tools;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.List;

public class GenerateAst {
    private static final String I = "    ";

    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: generate_ast <output directory>");
            System.exit(64);
        }
        String outputDir = args[0].replaceAll("\"", "");
        
        defineAst(outputDir, "Expr", Arrays.asList(
            "Binary   : Expr left, Token operator, Expr right",
            "Grouping : Expr expression",
            "Literal  : Object value",
            "Unary    : Token operator, Expr right"
        ));
    }

    private static void defineAst(String outputDir, String baseName, List<String> types) throws IOException {
        String path = outputDir + "/" + baseName + ".java";
        PrintWriter writer = new PrintWriter(path, "UTF-8");

        writer.println("package com.devjacob.tula.interpretation.parsing;");
        writer.println();
        writer.println("import java.util.List;");
        writer.println();
        writer.printf("public abstract class %s {%n", baseName);
        
        defineVisitor(writer, baseName, types);

        // The AST classes.
        for (String type : types) {
            String className = type.split(":")[0].trim();
            String fields = type.split(":")[1].trim(); 
            defineType(writer, baseName, className, fields);
        }

        // The base accept() method.
        writer.println();
        writer.printf("%spublic abstract <R> R accept(Visitor<R> visitor);%n", I);

        writer.println("}");
        writer.close();
    }

    private static void defineVisitor(PrintWriter writer, String baseName, List<String> types) {
        writer.printf("%spublic interface Visitor<R> {%n", I);
  
        for (String type : types) {
            String typeName = type.split(":")[0].trim();
            writer.printf("%s%sR visit%s%s(%s %s);%n", I, I, typeName, baseName, typeName, baseName.toLowerCase());
        }
    
        writer.printf("%s}%n%n", I);
    }

    private static void defineType(PrintWriter writer, String baseName, String className, String fieldList) {
        String[] fields = fieldList.split(", ");
        writer.printf("%spublic static class %s extends %s {%n", I, className, baseName);
  
        // Fields.
        for (String field : fields) {
            writer.printf("%s%spublic final %s;%n", I, I, field);
        }
        writer.println();

        
        // Constructor.
        writer.printf("%s%spublic %s(%s) {%n", I, I, className, fieldList);

        // Store parameters in fields.
        for (String field : fields) {
            String name = field.split(" ")[1];
            writer.printf("%s%s%sthis.%s = %s;%n", I, I, I, name, name);
        }

        writer.printf("%s%s}%n", I, I);

        // Visitor pattern.
        writer.println();
        writer.printf("%s%s@Override%n", I, I);
        writer.printf("%s%spublic <R> R accept(Visitor<R> visitor) {%n", I, I);
        writer.printf("%s%s%sreturn visitor.visit%s%s(this);%n", I, I, I, className, baseName);
        writer.printf("%s%s}%n", I, I);

        writer.printf("%s}%n", I);
        writer.println();
    }
}
