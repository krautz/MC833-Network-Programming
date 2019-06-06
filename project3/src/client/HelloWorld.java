package client;

import compute.Task;
import java.io.Serializable;
import java.math.BigDecimal;

public class HelloWorld implements Task<String>, Serializable {

    private static final long serialVersionUID = 227L;


    /**
     * Calculate pi.
     */
    public String execute() {
        return "Hello World !!!";
    }
}
