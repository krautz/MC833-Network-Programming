package client;

import org.json.JSONException;
import org.json.JSONObject;
import compute.Task;
import java.io.Serializable;

public class Command5 implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;

    public String execute() throws JSONException {
    	
    	// get start time
    	long start = System.currentTimeMillis();

    	// create object with the opened file
    	JSONObject database = Utils.openFile();
    	
    	// get end time
    	long end = System.currentTimeMillis();
    	
    	// print total time spent
    	System.out.println("TEMPO CONSULTA: " + (end - start));
 
        return database.toString();
    }
}
