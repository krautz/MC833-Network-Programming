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

    	// create object with the opened file
    	JSONObject my_obj = Utils.openFile();
    	
    	// iterate over object keys (get the keys)
    	Iterator<String> a = my_obj.keys();
    	while (a.hasNext()) 
            System.out.println(a.next());
    	

    	// modify given key value
    	my_obj.getJSONObject(key).put("name", "blebloblu");
    	
    	// get given object key field
    	String name = my_obj.getJSONObject(key).getString("name");
    	
    	// get end time
    	long end = System.currentTimeMillis();
    	
    	// write it to database file
    	Utils.writeFile(my_obj);
    	
    	// print total time spent
    	System.out.println("TEMPO SERVIDOR: " + (end - start));
 
        return name;
    }
}
