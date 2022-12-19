#include<mosquitto.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern int publish(int dist, struct mosquitto *mosq);
int on_connect(struct mosquitto *mosq, void *obj, int rc){
    if(rc != 0){
        printf("Error connecting the MQTT client. Exited with code = %d\n", rc);
        return -1;
    }
    mosquitto_subscribe(mosq, NULL, "/dist", 0);
    mosquitto_subscribe(mosq, NULL, "/temp", 0);
    mosquitto_subscribe(mosq, NULL, "/press", 0);
    mosquitto_subscribe(mosq, NULL, "/alt", 0);
    printf("Subscribed to the topics\n");
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg){
    printf("Topic: %s; Received message: %s\n", msg->topic, (char *) msg->payload);
    if(strcmp(msg->topic, "/dist") == 0){
        printf("Received distance\n");
        char *val = trimwhitespace(msg->payload);
        int dist  = atoi(val);
        if(dist < 50){
            if(dist < 10){
                publish(-2, mosq);
            }
            else{
                publish(-1, mosq);
            }
        }
        else{
            if(dist > 100){
                publish(2, mosq);
            }
            else{
                publish(1, mosq);
            }
        }
    }
}


void listento(struct mosquitto *mosq){
    int rc;
    int deviceID = 0;
    mosquitto_lib_init();
    //struct mosquitto *mosq;
    
    mosq = mosquitto_new("server", true, &deviceID);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, "10.0.0.75", 1883, 60);
    if(rc){
        printf("Error connecting the MQTT client. Exited with code = %d\n", rc);
    }
    
    printf("Connected to the broker\n");
    //mosquitto_publish(mosq, NULL, "/dist", 1, "5", 0, false);
    
    mosquitto_loop_start(mosq);
    getchar();
    mosquitto_loop_stop(mosq, true);
    

/*
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    */
}
