# NTP-Client
Creating a simple NTP (Network TIme Protocol) client in C

Running the code
```
$ gcc ntp_client.c
$ ./a.out
``` 
In the NTP Client code, the hostname can be changed to the respective countries' NTP server address.

```
char * hostname = "in.pool.ntp.org";
```
for India, or

```
char * hostname = "us.pool.ntp.org";
```
for US.

