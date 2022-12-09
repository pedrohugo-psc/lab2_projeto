import sqlite3, os, datetime as dt
import serial, signal, sys, serial.tools.list_ports

def interrompe_sinal(sinal, frame):
    print("Ctrl+C")
    sys.exit(0)

signal.signal(signal.SIGINT, interrompe_sinal)

ports  =list(serial.tools.list_ports.comports())
for SerialPortName in ports:
    print(SerialPortName)

try:
    porta="COM1"#input("digite a porta em que o arduino esta conectado")
    ser = serial.Serial(str(porta))
    print(ser.name)
except:
    print("porta inexistente")
    sys.exit(0)

while(1):
    dadosSerial = ser.read_until()
    dadosStr = str(dadosSerial)
    dadosInicial = dadosStr.index("{")
    dadosInicial = dadosStr.index("}")
    valoresMedidos = dadosStr[dadosInicial:(dadosFinal+1)]
    print(valoresMedidos)

serial.close()


path = os.getcwd()
cnxn = sqlite3.connect(path+r"\telegestao.db")
cursor = cnxn.cursor()

luminosidade = 2.5
temperatura = 3.1
tensao = 1.1
corrente = 6.0

cursor.execute("CREATE TABLE IF NOT EXISTS Luminosidade (Momento datetime NOT NULL, Valor decimal(3,3) NOT NULL)")
cursor.execute("CREATE TABLE IF NOT EXISTS Temperatura (Momento datetime NOT NULL, Valor decimal(3,3) NOT NULL)")
cursor.execute("CREATE TABLE IF NOT EXISTS Tensao (Momento datetime NOT NULL, Valor decimal(3,3) NOT NULL)")
cursor.execute("CREATE TABLE IF NOT EXISTS Corrente (Momento datetime NOT NULL, Valor decimal(3,3) NOT NULL)")

cursor.execute("INSERT INTO Luminosidade VALUES (?, ?)", [str(dt.datetime.now()), luminosidade])
cursor.execute("INSERT INTO Temperatura VALUES (?, ?)", [str(dt.datetime.now()), temperatura])
cursor.execute("INSERT INTO Tensao VALUES (?, ?)", [str(dt.datetime.now()), tensao])
cursor.execute("INSERT INTO Corrente VALUES (?, ?)", [str(dt.datetime.now()), corrente])

cnxn.commit()
cnxn.close