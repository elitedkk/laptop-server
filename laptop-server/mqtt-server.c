#include<mosquitto.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int publish(int action, struct mosquitto *mosq){
    char val[20];
    //printf("%f\n", dist);
    //gcvt(dist, 9, val);
    
    //printf("%s\n", val);
    
    int rc;
    int deviceId = 0;
    //mosquitto_lib_init();
    //struct mosquitto *mosq;
    sprintf(val, "%d", action);
    mosq = mosquitto_new("server-publisher", true, &deviceId);
    rc = mosquitto_connect(mosq, "10.0.0.75", 1883, 60);
    if(rc != 0){
        printf("Error connecting the MQTT client. Exited with code = %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    } 
    printf("Publishing now to the broker %s\n", val);
    

   
    int pub_result = mosquitto_publish(mosq, NULL, "/action", 1, action, 1, false);
    if(pub_result == MOSQ_ERR_SUCCESS){
        printf("Published to the broker\n");
    }
    else{
        printf("Publish not successful\n");
    }
    

    //mosquitto_disconnect(mosq);
    //mosquitto_destroy(mosq);
    //mosquitto_lib_cleanup();
    return 0;
}

