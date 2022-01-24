from typing import List
import json
import os.path
Import("env")
# import ipaddress


     
def AddDefine(key):
     env.Append(CPPDEFINES=[(key)])


def AddBoolean(key, value):
     env.Append(CPPDEFINES=[(key, value)])


def AddValueInCPP(conf, key, prim=""):
     if len(prim) > 0:
          value = conf[prim][key]
          try:
               if int(value) > -1:
                    env.Append(CPPDEFINES=[(key, value)])
          except:
               env.Append(CPPDEFINES=[(key, "\\\""+value+"\\\"")])
     else:
          value = conf[key]
          try:
               if int(value) > -1:
                    env.Append(CPPDEFINES=[(key, value)])
          except:
               env.Append(CPPDEFINES=[(key,  "\\\""+value+"\\\"")])


def AddValuesInCPP(conf, keys, prim=""):
     if type(keys) is list:
          for key in keys:
               AddValuesInCPP(conf, key, prim)

     elif type(keys) is str:
          try:
               if int(keys) > -1:
                    AddValueInCPP(conf, keys, prim)
          except:
               AddValueInCPP(conf, keys, prim)

     elif type(keys) is int:
          AddValueInCPP(conf, keys, prim)


if os.path.isfile('config.json'):
     lines = open('config.json', "r")
     conf = json.loads(lines.read())

     # Require
     AddValuesInCPP(conf, [
                         "RELAY_PIN", "BUTTON_PIN", "DEBOUNCE_TIME","RELAY_AUTO_OFF","RELAY_AUTO_OFF_AFTER",
                         "BUTTON_PRESS_COUNT_MAX", "IDXDEVICE", "NOMMODULE","NCORNO"])

     #check = conf["NCORNO"]
     #if check and int(check) == 1:
     #     AddBoolean("NCORNO", "true")
     #else:
     #     AddBoolean("NCORNO", "false")

     check = conf["USE_WIFI"]
     if check and int(check) == 1:
          AddDefine("USE_WIFI")
          AddValuesInCPP(conf, ["MYSSID", "MYSSID_PASSWORD"], "WIFI")

          check = conf["USE_OTA"]
          if check and int(check) == 1:
               AddDefine("USE_OTA")
               AddValuesInCPP(conf, ["OTANAME", "OTAPASSWORD"], "OTA")

               port = conf["OTA"]["OTAIP"]
               password = conf["OTA"]["OTAPASSWORD"]

               check = conf["UPLOAD_WITH_OTA"]
               if check and int(check) == 1:
                    if env["PIOENV"] == "nodemcuv2":
                         env.Replace(
                              UPLOAD_PROTOCOL="espota",
                              UPLOAD_PORT=port,
                         )
                    if password  and len(password):
                         if env["PIOENV"] == "nodemcuv2":
                              env.Replace(
                                   UPLOAD_FLAGS=["--port=8266", "--auth=" + password],
                              )

          check = conf["USE_IPFIXE"]
          if check and int(check) == 1:
               AddDefine("USE_IPFIXE")
               AddValuesInCPP(conf, ["IPFIXE", "GATEWAY", "SUBNET", "DNS"], "IP")

          check = conf["USE_MQTT"]
          if check and int(check) == 1:
               AddDefine("USE_MQTT")
               AddValuesInCPP(conf, ["TOPICIN", "TOPICOUT", "MQTT_SERVER",
                           "MQTT_LOGIN", "MQTT_PASSWORD", "MQTT_PORT","INTERVALCONNECT"], "MQTT")

          check = conf["USE_HTTP"]
          if check and int(check) == 1:
               AddDefine("USE_HTTP")
               AddValuesInCPP(conf, ["DOMOTIC_SERVER", "DOMOTIC_PORT"], "HTTP")

          check = conf["USE_WEBSERVER"]
          if check and int(check) == 1:
               AddDefine("USE_WEBSERVER")
               AddValuesInCPP(conf, ["WEBSERVER_PORT"], "WEBSERVER")

     #
     # Ajoute IOB_IOT_CONF_H dans CPPDEFINES du compilateur ( !!! warning REDEFINE si absent !!! )
     AddDefine("IOB_IOT_CONF_H")
     print(env["CPPDEFINES"])
