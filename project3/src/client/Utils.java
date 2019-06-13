package client;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import org.json.JSONObject;

// class with static methods common to all operations
public class Utils {
	
	// open file and return json object
	public static JSONObject openFile() {
		
		// create string to receive database file
    	String sb = "";
    	
		try {
			// open database file
			BufferedReader reader = new BufferedReader(new FileReader("/home/krautz/mc833/project3/src/database.txt"));
			
			// get each line of the file and write it to the final buffer
			String line = reader.readLine();
			while(line != null) {
				sb = sb + line.trim();
				line = reader.readLine();
			}
			
			// close file
			reader.close();
		} 
		
		// exception: print it
		catch (IOException e) {
			e.printStackTrace();
		}
		
		return new JSONObject(sb);
		
	}
	
	// write json object to file
	public static void writeFile(JSONObject obj) {
		
		// write it to database file
    	try {
    		
    		// open file
			FileWriter file = new FileWriter("/home/krautz/mc833/project3/src/database.txt");
			
			// write to it
			file.write(obj.toString());
			
			// close file
			file.close();

		} 
    	
    	// exception: print it
    	catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
