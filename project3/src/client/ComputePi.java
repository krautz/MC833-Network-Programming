package client;

import java.rmi.RMISecurityManager;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

import org.json.JSONException;

import java.math.BigDecimal;
import compute.Compute;

public class ComputePi {
	
	// main
	public static void main(String args[]) {
    	
    	// set security policy
    	System.setProperty("java.security.policy", "src/security.policy");
    	System.setSecurityManager(new RMISecurityManager());
    	
    	// check for security manager existance if it does not exist, create a new one
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        Scanner s = new Scanner(System.in);
        
        try {
        	// set the object name
            String name = "Compute";
            
            // create a registry searching for the server
            Registry registry = LocateRegistry.getRegistry("");
            Compute comp = (Compute) registry.lookup(name);
            
            // send request to execute Pi task
//            Pi task = new Pi(Integer.parseInt("300"));
//            BigDecimal pi = comp.executeTask(task);
//            System.out.println(pi);
            
            // send request to execute helloWorld task
            String param = s.nextLine();
            
            long start = System.currentTimeMillis();
            
            HelloWorld hellotask = new HelloWorld(param);
            try {
            	String stm = comp.executeTask(hellotask);
            	System.out.println(stm);
            }
            
            catch (JSONException j) {
            	System.out.println("error");
            }
            
            long end = System.currentTimeMillis();
        	
        	System.out.println("TEMPO SERVIDOR: " + (end - start));
              
        } 
        
        // any error ocurred during the creation: print and abort
        catch (Exception e) {
            System.err.println("ComputePi exception:");
            e.printStackTrace();
        }
    }    
}
