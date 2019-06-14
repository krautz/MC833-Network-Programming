package client;

import org.json.JSONException;
import org.json.JSONObject;
import compute.Task;
import java.io.Serializable;
import java.util.Iterator;

public class Command2 implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;
    
    private String city;
    
    public Command2 (String city) {
        this.city = city;
    }

    public String execute() throws JSONException {
    	
    	// get start time
    	long start = System.currentTimeMillis();

    	// create object with the opened file
    	JSONObject database = Utils.openFile();
   
    	// create response
    	String skills = "";
    	
    	// iterate over object keys (get the keys)
    	Iterator<String> keys = database.keys();
    	while (keys.hasNext()) {

    		// get person
    		JSONObject person = database.getJSONObject(keys.next());
    		
    		// check if the city matches
    		if (person.getString("city").equals(this.city)) {
    			
    			// add person experiences to response
    			skills = skills + person.getJSONArray("skills").toString() + " - ";
    		}
    	}
    	
    	// get end time
    	long end = System.currentTimeMillis();
    	
    	// print total time spent
    	System.out.println("TEMPO CONSULTA: " + (end - start));
 
        return skills;
    }
}
