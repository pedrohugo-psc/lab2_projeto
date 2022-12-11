import sqlite3
import serial
import signal
import sys
import serial.tools.list_ports
import time
import datetime as dt

def signal_handler(signal, frame):
    print("CTRL-C pressionado")
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

print("CTRL-C para sair")

ports = list(serial.tools.list_ports.comports())
for SerialPortName in ports:
    print(SerialPortName)

try:
    porta=input("Digite o nome da porta serial onde o Arduino está conectado, ex.: COMx :")
    ser = serial.Serial(str(porta))  # open serial port  
    print(ser.name)   

except serial.SerialException:
    print("Porta inexistente")  
    sys.exit(0) 

path = 'F:\\Documentos\\UFBA\\Lab Integrado II\\PROJETO\\lab2_projeto'

cnxn = sqlite3.connect(path+r"\telegestao.db")

cursor=cnxn.cursor()

cursor.execute("CREATE TABLE IF NOT EXISTS Medidas (Momento datetime NOT NULL, Valor varchar(100) NOT NULL)")


while(1):
    serDados = ser.read_until((b'\r'))
    print(serDados)

    dadosStr=str(serDados)

    measureValues=dadosStr

    lista_Values = measureValues.split(",")     #Cria uma lista 'x'com a string - 
                                                #Divida uma string em uma lista onde cada palavra é um item da lista: 

    medidas=tuple(lista_Values)            #Converta a lista em tupla   

    print("Valores para o banco de dados Ardino.db: "+str(medidas))
    
    cursor.execute("INSERT INTO Medidas VALUES (?, ?)", [str(dt.datetime.now()), str(medidas)])  #uso os elementos da tupla para inserir os valores no registro


    cnxn.commit()
    time.sleep(1)

cnxn.close()
ser.close()




















