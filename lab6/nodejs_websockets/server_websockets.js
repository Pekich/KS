const WebSocket = require('ws');
var concurrentClientCount = 0;

const server = new WebSocket.Server({ port: 28563 });

function get_key(request){
  if(!(/^p=(\w+)$/.test(request)))  
    return "Error";
  return request.substring(2)  
}

server.on('connection', function connection(ws) {
  concurrentClientCount++;
  console.log(concurrentClientCount + " concurrent clients are connected");

  ws.on('message', function (data) {
    console.log("Data received: "+data)
    data = data.toString()
   
    key = get_key(data);
 
    if(key == "Error")
       ws.send("Ошибка");
    else {
        if (key == "Heli123copter")
          ws.send("Доступ к ресурсу открыт\n Конфиденциальные данные:42");
        else
        ws.send("Доступ запрещен");
    }   
   
  });

  ws.on('close', function close() {
    concurrentClientCount--;
  });
});

