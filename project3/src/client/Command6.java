package client;

import org.json.JSONException;
import org.json.JSONObject;
import compute.Task;
import java.io.Serializable;

public class Command6 implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;
    
    private String email;
    
    public Command6 (String email) {
    	this.email = email;
    }

    public String execute() throws JSONException {
    	
    	// get start time
    	long start = System.currentTimeMillis();

    	// create object with the opened file
    	JSONObject database = Utils.openFile();
   
    	// get person
    	String person = database.getJSONObject(this.email).toString();
    	
    	// get end time
    	long end = System.currentTimeMillis();
    	
    	// print total time spent
    	System.out.println("TEMPO CONSULTA: " + (end - start));
 
        return person;
    }
}
