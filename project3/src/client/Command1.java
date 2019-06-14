package client;

import org.json.JSONException;
import org.json.JSONObject;
import compute.Task;
import java.io.Serializable;
import java.util.Iterator;

public class Command1 implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;
    
    private String education;
    
    public Command1 (String education) {
        this.education = education;
    }

    public String execute() throws JSONException {
    	
    	// get start time
    	long start = System.currentTimeMillis();
    	
    	System.out.println(this.education);

    	// create object with the opened file
    	JSONObject database = Utils.openFile();
   
    	// create response
    	String persons = "";
    	
    	// iterate over object keys (get the keys)
    	Iterator<String> keys = database.keys();
    	while (keys.hasNext()) {

    		// get person
    		JSONObject person = database.getJSONObject(keys.next());
    		
    		// check if the education matches
    		if (person.getString("education").equals(this.education)) {
    			
    			// add person to response
    			persons = persons + person.toString() + " - ";
    		}
    	}
    	
    	// get end time
    	long end = System.currentTimeMillis();
    	
    	// print total time spent
    	System.out.println("TEMPO CONSULTA: " + (end - start));
 
        return persons;
    }
}
