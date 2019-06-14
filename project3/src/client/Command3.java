package client;

import org.json.JSONException;
import org.json.JSONObject;
import compute.Task;
import java.io.Serializable;

public class Command3 implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;
    
    private String email;
    private String skill;
    
    public Command3 (String email, String skill) {
    	this.email = email;
    	this.skill = skill;
    }

    public String execute() throws JSONException {
    	
    	// get start time
    	long start = System.currentTimeMillis();

    	// create object with the opened file
    	JSONObject database = Utils.openFile();
    	
    	// add experience
    	database.getJSONObject(this.email).getJSONArray("experiences").put(this.skill);
    	
    	// write it to database file
    	Utils.writeFile(database);
    	
    	// get end time
    	long end = System.currentTimeMillis();
    	
    	// print total time spent
    	System.out.println("TEMPO CONSULTA: " + (end - start));
 
        return "skill added";
    }
}
