/**
    Ce fichier est utilise pour le developpement seulement 
    les valeurs sont remplacees par buildconfig.py a la compilation du projet
    */
    #ifndef IOB_IOT_CONF_H
    #define IOB_IOT_CONF_H

    #define RELAY_PIN 0 
    #define BUTTON_PIN 2 
    #define DEBOUNCE_TIME 100 
    #define BUTTON_PRESS_COUNT_MAX 5 
    #define IDXDEVICE 13 
    #define NOMMODULE "sonnette" 
    #define NCORNO true 

    
    #define USE_WIFI
    #ifdef USE_WIFI

        #define MYSSID "my_ssid" 
        #define MYSSID_PASSWORD "my_ssid_password" 

        #define USE_OTA
        #ifdef USE_OTA
            #define OTANAME "default"
            #define OTAPASSWORD "password"
        #endif /* USE_OTA */ 

        #define USE_IPFIXE
        #ifdef USE_IPFIXE
            #define IPFIXE "192.168.1.51" 
            #define GATEWAY "192.168.1.1" 
            #define SUBNET "255.255.255.0" 
            #define DNS "192.168.1.1" 
        #endif /* USE_IPFIXE */ 

        #define USE_MQTT
        #ifdef USE_MQTT
            #define MQTT_PORT 1883 
            #define TOPICIN "domoticz/out" 
            #define TOPICOUT "domoticz/in" 
            #define MQTT_SERVER "192.168.1.2" 
            #define MQTT_LOGIN "login"
            #define MQTT_PASSWORD "password"
        #endif /* USE_MQTT */ 

        #define USE_HTTP
        #ifdef USE_HTTP
            #define DOMOTIC_SERVER "192.168.1.2" 
            #define DOMOTIC_PORT 8080 
        #endif /* USE_HTTP */ 

        #define USE_WEBSERVER
        #ifdef USE_WEBSERVER
            #define WEBSERVER_PORT 80
        #endif /* USE_WEBSERVER */ 

    #endif /* USE_WIFI */ 

    #endif /* IOB_IOT_Conf_h */ 