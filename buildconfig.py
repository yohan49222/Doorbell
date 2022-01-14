Import("env")
import os.path
import json
#import ipaddress

#
# Creation du fichier include/IOB_IOT_Conf.h pour le developpement
# #define sans valeur
#
def ConfigH():
    content = """/**
Ce fichier est utilise pour le developpement seulement 
les valeurs sont remplacees par buildconfig.py a la compilation du projet
*/
#ifndef IOB_IOT_Conf_h
#define IOB_IOT_Conf_h

#define RELAY_PIN 0 
#define BUTTON_PIN 2 
#define DEBOUNCE_TIME 100 
#define BUTTON_PRESS_COUNT_MAX 5 
#define IDXDEVICE 13 
#define NOMMODULE "sonnette" 
#define NCORNO true

"""

    lines = open('config.json', "r")
    conf = json.loads(lines.read())

    check = conf["USE_WIFI"]
    if check and int(check) == 1:
        content += """
#define USE_WIFI
#ifdef USE_WIFI

    #define MYSSID "my_ssid" 
    #define MYSSID_PASSWORD "my_ssid_password" """

        check = conf["USE_OTA"]
        if check and int(check) == 1:
            content += """

    #define USE_OTA
    #ifdef USE_OTA
        #define OTANAME "default"
        #define OTAPASSWORD "password"
    #endif /* USE_OTA */ """

        check = conf["USE_IPFIXE"]
        if check and int(check) == 1:
            content += """

    #define USE_IPFIXE
    #ifdef USE_IPFIXE
        #define IPFIXE "192.168.1.51" 
        #define GATEWAY "192.168.1.1" 
        #define SUBNET "255.255.255.0" 
        #define DNS "192.168.1.1" 
    #endif /* USE_IPFIXE */ """

        check = conf["USE_MQTT"]
        if check and int(check) == 1:
            content += """

    #define USE_MQTT
    #ifdef USE_MQTT
        #define MQTT_PORT 1883 
        #define TOPICIN "domoticz/out" 
        #define TOPICOUT "domoticz/in" 
        #define MQTT_SERVER "192.168.1.2" 
        #define MQTT_LOGIN "login"
        #define MQTT_PASSWORD "password"
    #endif /* USE_MQTT */ """

        check = conf["USE_HTTP"]
        if check and int(check) == 1:
            content += """

    #define USE_HTTP
    #ifdef USE_HTTP
        #define DOMOTIC_SERVER "192.168.1.2" 
        #define DOMOTIC_PORT 8080 
    #endif /* USE_HTTP */ """

        check = conf["USE_WEBSERVER"]
        if check and int(check) == 1:
            content += """

    #define USE_WEBSERVER
    #ifdef USE_WEBSERVER
        #define WEBSERVER_PORT 80
    #endif /* USE_WEBSERVER */ """

        check = conf["USE_WIFI"]
        if check and int(check) == 1:
            content += """

#endif /* USE_WIFI */ """

    content += """

#endif /* IOB_IOT_Conf_h */ """

    fileH = open('include/IOB_IOT_Conf.h', 'w')
    fileH.write(content)
    fileH.close()

#
# Make default config.json
#
def initialConf():
    file = open('config.json', 'w')
    content = """
    {
        "UPLOAD_WITH_OTA":"0",
        "RELAY_PIN":"0",
        "BUTTON_PIN":"2",
        "NCORNO":"1",
        "DEBOUNCE_TIME":"100",
        "BUTTON_PRESS_COUNT_MAX":"5",
        "NOMMODULE":"sonnette",
        "IDXDEVICE":"13",
        "USE_MQTT":"1",
        "USE_WEBSERVER":"1",
        "USE_HTTP":"1",
        "USE_IPFIXE":"1",
        "USE_OTA":"0",
        "USE_WIFI":"1",
        "IP":
        {
            "IPFIXE":"192.168.1.51",
            "GATEWAY":"192.168.1.1",
            "SUBNET":"255.255.255.0",
            "DNS":"192.168.1.1"
        },
        "OTA":
        {
            "OTAIP":"192.168.1.51",
            "OTANAME":"sonnette",
            "OTAPASSWORD":""
        },
        "WIFI":
        {
            "SSID":"",
            "SSIDPASSWORD":""
        },
        "WEBSERVER":
        {
            "WEBSERVER_PORT":"80"
        },
        "HTTP":
        {
            "DOMOTIC_SERVER":"192.168.1.2",
            "DOMOTIC_PORT":"8080"
        },
        "MQTT":
        {
            "TOPICIN":"domoticz/out",
            "TOPICOUT":"domoticz/in",
            "MQTT_SERVER":"192.168.1.2",
            "MQTT_PORT":"1883",
            "MQTT_LOGIN":"",
            "MQTT_PASSWORD":""
        }
    }"""
    file.write(content)
    file.close()
    print("Config file created: 'config.json' , please set beafore build" )


if os.path.isfile('config.json') == False:
    initialConf()
    

if os.path.isfile('config.json'):
    lines = open('config.json', "r")
    conf = json.loads(lines.read())
    
    keys = ["RELAY_PIN","BUTTON_PIN","DEBOUNCE_TIME","BUTTON_PRESS_COUNT_MAX","IDXDEVICE"]
    for key in keys:
        env.Append(CPPDEFINES=[(key, int(conf[key]))])

    env.Append(CPPDEFINES=[("NOMMODULE", "\\\"" + conf["NOMMODULE"] + "\\\"")])

    check = conf["NCORNO"]
    if check and int(check) == 1:
        env.Append(CPPDEFINES=[("NCORNO", "\\\"true\\\"" )])
    else:
        env.Append(CPPDEFINES=[("NCORNO", "\\\"false\\\"" )])

    check = conf["USE_WIFI"]
    if check and int(check) == 1:
        print("Using WIFI")
        env.Append(CPPDEFINES=[("USE_WIFI")])
        env.Append(CPPDEFINES=[("MYSSID", "\\\"" + conf["WIFI"]["SSID"] + "\\\"")])
        env.Append(CPPDEFINES=[("MYSSID_PASSWORD", "\\\"" + conf["WIFI"]["SSIDPASSWORD"] + "\\\"")])

        check = conf["USE_OTA"]
        if check and int(check) == 1:

            print("\tUsing OTA")
            port = conf["OTA"]["OTAIP"]
            password = conf["OTA"]["OTAPASSWORD"]
            env.Append(CPPDEFINES=[("OTANAME", "\\\"" + conf["OTA"]["OTANAME"] + "\\\"")])
            if len(port) > 0 and len(password):
                env.Append(CPPDEFINES=[("USE_OTA")])
                env.Append(CPPDEFINES=[("OTAPASSWORD", "\\\"" + password + "\\\"")])

                check = conf["UPLOAD_WITH_OTA"]
                if check and int(check) == 1:
                    if env["PIOENV"] == "nodemcuv2":
                        env.Replace(
                            UPLOAD_PROTOCOL="espota",
                            UPLOAD_PORT=port,
                            UPLOAD_FLAGS=["--port=8266", "--auth=" + password],
                        )

        check = conf["USE_IPFIXE"]
        if check and int(check) == 1:
            print("\tUsing IPFIXE")
            env.Append(CPPDEFINES=[("USE_IPFIXE")])
            keys = ["IPFIXE","GATEWAY","SUBNET","DNS"]
            for key in keys:
                env.Append(CPPDEFINES=[(key, "\\\"" + conf["IP"][key] + "\\\"")])

        check = conf["USE_MQTT"]
        if check and int(check) == 1:
            print("\tUsing USE_MQTT")
            env.Append(CPPDEFINES=[("USE_MQTT")])
            env.Append(CPPDEFINES=[("MQTT_PORT",  int(conf["MQTT"]["MQTT_PORT"]) )])
            keys = ["TOPICIN","TOPICOUT","MQTT_SERVER","MQTT_LOGIN","MQTT_PASSWORD"]
            for key in keys:
                env.Append(CPPDEFINES=[(key, "\\\"" + conf["MQTT"][key] + "\\\"")])

        check = conf["USE_HTTP"]
        if check and int(check) == 1:
            print("\tUsing HTTP")
            env.Append(CPPDEFINES=[("USE_HTTP")])
            env.Append(CPPDEFINES=[("DOMOTIC_SERVER", "\\\"" + conf["HTTP"]["DOMOTIC_SERVER"] + "\\\"")])
            env.Append(CPPDEFINES=[("DOMOTIC_PORT", int(conf["HTTP"]["DOMOTIC_PORT"]) )])



        check = conf["USE_WEBSERVER"]
        if check and int(check) == 1:
            print("\tUsing WEBSERVER")
            env.Append(CPPDEFINES=[("USE_WEBSERVER")])
            env.Append(CPPDEFINES=[("WEBSERVER_PORT", int(conf["WEBSERVER"]["WEBSERVER_PORT"]) )])

#
# Creation du fichier include/IOB_IOT_Conf_h
#
ConfigH()

#
# Ajoute IOB_IOT_Conf_h dans CPPDEFINES du compilateur ( !!! warning REDEFINE si absent !!! )
env.Append(CPPDEFINES=[("IOB_IOT_Conf_h")])
