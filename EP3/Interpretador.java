import java.io.*;
import java.util.*;

class Interpretador {
    static String inputFileName = "testeula.ula";
    static String outputFileName = "testeula.hex";
    static HashMap<String, Character> operationMap = new HashMap<String, Character>();

    public static void initHash() {
        operationMap.put("an", '0');
        operationMap.put("naob", '1');
        operationMap.put("anb", '2');
        operationMap.put("zerol", '3');
        operationMap.put("naeb", '4');
        operationMap.put("bn", '5');
        operationMap.put("axb", '6');
        operationMap.put("abn", '7');
        operationMap.put("anob", '8');
        operationMap.put("naxb", '9');
        operationMap.put("copiab", 'A');
        operationMap.put("ab", 'B');
        operationMap.put("uml", 'C');
        operationMap.put("aobn", 'D');
        operationMap.put("aob", 'E');
        operationMap.put("copiaa", 'F');
    }

    public static String substring(String line, int i, char j) {
        String result = "";

        for (int k = i; line.charAt(k) != j; result += line.charAt(k), k++)
            ;

        return result;
    }

    public static void main(String[] args) {
        String line, operation, output = "";
        char x = 'F', y = 'F', w;

        initHash(); // inicializar o hash

        if ((args.length >= 2) && (args[0].compareTo("-n") == 0)) {
            inputFileName = args[1];
            outputFileName = args[2];
        }

        try (BufferedReader br = new BufferedReader(new FileReader(inputFileName))) {
            // Para cada linha do arquivo, atribuir o valor para a variável daquela linha
            while ((line = br.readLine()) != null) {
                line = line.replaceAll(" ", "").toLowerCase();
                if (line.length() > 0) {
                    switch (line.charAt(0)) {
                        // Caso x, y ou w
                        case 'x':
                            x = line.charAt(2);
                            break;
                        case 'y':
                            y = line.charAt(2);
                            break;
                        case 'w':
                            // Caso seja W, procura o respectivo valor hexadecimal
                            // no hashmap
                            operation = substring(line, 2, ';');
                            System.out.println(operation);
                            w = operationMap.get(operation);

                            StringBuilder sb = new StringBuilder();
                            sb.append(x);
                            sb.append(y);
                            sb.append(w);
                            sb.append('\n');

                            output += sb.toString().toUpperCase();

                            break;
                    }
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("Não foi possível encontrar o arquivo. Error: " + e.getMessage());
        } catch (IOException e) {
            System.out.println("Não foi possível abrir o arquivo. Error: " + e.getMessage());
        } catch (Exception e) {
            System.out.println("Houve um erro. Erro: " + e.getMessage());
            e.printStackTrace();
        }

        if (output.length() > 0) {
            try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFileName))) {
                bw.write(output);
            } catch (IOException e) {
                System.out.println("Não foi possível abrir o arquivo. Error: " + e.getMessage());
            } catch (Exception e) {
                System.out.println("Houve um erro. Erro: " + e.getMessage());
                e.printStackTrace();
            }
        }
    }
}