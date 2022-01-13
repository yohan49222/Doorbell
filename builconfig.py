Import ("env")
import os.path
import json

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

    env.Append(CPPDEFINES=[("RELAY_PIN", int(conf["RELAY_PIN"]))])
    env.Append(CPPDEFINES=[("BUTTON_PIN", int(conf["BUTTON_PIN"]))])
    env.Append(CPPDEFINES=[("DEBOUNCE_TIME", int(conf["DEBOUNCE_TIME"]))])
    env.Append(CPPDEFINES=[("BUTTON_PRESS_COUNT_MAX", int(conf["BUTTON_PRESS_COUNT_MAX"]))])
    env.Append(CPPDEFINES=[("NOMMODULE", "\\\"" + conf["NOMMODULE"] + "\\\"")])
    env.Append(CPPDEFINES=[("IDXDEVICE", int(conf["IDXDEVICE"]))])

    check = conf["NCORNO"]
    if check and int(check) == 1:
        env.Append(CPPDEFINES=[("NCORNO", "\\\"true\\\"" )])
    else:
        env.Append(CPPDEFINES=[("NCORNO", "\\\"false\\\"" )])

    check = conf["USE_OTA"]
    if check and int(check) == 1:

        print("Using OTA")
        port = conf["OTA"]["OTAIP"]
        password = conf["OTA"]["OTAPASSWORD"]

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

    check = conf["USE_WIFI"]
    if check and int(check) == 1:
        print("Using WIFI")
        env.Append(CPPDEFINES=[("USE_WIFI")])
        env.Append(CPPDEFINES=[("MYSSID", "\\\"" + conf["WIFI"]["SSID"] + "\\\"")])
        env.Append(CPPDEFINES=[("MYSSID_PASSWORD", "\\\"" + conf["WIFI"]["SSIDPASSWORD"] + "\\\"")])

    check = conf["USE_IPFIXE"]
    if check and int(check) == 1:
        print("Using IPFIXE")
        env.Append(CPPDEFINES=[("USE_IPFIXE")])
        env.Append(CPPDEFINES=[("IPFIXE", "\\\"" + conf["IP"]["IPFIXE"] + "\\\"")])
        env.Append(CPPDEFINES=[("GATEWAY", "\\\"" + conf["IP"]["GATEWAY"] + "\\\"")])
        env.Append(CPPDEFINES=[("SUBNET", "\\\"" + conf["IP"]["SUBNET"] + "\\\"")])
        env.Append(CPPDEFINES=[("DNS", "\\\"" + conf["IP"]["DNS"] + "\\\"")])


    check = conf["USE_MQTT"]
    if check and int(check) == 1:
        print("Using USE_MQTT")
        env.Append(CPPDEFINES=[("USE_MQTT")])
        env.Append(CPPDEFINES=[("TOPICIN", "\\\"" + conf["MQTT"]["TOPICIN"] + "\\\"")])
        env.Append(CPPDEFINES=[("TOPICOUT", "\\\"" + conf["MQTT"]["TOPICOUT"] + "\\\"")])        
        env.Append(CPPDEFINES=[("MQTT_SERVER", "\\\"" + conf["MQTT"]["MQTT_SERVER"] + "\\\"")])
        env.Append(CPPDEFINES=[("MQTT_PORT",  int(conf["MQTT"]["MQTT_PORT"]) )])
        env.Append(CPPDEFINES=[("MQTT_LOGIN", "\\\"" + conf["MQTT"]["MQTT_LOGIN"] + "\\\"")])
        env.Append(CPPDEFINES=[("MQTT_PASSWORD", "\\\"" + conf["MQTT"]["MQTT_PASSWORD"] + "\\\"")])

    check = conf["USE_HTTP"]
    if check and int(check) == 1:
        print("Using HTTP")
        env.Append(CPPDEFINES=[("USE_HTTP")])
        env.Append(CPPDEFINES=[("DOMOTIC_SERVER", "\\\"" + conf["HTTP"]["DOMOTIC_SERVER"] + "\\\"")])
        env.Append(CPPDEFINES=[("DOMOTIC_PORT", int(conf["HTTP"]["DOMOTIC_PORT"]) )])



    check = conf["USE_WEBSERVER"]
    if check and int(check) == 1:
        print("Using WEBSERVER")
        env.Append(CPPDEFINES=[("USE_WEBSERVER")])
        env.Append(CPPDEFINES=[("WEBSERVER_PORT", int(conf["WEBSERVER"]["WEBSERVER_PORT"]) )])
