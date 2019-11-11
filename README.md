## Oscilloscope Arduino + Display 5110  

### Prerequisites  

What things you need to install the software and how to install them  

 - Docker v18.09+  
 - Git  
 - Make  

### Upload on your board  


1. Clone repository  
```
$ git clone git@github.com:ivankomolin/arduino-oscilloscope-5110.git ./
```

2. Change enviroments file .env  
```
$ nano .env
```

3. Install arduino and upload firmware on your board  
```
$ make install
$ make upload
```
