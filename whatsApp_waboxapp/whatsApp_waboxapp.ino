Client client("waboxapp.com", 443); 
String PostData = "token=my-test-api-key&uid=12025550123&to=12025550193&custom_uid=msg-4114&text=Hello world!"; 

if (client.connect()) { 
client.println("POST /api/send/chat HTTP/1.1"); 
client.println("Host: waboxapp.com"); 
client.println("Connection: close"); 
client.println("Content-Type: application/x-www-form-urlencoded;"); 
client.print("Content-Length: "); 
client.println(PostData.length()); 
client.println(); 
client.println(PostData); 
}
