
package engine;

import java.rmi.RMISecurityManager;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;

import compute.Compute;
import compute.Task;

public class ComputeEngine implements Compute {

	// constructor
    public ComputeEngine() {
    	// call parent class (Compute)
        super();
    }
    
    // first method
    public <T> T executeTask(Task<T> t) {
        return t.execute();
    }

    // main
    public static void main(String[] args) {
    	
    	// create scanner
    	Scanner scanner = new Scanner(System.in);
    	
    	// get chosen server name
    	System.out.println("Insira o nome do server:");
    	String name = scanner.nextLine();
    	
    	// set security policy
    	System.setProperty("java.security.policy", "src/security.policy");
    	System.setSecurityManager(new RMISecurityManager());
    	
    	// check for security manager existance if it does not exist, create a new one
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        
        try {            
            // create the new engine
            Compute engine = new ComputeEngine();
            
            // create the stub to an port 1099
            Compute stub =
                (Compute) UnicastRemoteObject.exportObject(engine, 0);
            Registry registry = LocateRegistry.createRegistry(1099);
            
            // bind the object name and stub
            registry.rebind(name, stub);
            System.out.println("ComputeEngine bound");
        } 
        
        // any error ocurred during the creation: print and abort
        catch (Exception e) {
            System.err.println("ComputeEngine exception:");
            e.printStackTrace();
        }
    }
}
