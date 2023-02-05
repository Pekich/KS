#!/usr/bin/python
# -*- coding: UTF-8 -*-

from twisted.internet import reactor, protocol, endpoints
import re
import sys

class ProcessClient(protocol.Protocol):
    def __init__(self, server):
        self.server = server
        self.dataForProcessing = ""

    def connectionMade(self):
        self.server.concurrentClientCount += 1
        print(str(self.server.concurrentClientCount) + " concurrent clients are connected")

    def connectionLost(self, reason):
        self.server.concurrentClientCount -= 1

    def sendResponse(self,response):
        pattern = ("<html><head></head><body><p>" + response+ "</p></body></html>").encode('utf8')
        print(pattern)
        self.transport.write(("HTTP/1.0 200 OK\n").encode('utf8'))
        self.transport.write(("Server: Python\n").encode('utf8'))
        self.transport.write(("Content-length: " + str(len(pattern)) + "\n").encode('utf8'))
        self.transport.write(("Connection: close").encode('utf8'))
        self.transport.write(("Content-Type: text/html; charset=utf-8\n\n").encode('utf8'))
        self.transport.write(pattern)
        self.transport.loseConnection()


    def checkPassword(self,data):
        pass_str = re.search(r'server\.py\?p=(\w+)',data)
        if (pass_str == None):
            return "Error"
        pasw = pass_str.group(1)
        if(pasw == "Heli123copter"):
            return "Confidential data:42"
        return "Incorrect password!"

    def dataReceived(self, data):
        #self - это клиент
        data = data.decode('utf8')
        print("Data received: "+data+"\r\n")
        self.sendResponse(self.checkPassword(data))
       
        
class Server(protocol.Factory):
    def __init__(self):
        self.concurrentClientCount = 0

    def buildProtocol(self, addr):
        return ProcessClient(self)

endpoints.serverFromString(reactor, "tcp:28563").listen(Server())
reactor.run()
