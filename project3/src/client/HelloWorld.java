package client;

import org.json.JSONException;
import org.json.JSONObject;
import compute.Task;
import java.io.Serializable;

public class HelloWorld implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;
    
    private String key;
    
    public HelloWorld (String key) {
        this.key = key;
    }
    
    /**
     * Calculate pi.
     */
    public String execute() throws JSONException {
    	
    	long start = System.currentTimeMillis();

    	JSONObject my_obj = new JSONObject("{'0': { 'name': 'caio', 'cargo': 'estudante'}, '1': { 'name': 'allan', 'cargo': 'estudante2'}}");
    	
    	String name = my_obj.getJSONObject(key).getString("name");
    	
    	long end = System.currentTimeMillis();
    	
    	System.out.println("TEMPO SERVIDOR: " + (end - start));
 
        return name;
    }
}
