from bluetooth import *
from BT_lib import BT_device
from player_lib import players
import time
import threading

#import lib----------------------------------------------------------

buf_size = 15
port_no = 1

player_num = 2
player = [players(),players()]
player0_alive_flag = False
global client_BT


sys_time = time.time()
client_BT_name = "ESP32_IOT"

#Set constant and variable-----------------------------------------------------------

def print_t(m):
    t = round(time.time() - sys_time,3)
    print(repr(str(t) +"s ~~> "+ str(m)))

def setup():
    print_t("Program Start, Performing inquiry...")
    global client_BT
    flat = False 
    while True:
        nearby_devices = discover_devices(duration=3, lookup_names=True,flush_cache=True, lookup_class=False)
        print_t("Found {} devices".format(len(nearby_devices)))
        
        for addr, name in nearby_devices:
            try:
                print_t("   {} - {}".format(addr, name))
                if name == client_BT_name:
                    client_BT = BT_device(host = addr, name = name, buff_size = buf_size, port = port_no)
                    flat = True
            except UnicodeEncodeError:
                print_t("   {} - {}".format(addr, name.encode("utf-8", "replace")))
        
        if flat == True:
            break
        else:
            print_t("Can't found device... please wait...")
        
    client_BT.init_socket()
        
    print_t("Setup completed!")
    time.sleep(1)

def BT_loop():
    global player
    global player0_alive_flag
    while True:
        data = client_BT.recv_data()
        if data:
            data = data.replace("\x01","")
            #print_t("Received {} from host {}".format(data,client_BT.host))
            #print_t(client_BT.send_data("GG" + str(i)))
            lst = []
            for i in data:
                j = False
                if int(i) == 1:j = True
                else:j = False
                lst.append(j)
            player[0].set(lst[0],lst[1],lst[2],lst[3],lst[4],lst[5])
            player[1].set(lst[6],lst[7],lst[8],lst[9],lst[10],lst[11])
            player0_alive_flag = lst[12]
    
#Define method-----------------------------------------------------------

if __name__ == '__main__':
    try:
        setup()
        thread = threading.Thread(target = BT_loop)
        thread.start()
        while True:
            time.sleep(1)
        
        
    except KeyboardInterrupt:
        print("\nkeyboard interrupt, Program stopped.")
        
        client_BT.socket.close()
        time.sleep(2)
        sys.exit(0)

#Main LOOP---------------------------------------------------------------