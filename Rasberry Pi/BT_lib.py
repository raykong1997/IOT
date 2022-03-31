from bluetooth import *

class BT_device:
    timeout = 0
    max_timeout = 2000
    def __init__(self, host=0,name=0,port=1,socket=0,buff_size = 0):
        
        self.host = host
        self.name = name
        self.port = port
        self.socket = socket
        self.buff_size = buff_size
#------------------------------------------------------------------------------   
    def init_socket(self):
        
        self.socket=BluetoothSocket(RFCOMM)
        self.socket.connect((self.host, self.port))
    
        self.socket.setblocking(False)
        self.timeout = 0
        print("{} connected at port{}".format(self.host,self.port))
#------------------------------------------------------------------------------    
    def reconnect_socket(self,e):
        
        print("{} disconnected because of {}, trying to reconnect.".format(self.host,e))
        
        flat = True
        
        while True:
            
            try:
                nearby_devices = discover_devices(duration=2, lookup_names=True,flush_cache=True, lookup_class=False)
            
                for addr, name in nearby_devices:
                    
                    if name == self.name and addr == self.host:
                        flat = False
                    else:
                        pass
                
            except Exception as err:
                print (err)
                pass
        
            if flat == False:
                self.init_socket()
                break
            
            print("Searching... Can't find device")
        
        print("{} reconnected.".format(self.host))
#------------------------------------------------------------------------------    
    def recv_data(self):
        data = ""
        try:
            #print(self.timeout)
            data = self.socket.recv(self.buff_size).decode("utf-8")
            self.timeout = 0
            
        
        except Exception as err:
            #print(err)
            #if err.__class__.__name__ == "BluetoothError":
            #    print("error from {}, error code: {}".format(self.player,err))
                
            if err.__context__.args[1] == "Resource temporarily unavailable":
                
                #print(self.timeout)
                #self.timeout = self.timeout + 1    
                #if self.timeout > self.maxtimeout:
                #    self.reconnect_socket(err.__context__.args[1])
                pass
                    
            elif err.__context__.args[1] == "Connection timed out":
                self.reconnect_socket(err.__context__.args[1])
                    
        finally:
            return data
#------------------------------------------------------------------------------    
#    def send_data(self,data):
#        result = ""
#        try:
#            self.socket.send(data)
#            result = "{} sended to player{}".format(data,self.player)
#                
#        except Exception as err:
#            pass
#        finally:
#            return result
#------------------------------------------------------------------------------