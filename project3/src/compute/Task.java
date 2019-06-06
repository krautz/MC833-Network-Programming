package compute;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Task<T> {
    T execute();
}