package client;

import org.json.JSONException;
import org.json.JSONObject;
import compute.Task;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Serializable;
import java.util.Iterator;

public class HelloWorld implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;
    
    private String key;
    
    public HelloWorld (String key) {
        this.key = key;
    }
    
    public String execute() throws JSONException {
    	
    	// get start time
    	long start = System.currentTimeMillis();

    	
    	// create string to receive database file
    	String sb = "";
		try {
			// open database file
			BufferedReader reader = new BufferedReader(new FileReader("/home/krautz/mc833/project3/src/database.txt"));
			
			// get each line of the file and write it to the final buffer
			String line = reader.readLine();
			while(line != null) { 
				sb = sb + line;
				line = reader.readLine();
			}
			
			// close file
			reader.close();
			
			// remove all whitespaces from the buffer
			sb = sb.replaceAll("\\s+","");
			
			// print it
			System.out.println(sb);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

    	// create object with the opened file
    	JSONObject my_obj = new JSONObject(sb);
    	
    	// iterate over object keys (get the keys)
    	Iterator<String> a = my_obj.keys();
    	while (a.hasNext()) 
            System.out.println(a.next());
    	

    	// modify given key value
    	my_obj.getJSONObject(key).put("name", "ble");
    	
    	// get given object key field
    	String name = my_obj.getJSONObject(key).getString("name");
    	
    	// get end time
    	long end = System.currentTimeMillis();
    	
    	// serialize object
    	System.out.println(my_obj.toString());
    	
    	// write it to database file
    	try {
    		// open file
			FileWriter file = new FileWriter("/home/krautz/mc833/project3/src/database.txt");
			
			// write to it
			file.write(my_obj.toString());
			
			// close file
			file.close();

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	// print total time spent
    	System.out.println("TEMPO SERVIDOR: " + (end - start));
 
        return name;
    }
}
