// CalculatorCore.java
import java.io.*;

public class CalculatorCore {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Error");
            return;
        }

        // Join args into a single expression
        String expression = String.join(" ", args);

        try {
            double result = new Parser(expression).parse();
            System.out.println(result);

            // Store result in cache
            File cache = new File("/tmp/calculator_cache.txt");
            try (PrintWriter out = new PrintWriter(cache)) {
                out.println(result);
            }

        } catch (Exception e) {
            System.out.println("Error");
        }
    }
}

class Parser {
    private String expr;
    private int pos = -1;
    private int ch;

    public Parser(String expression) {
        // Remove invalid characters
        expr = expression.replaceAll("[^0-9\\.\\+\\-\\*\\/\\(\\) ]", "");
        // Add spaces around operators for easier tokenization
        expr = expr.replaceAll("([+\\-*/()])", " $1 ");
        this.expr = expr;
    }

    void nextChar() {
        ch = (++pos < expr.length()) ? expr.charAt(pos) : -1;
    }

    boolean eat(int charToEat) {
        while (ch == ' ') nextChar();
        if (ch == charToEat) {
            nextChar();
            return true;
        }
        return false;
    }

    public double parse() throws Exception {
        nextChar();
        double x = parseExpression();
        if (pos < expr.length()) throw new RuntimeException("Unexpected: " + (char)ch);
        return x;
    }

    double parseExpression() throws Exception {
        double x = parseTerm();
        for (;;) {
            if (eat('+')) x += parseTerm();
            else if (eat('-')) x -= parseTerm();
            else return x;
        }
    }

    double parseTerm() throws Exception {
        double x = parseFactor();
        for (;;) {
            if (eat('*')) x *= parseFactor();
            else if (eat('/')) x /= parseFactor();
            else return x;
        }
    }

    double parseFactor() throws Exception {
        if (eat('+')) return parseFactor();
        if (eat('-')) return -parseFactor();

        double x;
        int startPos = this.pos;
        if (eat('(')) {
            x = parseExpression();
            eat(')');
        } else if ((ch >= '0' && ch <= '9') || ch == '.') {
            while ((ch >= '0' && ch <= '9') || ch == '.') nextChar();
            x = Double.parseDouble(expr.substring(startPos, this.pos));
        } else {
            throw new RuntimeException("Unexpected: " + (char)ch);
        }

        return x;
    }
}

