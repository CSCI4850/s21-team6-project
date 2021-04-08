import java.io.FileWriter;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileNotFoundException;

//Data validation for GO Neural Net input
public class Validator {

	public static void main(String[] args) throws IOException {
		
			FileWriter writer;
			BufferedReader reader;
			try
	        {
				writer = new FileWriter("nineFullData(2)_Validated.txt");
	            reader = new BufferedReader(new FileReader("nineFullData(2).txt"));
	            String line = reader.readLine();
	            while (line != null) {
					if(!line.substring(line.length() - 4).contains("-")) {
						writer.write(line + "\n");
					}
					line = reader.readLine();
				}
	            
	            reader.close();
	            writer.close();
	        }
	        catch (FileNotFoundException e)
	        {
	            System.out.println("File not found");
	        }
	}

}
