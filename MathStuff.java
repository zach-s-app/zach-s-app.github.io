// MathStuff.java
class MathStuff {
    public static void main(String[] args) {
        if (args.length != 3) {
            System.out.println("Usage: java MathStuff.java <num1> <operator> <num2>");
            return;
        }

        try {
            double a = Double.parseDouble(args[0]);
            String op = args[1];
            double b = Double.parseDouble(args[2]);
            double result;

            switch (op) {
                case "+":
                    result = a + b;
                    break;
                case "-":
                    result = a - b;
                    break;
                case "*":
                    result = a * b;
                    break;
                case "/":
                    if (b == 0) {
                        System.out.println("Error: Division by zero");
                        return;
                    }
                    result = a / b;
                    break;
                default:
                    System.out.println("Unsupported operator. Use +, -, *, or /");
                    return;
            }

            System.out.println(a + " " + op + " " + b + " = " + result);

        } catch (NumberFormatException e) {
            System.out.println("Error: Invalid number format");
        } catch (Exception e) {
            System.out.println("Unexpected error: " + e.getMessage());
        }
    }
}

