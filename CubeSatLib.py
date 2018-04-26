def communicationTest(): #Verifies if LoRa is working
    protocol test here
    SPI_COMMUNICATION()
    print("Checking communication system:  %b" % info)
    return list [   status, data1 , data2 , ... , dataN  ]

def energyTest(): #Colects sensors data / battery level / temperature /
    protocol test here
    I2C_COMMUNICATION()
    print("Checking energy system:  %b" % info)
    return list [   status, data1 , data2 , ... , dataN  ]

def ADCTest(): #Colects sensors data / stability status  / motors status
    protocol test here
    SPI_COMMUNICATION()
    print("Checking ADC system:  %t" % info)
    return list [   status, data1 , data2 , ... , dataN  ]

def raspTest(): #Verifies if communication between rasps is working
    protocol test here
    UART_COMMUNICATION()
    print("Checking raspberry communication:  %b" % info)
    return list [  status, info  ]

def testDataAnalyzer(test_data, number_system):
    analyze protocol here
    return status

def checkSys ():
    system_data =  []
    systemData.append(communicationTest())
    systemData.append(energyTest())
    systemData.append(ADCTest())
    systemData.append(raspTest())
    start = testDataAnalyzer (system_data)
    system_data.append(start)
    return system_data

def TMSys(currentPack):
    
