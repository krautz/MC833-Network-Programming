package client;

import java.rmi.NotBoundException;
import java.rmi.RMISecurityManager;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

import org.json.JSONException;

import compute.Compute;

public class Client {
	
	// main
	public static void main(String args[]) throws RemoteException, NotBoundException {
		
		// get command
        System.out.println("Insira o comando seguido dos parametros separado por '-':");
        System.out.println("Exemplo Comando 1: 1-Engenharia da computacao ** listar todas as pessoas formadas em um determinado curso");
        System.out.println("Exemplo Comando 2: 2-Campinas **  listar as habilidades dos perfis que moram em uma determinada cidade;");
        System.out.println("Exemplo Comando 3: 3-carlos_silva@gmail.com-cozinheiro ** acrescentar uma nova experiência em um perfil");
        System.out.println("Exemplo Comando 4: 4-carlos_silva@gmail.com ** dado o email do perfil, retornar sua experiência");
        System.out.println("Exemplo Comando 5: 5 ** listar todas as informações de todos os perfis");
        System.out.println("Exemplo Comando 6: 6-carlos_silva@gmail.com ** dado o email de um perfil, retornar suas informações");
        Scanner scanner = new Scanner(System.in);
        String command = scanner.nextLine();
        String[] commands = command.split("-");
        scanner.close();
        
        // get time
        long start = System.currentTimeMillis();
    	
    	// set security policy
    	System.setProperty("java.security.policy", "src/security.policy");
    	System.setSecurityManager(new RMISecurityManager());
    	
    	// check for security manager existance if it does not exist, create a new one
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        
        // set the object name
        String name = "Compute";
        
        // create a registry searching for the server
        Registry registry = LocateRegistry.getRegistry("");
        Compute comp = (Compute) registry.lookup(name);
        
        // create response string
        String response = "";
        
        // execute the right command
        try {
	        if (commands[0].equals("1")) {
	        	Command1 task = new Command1(commands[1]);
	        	response = comp.executeTask(task);
	        } else if (commands[0].equals("2")) {
	        	Command2 task = new Command2(commands[1]);
	        	response = comp.executeTask(task);
	        } else if (commands[0].equals("3")) {
	        	Command3 task = new Command3(commands[1], commands[2]);
	        	response = comp.executeTask(task);
	        } else if (commands[0].equals("4")) {
	        	Command4 task = new Command4(commands[1]);
	        	response = comp.executeTask(task);
	        } else if (commands[0].equals("5")) {
	        	Command5 task = new Command5();
	        	response = comp.executeTask(task);
	        } else if (commands[0].equals("6")) {
	        	Command6 task = new Command6(commands[1]);
	        	response = comp.executeTask(task);
	        } else {
	        	response = "Comando Inválido!";
	        }
        }
        
        // any erro occured: report it
	    catch (JSONException e) {
	    	response = "Erro ao executar a consulta. Provável erro de entrada";
	    }
        
        // print response
        System.out.println("Resposta: " + response);
        
        // get time
        long end = System.currentTimeMillis();
    	
        // print total time
    	System.out.println("TEMPO TOTAL: " + (end - start));
          
    } 
  
}
